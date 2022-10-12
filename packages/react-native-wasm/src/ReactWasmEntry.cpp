
#include <chrono>
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#include <emscripten/threading.h>
#include <iostream>
#include <mutex>
#include <thread>

#include <ReactCommon/TurboCxxModule.h>
#include <ReactCommon/TurboModuleBinding.h>
#include <cxxreact/CxxNativeModule.h>
#include <cxxreact/Instance.h>
#include <cxxreact/ModuleRegistry.h>
#include <cxxreact/NativeModule.h>
#include <folly/dynamic.h>
#include <react/config/ReactNativeConfig.h>
#include <react/renderer/componentregistry/ComponentDescriptorProviderRegistry.h>
#include <react/renderer/components/view/ViewComponentDescriptor.h>
#include <react/renderer/core/EventBeat.h>
#include <react/renderer/core/LayoutConstraints.h>
#include <react/renderer/core/LayoutContext.h>
#include <react/renderer/core/LayoutPrimitives.h>
#include <react/renderer/core/ReactPrimitives.h>
#include <react/renderer/mounting/ShadowTree.h>
#include <react/renderer/runtimescheduler/RuntimeScheduler.h>
#include <react/renderer/scheduler/Scheduler.h>
#include <react/renderer/scheduler/SchedulerToolbox.h>
#include <react/renderer/scheduler/SurfaceHandler.h>
#include <react/utils/ContextContainer.h>

#include "Libraries/Utilities/DevSettings/DevSettings.hpp"
#include "Libraries/Utilities/DeviceInfo/DeviceInfo.hpp"
#include "Libraries/Utilities/PlatformConstants.hpp"
#include "Libraries/Utilities/SourceCode/SourceCode.hpp"
#include "Libraries/Utilities/StatusBarManager/StatusBarManager.hpp"
#include "Libraries/Utilities/Timing/Timing.hpp"

#include "Libraries/Components/Text/RawTextManager.hpp"
#include "Libraries/Components/Text/TextManager.hpp"
#include "Libraries/Components/Text/VirtualTextManager.hpp"
#include "Libraries/Components/View/ViewManager.hpp"
#include "Libraries/ReactNativeWasm/Bindings/JSWasmExecutor.hpp"
#include "Libraries/ReactNativeWasm/Config/ReactNativeConfig.hpp"
#include "Libraries/ReactNativeWasm/NativeQueue/NativeQueue.hpp"
#include "Libraries/ReactNativeWasm/Renderer/Renderer.hpp"
#include "Libraries/ReactNativeWasm/Scheduler/SchedulerDelegate.hpp"
#include "Libraries/ReactNativeWasm/UIManager/UIManagerAnimationDelegate.hpp"
#include "Libraries/ReactNativeWasm/UIManager/UIManagerModule.hpp"
#include "Libraries/Utilities/JavascriptAccessor/JavascriptAccessor.hpp"

using UniqueCxxNativeModuleMap = std::unordered_map<std::string, facebook::xplat::module::CxxModule::Provider>;
using UniqueNativeModuleVector = std::vector<std::unique_ptr<facebook::react::NativeModule>>;
using SharedReactNativeWasmComponentManagers = std::vector<std::shared_ptr<ReactNativeWasm::Components::Manager>>;
using TurboModuleCache = std::unordered_map<std::string, std::shared_ptr<facebook::react::TurboModule>>;

std::shared_ptr<facebook::react::Instance> reactInstance;
std::shared_ptr<ReactNativeWasm::NativeQueue> nativeQueue;
std::shared_ptr<facebook::react::Scheduler> reactScheduler;
std::shared_ptr<ReactNativeWasm::UIManagerAnimationDelegate> uiManagerAnimationDelegate;
std::shared_ptr<ReactNativeWasm::SchedulerDelegate> schedulerDelegate;

std::shared_ptr<SharedReactNativeWasmComponentManagers> componentManagers;

std::shared_ptr<ReactNativeWasm::Renderer> renderer;

std::shared_ptr<facebook::react::LongLivedObjectCollection> longLivedObjectCollection_;
std::shared_ptr<TurboModuleCache> turboModuleCache;

std::shared_ptr<UniqueCxxNativeModuleMap> nativeModules;

UniqueCxxNativeModuleMap getCxxNativeModules() {
  UniqueCxxNativeModuleMap modules;

  modules.insert({"PlatformConstants", []() { return std::make_unique<facebook::react::PlatformConstantsModule>(); }});
  modules.insert({"DevSettings", []() { return std::make_unique<ReactNativeWasm::DevSettings>(); }});
  modules.insert({"UIManager", []() {
                    return std::make_unique<ReactNativeWasm::UIManagerModule>(
                      reactScheduler->getUIManager(), componentManagers, renderer);
                  }});
  modules.insert({"Timing", []() { return std::make_unique<ReactNativeWasm::Timing>(); }});
  modules.insert({"DeviceInfo", []() { return std::make_unique<ReactNativeWasm::DeviceInfo>(); }});
  modules.insert({"SourceCode", []() { return std::make_unique<ReactNativeWasm::SourceCode>(); }});
  modules.insert({"StatusBarManager", []() { return std::make_unique<ReactNativeWasm::StatusBarManager>(); }});

  return modules;
}

UniqueNativeModuleVector getNativeModules(
  std::shared_ptr<facebook::react::Instance> instance, std::shared_ptr<ReactNativeWasm::NativeQueue> nativeQueue) {
  UniqueNativeModuleVector modules;

  modules.push_back(std::make_unique<facebook::react::CxxNativeModule>(
    instance, facebook::react::PlatformConstantsModule::Name,
    []() { return std::make_unique<facebook::react::PlatformConstantsModule>(); }, nativeQueue));

  modules.push_back(std::make_unique<facebook::react::CxxNativeModule>(
    instance, "DevSettings", []() { return std::make_unique<ReactNativeWasm::DevSettings>(); }, nativeQueue));

  modules.push_back(std::make_unique<facebook::react::CxxNativeModule>(
    instance, "DeviceInfo", []() { return std::make_unique<ReactNativeWasm::DevSettings>(); }, nativeQueue));

  modules.push_back(std::make_unique<facebook::react::CxxNativeModule>(
    instance, "SourceCode", []() { return std::make_unique<ReactNativeWasm::SourceCode>(); }, nativeQueue));

  modules.push_back(std::make_unique<facebook::react::CxxNativeModule>(
    instance, "StatusBarManager", []() { return std::make_unique<ReactNativeWasm::StatusBarManager>(); }, nativeQueue));

  modules.push_back(std::make_unique<facebook::react::CxxNativeModule>(
    instance, "UIManager",
    []() {
      return std::make_unique<ReactNativeWasm::UIManagerModule>(
        reactScheduler->getUIManager(), componentManagers, renderer);
    },
    nativeQueue));

  modules.push_back(std::make_unique<facebook::react::CxxNativeModule>(
    instance, ReactNativeWasm::Timing::Name, []() { return std::make_unique<ReactNativeWasm::Timing>(); },
    nativeQueue));

  return modules;
}

struct InstanceCallback : public facebook::react::InstanceCallback {
  ~InstanceCallback() override {}
  void onBatchComplete() override {
    // Should render the layout on batch complete
    std::cout << "onBatchComplete" << std::endl;

    renderer->flush();
  }
  void incrementPendingJSCalls() override {}
  void decrementPendingJSCalls() override {}
};

auto createComponentsRegistry() -> std::shared_ptr<facebook::react::ComponentDescriptorProviderRegistry> {
  auto providerRegistry = std::make_shared<facebook::react::ComponentDescriptorProviderRegistry>();

  providerRegistry->add(
    facebook::react::concreteComponentDescriptorProvider<facebook::react::ViewComponentDescriptor>());

  return providerRegistry;
}

void initReactInstance() {
  nativeQueue = std::make_shared<ReactNativeWasm::NativeQueue>();
  reactInstance = std::make_shared<facebook::react::Instance>();

  auto moduleRegistry =
    std::make_shared<facebook::react::ModuleRegistry>(std::move(getNativeModules(reactInstance, nativeQueue)));

  reactInstance->initializeBridge(
    std::make_unique<InstanceCallback>(), std::make_shared<ReactNativeWasm::JSWasmExecutorFactory>(), nativeQueue,
    moduleRegistry);
}

void initReactScheduler() {
  uiManagerAnimationDelegate = std::make_shared<ReactNativeWasm::UIManagerAnimationDelegate>();
  schedulerDelegate = std::make_shared<ReactNativeWasm::SchedulerDelegate>();

  componentManagers = std::make_shared<SharedReactNativeWasmComponentManagers>();
  componentManagers->push_back(std::make_shared<ReactNativeWasm::Components::ViewManager>(renderer));
  componentManagers->push_back(std::make_shared<ReactNativeWasm::Components::RawTextManager>(renderer));
  componentManagers->push_back(std::make_shared<ReactNativeWasm::Components::VirtualTextManager>(renderer));
  componentManagers->push_back(std::make_shared<ReactNativeWasm::Components::TextManager>(renderer));

  facebook::react::ContextContainer::Shared contextContainer = std::make_shared<facebook::react::ContextContainer>();

  folly::dynamic configStore = folly::dynamic::object();

  configStore.insert("react_fabric:remove_outstanding_surfaces_on_destruction_ios", true);
  configStore.insert("react_native_new_architecture:enable_call_immediates_ios", true);

  std::shared_ptr<const facebook::react::ReactNativeConfig> config =
    std::make_shared<const ReactNativeWasm::ReactNativeConfig>(configStore);
  contextContainer->insert("ReactNativeConfig", config);

  auto runtimeExecutor = reactInstance->getRuntimeExecutor();
  auto runtimeScheduler = std::make_shared<facebook::react::RuntimeScheduler>(runtimeExecutor);

  contextContainer->insert("RuntimeScheduler", std::weak_ptr<facebook::react::RuntimeScheduler>(runtimeScheduler));

  facebook::react::EventBeat::Factory synchronousBeatFactory =
    [](facebook::react::EventBeat::SharedOwnerBox const &ownerBox) -> std::unique_ptr<facebook::react::EventBeat> {
    return std::make_unique<facebook::react::EventBeat>(ownerBox);
  };

  facebook::react::EventBeat::Factory asynchronousBeatFactory =
    [](facebook::react::EventBeat::SharedOwnerBox const &ownerBox) -> std::unique_ptr<facebook::react::EventBeat> {
    return std::make_unique<facebook::react::EventBeat>(ownerBox);
  };

  auto componentsRegistry = createComponentsRegistry();

  auto toolbox = facebook::react::SchedulerToolbox{};
  toolbox.contextContainer = contextContainer;
  toolbox.componentRegistryFactory = [componentsRegistry](
                                       facebook::react::EventDispatcher::Weak const &eventDispatcher,
                                       facebook::react::ContextContainer::Shared const &contextContainer)
    -> facebook::react::ComponentDescriptorRegistry::Shared {
    auto registry = componentsRegistry->createComponentDescriptorRegistry({eventDispatcher, contextContainer});
    // Enabling the fallback component will require us to run the view component
    // codegen to generate UnimplementedNativeViewComponentDescriptor
    /*
    auto mutableRegistry =
    std::const_pointer_cast<facebook::react::ComponentDescriptorRegistry>(registry);
    mutableRegistry->setFallbackComponentDescriptor(
        std::make_shared<facebook::react::UnimplementedNativeViewComponentDescriptor>(
            facebook::react::ComponentDescriptorParameters{
                eventDispatcher, contextContainer, nullptr}));
    */
    return registry;
  };
  toolbox.runtimeExecutor = runtimeExecutor;
  toolbox.synchronousEventBeatFactory = synchronousBeatFactory;
  toolbox.asynchronousEventBeatFactory = asynchronousBeatFactory;

  // backgroundExecutor_ = JBackgroundExecutor::create("fabric_bg");
  toolbox.backgroundExecutor = [](std::function<void()> &&callback) { nativeQueue->runOnQueue(std::move(callback)); };

  reactScheduler =
    std::make_shared<facebook::react::Scheduler>(toolbox, uiManagerAnimationDelegate.get(), schedulerDelegate.get());
}

void installTurboModulesBindings() {
  nativeModules = std::make_shared<UniqueCxxNativeModuleMap>(getCxxNativeModules());

  turboModuleCache = std::make_shared<TurboModuleCache>();
  longLivedObjectCollection_ = std::make_shared<facebook::react::LongLivedObjectCollection>();

  auto jsInvoker = reactInstance->getJSCallInvoker();

  auto turboModuleInvoker = [turboModuleCache = std::weak_ptr<TurboModuleCache>(turboModuleCache),
                             jsInvoker = std::weak_ptr<facebook::react::CallInvoker>(jsInvoker),
                             nativeModules = std::weak_ptr<UniqueCxxNativeModuleMap>(nativeModules)](
                              const std::string &name) -> std::shared_ptr<facebook::react::TurboModule> {
    auto turboModuleCacheLocked = turboModuleCache.lock();
    auto jsInvokerLocked = jsInvoker.lock();
    auto nativeModulesLocked = nativeModules.lock();

    std::cout << "Searching for TurboModule " << name << std::endl;

    if (!turboModuleCacheLocked || !jsInvokerLocked) {
      return nullptr;
    }

    std::cout << "lock acquired" << std::endl;

    auto turboModuleLookup = turboModuleCacheLocked->find(name);
    if (turboModuleLookup != turboModuleCacheLocked->end()) {
      std::cout << "Found Module cached !" << std::endl;

      return turboModuleLookup->second;
    }

    auto nativeModuleLookup = nativeModulesLocked->find(name);

    if (nativeModuleLookup != nativeModulesLocked->end()) {
      std::cout << "Found Module !" << std::endl;

      auto turboModule =
        std::make_shared<facebook::react::TurboCxxModule>(nativeModuleLookup->second(), jsInvokerLocked);

      turboModuleCacheLocked->insert({nativeModuleLookup->first, turboModule});

      return turboModule;
    }

    return nullptr;
  };

  auto javascriptContext = reactInstance->getJavaScriptContext();
  auto runtime = static_cast<facebook::jsi::Runtime *>(javascriptContext);

  facebook::react::TurboModuleBinding::install(
    *runtime, std::move(turboModuleInvoker), facebook::react::TurboModuleBindingMode::HostObject,
    longLivedObjectCollection_);
}

void run() {
  initReactInstance();
  installTurboModulesBindings();
  initReactScheduler();

  ReactNativeWasm::JavascriptAccessor::insertScriptTag("react-native.bundle.js");

  std::cout << "After loadBundle" << std::endl;
}

int main(int argc, char *argv[]) {
  renderer = std::make_shared<ReactNativeWasm::Renderer>();

  run();

  emscripten_exit_with_live_runtime();

  return 0;
}

void onBundleLoaded() {
  std::cout << "Bundle loaded !" << std::endl;

  auto surfaceId = 11;

  auto layoutContext = facebook::react::LayoutContext{};
  layoutContext.pointScaleFactor = 1;

  auto layoutConstraints = facebook::react::LayoutConstraints{};
  layoutConstraints.layoutDirection = facebook::react::LayoutDirection::LeftToRight;

  auto surfaceHandler = facebook::react::SurfaceHandler{"main", surfaceId};
  surfaceHandler.setContextContainer(reactScheduler->getContextContainer());
  surfaceHandler.setProps(folly::dynamic::object());
  surfaceHandler.constraintLayout(layoutConstraints, layoutContext);

  reactScheduler->registerSurface(surfaceHandler);

  surfaceHandler.start();
}

EMSCRIPTEN_BINDINGS(ReactWasmEntry) { emscripten::function("__onBundleLoaded", &onBundleLoaded); }
