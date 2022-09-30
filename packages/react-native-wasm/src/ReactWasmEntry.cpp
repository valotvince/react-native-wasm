
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/threading.h>

#include <cxxreact/NativeModule.h>
#include <cxxreact/CxxNativeModule.h>
#include <cxxreact/ModuleRegistry.h>
#include <cxxreact/Instance.h>
#include <folly/dynamic.h>
#include <react/renderer/scheduler/Scheduler.h>
#include <react/renderer/scheduler/SchedulerToolbox.h>
#include <react/renderer/runtimescheduler/RuntimeScheduler.h>
#include <react/renderer/componentregistry/ComponentDescriptorProviderRegistry.h>
#include <react/utils/ContextContainer.h>
#include <react/renderer/core/EventBeat.h>
#include <react/renderer/components/view/ViewComponentDescriptor.h>
#include <react/config/ReactNativeConfig.h>

#include "Libraries/Utilities/DevSettings/DevSettings.hpp"
#include "Libraries/Utilities/PlatformConstants.hpp"
#include "Libraries/Utilities/Timing/Timing.hpp"

#include "Libraries/ReactNativeWasm/Config/ReactNativeConfig.hpp"
#include "Libraries/Components/View/ViewManager.hpp"
#include "Libraries/Components/Text/TextManager.hpp"
#include "Libraries/Components/Text/VirtualTextManager.hpp"
#include "Libraries/Components/Text/RawTextManager.hpp"
#include "Libraries/ReactNativeWasm/Bindings/JSWasmExecutor.hpp"
#include "Libraries/ReactNativeWasm/Renderer/Renderer.hpp"
#include "Libraries/ReactNativeWasm/UIManager/UIManagerModule.hpp"
#include "Libraries/ReactNativeWasm/UIManager/UIManagerAnimationDelegate.hpp"
#include "Libraries/ReactNativeWasm/Scheduler/SchedulerDelegate.hpp"
#include "ReactWasmInstance.hpp"
#include "Libraries/ReactNativeWasm/NativeQueue/NativeQueue.hpp"

using SharedNativeModuleVector = std::vector<std::shared_ptr<facebook::react::NativeModule>>;
using UniqueNativeModuleVector = std::vector<std::unique_ptr<facebook::react::NativeModule>>;
using SharedReactNativeWasmComponentManagers = std::vector<std::shared_ptr<ReactNativeWasm::Components::Manager>>;

std::shared_ptr<facebook::react::Instance> reactInstance;
std::shared_ptr<ReactNativeWasm::Instance> instance;
std::shared_ptr<ReactNativeWasm::NativeQueue> nativeQueue;
std::shared_ptr<facebook::react::Scheduler> reactScheduler;
std::shared_ptr<ReactNativeWasm::UIManagerAnimationDelegate> uiManagerAnimationDelegate;
std::shared_ptr<ReactNativeWasm::SchedulerDelegate> schedulerDelegate;

std::shared_ptr<SharedReactNativeWasmComponentManagers> componentManagers;

std::shared_ptr<ReactNativeWasm::Renderer> renderer;

UniqueNativeModuleVector getNativeModules(std::shared_ptr<facebook::react::Instance> instance, std::shared_ptr<ReactNativeWasm::NativeQueue> nativeQueue) {
    UniqueNativeModuleVector modules;

    modules.push_back(
        std::make_unique<facebook::react::CxxNativeModule>(
            instance,
            facebook::react::PlatformConstantsModule::Name,
            []() { return std::make_unique<facebook::react::PlatformConstantsModule>(); },
            nativeQueue
        )
    );

    modules.push_back(
        std::make_unique<facebook::react::CxxNativeModule>(
            instance,
            "DevSettings",
            []() { return std::make_unique<ReactNativeWasm::DevSettings>(); },
            nativeQueue
        )
    );

    modules.push_back(
        std::make_unique<facebook::react::CxxNativeModule>(
            instance,
            "UIManager",
            []() { return std::make_unique<ReactNativeWasm::UIManagerModule>(reactScheduler->getUIManager(), componentManagers, renderer); },
            nativeQueue
        )
    );

    modules.push_back(
        std::make_unique<facebook::react::CxxNativeModule>(
            instance,
            ReactNativeWasm::Timing::Name,
            []() { return std::make_unique<ReactNativeWasm::Timing>(); },
            nativeQueue
        )
    );

    return modules;
}

SharedNativeModuleVector getSharedNativeModules(std::shared_ptr<facebook::react::Instance> instance, std::shared_ptr<ReactNativeWasm::NativeQueue> nativeQueue) {
    SharedNativeModuleVector modules;

    modules.push_back(
        std::make_shared<facebook::react::CxxNativeModule>(
            instance,
            facebook::react::PlatformConstantsModule::Name,
            []() { return std::make_unique<facebook::react::PlatformConstantsModule>(); },
            nativeQueue
        )
    );

    modules.push_back(
        std::make_shared<facebook::react::CxxNativeModule>(
            instance,
            "DevSettings",
            []() { return std::make_unique<ReactNativeWasm::DevSettings>(); },
            nativeQueue
        )
    );

    modules.push_back(
        std::make_shared<facebook::react::CxxNativeModule>(
            instance,
            "UIManager",
            []() { return std::make_unique<ReactNativeWasm::UIManagerModule>(reactScheduler->getUIManager(), componentManagers, renderer); },
            nativeQueue
        )
    );

    modules.push_back(
        std::make_shared<facebook::react::CxxNativeModule>(
            instance,
            ReactNativeWasm::Timing::Name,
            []() { return std::make_unique<ReactNativeWasm::Timing>(); },
            nativeQueue
        )
    );

    return modules;
}

struct InstanceCallback : public facebook::react::InstanceCallback {
  ~InstanceCallback() override {}
  void onBatchComplete() override {}
  void incrementPendingJSCalls() override {}
  void decrementPendingJSCalls() override {}
};

auto createComponentsRegistry() -> std::shared_ptr<facebook::react::ComponentDescriptorProviderRegistry> {
    auto providerRegistry = std::make_shared<facebook::react::ComponentDescriptorProviderRegistry>();

    providerRegistry->add(
        facebook::react::concreteComponentDescriptorProvider<facebook::react::ViewComponentDescriptor>());

    return providerRegistry;
}

void run() {
    instance = std::make_shared<ReactNativeWasm::Instance>();
    nativeQueue = std::make_shared<ReactNativeWasm::NativeQueue>();
    uiManagerAnimationDelegate = std::make_shared<ReactNativeWasm::UIManagerAnimationDelegate>();
    schedulerDelegate = std::make_shared<ReactNativeWasm::SchedulerDelegate>();

    componentManagers = std::make_shared<SharedReactNativeWasmComponentManagers>();
    componentManagers->push_back(std::make_shared<ReactNativeWasm::Components::ViewManager>(renderer));
    componentManagers->push_back(std::make_shared<ReactNativeWasm::Components::RawTextManager>(renderer));
    componentManagers->push_back(std::make_shared<ReactNativeWasm::Components::VirtualTextManager>(renderer));
    componentManagers->push_back(std::make_shared<ReactNativeWasm::Components::TextManager>(renderer));

    std::cout << "toto"<<std::endl;

    reactInstance = std::make_shared<facebook::react::Instance>();

    facebook::react::ContextContainer::Shared contextContainer = std::make_shared<facebook::react::ContextContainer>();

    folly::dynamic configStore = folly::dynamic::object();

    try {
        configStore.insert("react_fabric:remove_outstanding_surfaces_on_destruction_ios", true);
        configStore.insert("react_native_new_architecture:enable_call_immediates_ios", true);
    } catch (std::exception e) {
        std::cerr << "Exception 2: " << e.what() << std::endl;

        throw e;
    }

    std::shared_ptr<const facebook::react::ReactNativeConfig> config =
        std::make_shared<const ReactNativeWasm::ReactNativeConfig>(configStore);
    contextContainer->insert("ReactNativeConfig", config);

    auto runtimeExecutor = reactInstance->getRuntimeExecutor();
    auto runtimeScheduler = std::make_shared<facebook::react::RuntimeScheduler>(runtimeExecutor);

    contextContainer->insert(
          "RuntimeScheduler",
          std::weak_ptr<facebook::react::RuntimeScheduler>(runtimeScheduler));

    facebook::react::EventBeat::Factory synchronousBeatFactory =
        [](
            facebook::react::EventBeat::SharedOwnerBox const &ownerBox)
        -> std::unique_ptr<facebook::react::EventBeat> {
        return std::make_unique<facebook::react::EventBeat>(ownerBox);
    };

    facebook::react::EventBeat::Factory asynchronousBeatFactory =
        [](
            facebook::react::EventBeat::SharedOwnerBox const &ownerBox)
        -> std::unique_ptr<facebook::react::EventBeat> {
        return std::make_unique<facebook::react::EventBeat>(ownerBox);
    };

    auto componentsRegistry = createComponentsRegistry();

    auto toolbox = facebook::react::SchedulerToolbox{};
    toolbox.contextContainer = contextContainer;
    toolbox.componentRegistryFactory = [componentsRegistry](facebook::react::EventDispatcher::Weak const &eventDispatcher,
                                            facebook::react::ContextContainer::Shared const &contextContainer)
        -> facebook::react::ComponentDescriptorRegistry::Shared {
        auto registry = componentsRegistry->createComponentDescriptorRegistry({eventDispatcher, contextContainer});
        // Enabling the fallback component will require us to run the view component codegen to generate
        // UnimplementedNativeViewComponentDescriptor
        /*
        auto mutableRegistry = std::const_pointer_cast<facebook::react::ComponentDescriptorRegistry>(registry);
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
    toolbox.backgroundExecutor = [](std::function<void()> &&callback){
        nativeQueue->runOnQueue(std::move(callback));
    };

    try {
        reactScheduler = std::make_shared<facebook::react::Scheduler>(
            toolbox,
            uiManagerAnimationDelegate.get(),
            schedulerDelegate.get()
        );
    } catch (std::exception e) {
        std::cerr << "Exception: " << e.what() << std::endl;

        throw e;
    }

    auto moduleRegistry = std::make_shared<facebook::react::ModuleRegistry>(std::move(getNativeModules(reactInstance, nativeQueue)));
    auto sharedModules = std::make_shared<SharedNativeModuleVector>(getSharedNativeModules(reactInstance, nativeQueue));

    reactInstance->initializeBridge(
        std::make_unique<InstanceCallback>(),
        std::make_shared<ReactNativeWasm::JSWasmExecutorFactory>(),
        nativeQueue,
        moduleRegistry
    );

    instance->setupRuntime(moduleRegistry, std::move(sharedModules));
    instance->loadBundle();

    std::cout << "After loadBundle" << std::endl;

    // while (true) {}
}

// SDL_Window *window;
// SDL_Renderer *renderer;

void loop() {
    renderer->flush();
}

int main(int argc, char* argv[]) {
    renderer = std::make_shared<ReactNativeWasm::Renderer>();

    run();

    emscripten_set_main_loop(loop, 0, 1);
    // emscripten_exit_with_live_runtime();

    return 0;
}

void onBundleLoaded() {
    folly::dynamic params = folly::dynamic::array(
    std::move("main"),
    folly::dynamic::object("initialProps", {})("rootTag", 11)(
        "fabric", false));

    reactInstance->callJSFunction("AppRegistry", "runApplication", std::move(params));
}

EMSCRIPTEN_BINDINGS(ReactWasmEntry) {
    emscripten::function("__onBundleLoaded", &onBundleLoaded);
}
