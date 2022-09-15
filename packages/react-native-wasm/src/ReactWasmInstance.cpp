#include "ReactWasmInstance.hpp"

#include <iostream>
#include <cxxreact/JSBigString.h>
#include <folly/json.h>
#include <ReactCommon/TurboModuleBinding.h>
#include <ReactCommon/TurboModule.h>
#include <stdint.h>
#include <folly/dynamic.h>
#include <emscripten/bind.h>
#include <cxxreact/CxxNativeModule.h>

// #include "Libraries/ReactNativeWasm/Runtime/Runtime.hpp"
#include "Libraries/ReactNativeWasm/Turbo/TurboModuleManager.hpp"
#include "./Libraries/Utilities/JavascriptAccessor/JavascriptAccessor.hpp"

namespace ReactNativeWasm {
    void Instance::loadBundle() {
        ReactNativeWasm::JavascriptAccessor::insertScriptTag("react-native.bundle.js");
    };

    std::shared_ptr<facebook::react::ModuleRegistry> registry;
    std::shared_ptr<std::vector<std::shared_ptr<facebook::react::NativeModule>>> modules;

    void Instance::setupRuntime(std::shared_ptr<facebook::react::ModuleRegistry> moduleRegistry, std::shared_ptr<std::vector<std::shared_ptr<facebook::react::NativeModule>>> modules) {
        ReactNativeWasm::registry = moduleRegistry;
        ReactNativeWasm::modules = modules;
        // auto runtime = std::make_shared<ReactNativeWasm::Runtime>();

        std::cout << "Modules" << std::endl;
        std::cout << modules->size() << std::endl;

        folly::dynamic configArray = folly::dynamic::array;

        for (auto const &moduleName : moduleRegistry->moduleNames()) {
            auto moduleConfig = moduleRegistry->getConfig(moduleName);
            configArray.push_back(moduleConfig ? std::move(moduleConfig->config) : nullptr);
        }

        folly::dynamic config = folly::dynamic::object("remoteModuleConfig", std::move(configArray));

        ReactNativeWasm::JavascriptAccessor::setGlobalVariable(
            "__fbBatchedBridgeConfig", std::make_unique<facebook::react::JSBigStdString>(folly::toJson(config))->c_str());

        // auto turboModuleManager = std::make_shared<facebook::react::TurboModuleManager>(moduleRegistry, this->getJSCallInvoker());

        // auto binding = [turboModuleManager](const std::string &name) -> std::shared_ptr<TurboModule> {
        //     return turboModuleManager->getModule(name);
        // };

        // EMSCRIPTEN_KEEPALIVE
        // auto turboModuleProxy = [](const std::string &name) -> void {
        //     std::cout << "__turboModuleProxy called" + name << std::endl;
        // };

        // ReactNativeWasm::JavascriptAccessor::setGlobalVariablePointer("__turboModuleProxy",  reinterpret_cast<uintptr_t>(&binding));

        // TurboModuleBinding::install(
        //     *runtime,
        //     std::function(binding),
        //     TurboModuleBindingMode::HostObject,
        //     // Needs to change to a longliveCollection for bundle-less react (in memory)
        //     nullptr);

        std::cout << "After setupRuntime" << std::endl;
    };
}


std::shared_ptr<facebook::react::NativeModule> turboModuleProxy(std::string name) {
    for(auto it = ReactNativeWasm::modules->begin(); it != ReactNativeWasm::modules->end(); ++it) {
        if (it->get()->getName() == name) {
            return ReactNativeWasm::modules->at(std::distance(ReactNativeWasm::modules->begin(), it));
        }
    }

    return nullptr;
}

EMSCRIPTEN_BINDINGS(ReactNativeWasm) {
    emscripten::class_<facebook::react::NativeModule>("NativeModule")
        .smart_ptr<std::shared_ptr<facebook::react::NativeModule>>("NativeModule")
        .function("getName", &facebook::react::NativeModule::getName)
        .function("getConstants", emscripten::optional_override([](facebook::react::NativeModule& self) {
            std::string constants(std::make_unique<facebook::react::JSBigStdString>(folly::toJson(self.getConstants()))->c_str());

            return constants;
        }));

    emscripten::function("__turboModuleProxy", &turboModuleProxy, emscripten::allow_raw_pointers());
}
