#include "ReactWasmInstance.hpp"

#include <iostream>
#include <cxxreact/JSBigString.h>
#include <folly/json.h>
#include <ReactCommon/TurboModuleBinding.h>
#include <ReactCommon/TurboModule.h>
#include <stdint.h>
#include <folly/dynamic.h>
#include <emscripten/bind.h>
#include <emscripten/threading.h>
#include <cxxreact/CxxNativeModule.h>
#include <cxxreact/JsArgumentHelpers.h>

#include "Libraries/Utilities/Timing/Timing.hpp"
#include "Libraries/Utilities/JavascriptAccessor/JavascriptAccessor.hpp"

namespace ReactNativeWasm {
    Instance::~Instance() {
        std::cout << "Instance DELETED" << std::endl;
    }

    void Instance::loadBundle() {
        ReactNativeWasm::JavascriptAccessor::insertScriptTag("react-native.bundle.js");
    };

    int callId = 0;
    std::shared_ptr<facebook::react::ModuleRegistry> registry;
    std::shared_ptr<std::vector<std::shared_ptr<facebook::react::NativeModule>>> modules;

    void Instance::setupRuntime(std::shared_ptr<facebook::react::ModuleRegistry> moduleRegistry, std::shared_ptr<std::vector<std::shared_ptr<facebook::react::NativeModule>>> modules) {
        ReactNativeWasm::registry = moduleRegistry;
        ReactNativeWasm::modules = modules;

        // TODO: Find a way to use the same modules as ModuleRegistry
        // For lazy init of CxxNativeModule
        for(auto it = ReactNativeWasm::modules->begin(); it != ReactNativeWasm::modules->end(); ++it) {
            it->get()->getConstants();
        }

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

void nativeCallSyncHook(unsigned int moduleId, unsigned int methodId, std::string jsonArgs) {
    auto args = folly::parseJson(jsonArgs);

    auto module = ReactNativeWasm::modules->at(moduleId);

    if (!module) {
        throw std::invalid_argument("Module id " + std::to_string(moduleId) + " doesn't exist");
    }

    module->invoke(methodId, std::move(args), 100);
}

EMSCRIPTEN_BINDINGS(ReactWasmInstance) {
    emscripten::class_<facebook::react::NativeModule>("NativeModule")
        .smart_ptr<std::shared_ptr<facebook::react::NativeModule>>("NativeModule")
        .function("getName", &facebook::react::NativeModule::getName)
        .function("getConstants", emscripten::optional_override([](facebook::react::NativeModule& self) {
            std::string constants(std::make_unique<facebook::react::JSBigStdString>(folly::toJson(self.getConstants()))->c_str());

            return constants;
        }))
        .function("invoke", emscripten::optional_override([](facebook::react::NativeModule& self, std::string methodName, std::string jsonArgs) {
            auto args = folly::parseJson(jsonArgs);

            auto methods = self.getMethods();
            auto it = methods.begin();

            std::cout << "Trying invoking " << self.getName() << "::" << methodName << std::endl;

            for (it = methods.begin(); it != methods.end(); ++it) {
                if (it->name == methodName) {
                    auto reactMethodId = std::distance(methods.begin(), it);

                    std::cout << self.getName() << " invoking " << methodName << std::endl;

                    return self.invoke(reactMethodId, std::move(args), ++ReactNativeWasm::callId);
                }
            }

            throw std::invalid_argument("Method name " + methodName + " doesn't exist on NativeModule " + self.getName());
        }))
        .function("callSerializableNativeHook", emscripten::optional_override([](facebook::react::NativeModule& self, std::string methodName, std::string jsonArgs) {
            auto args = folly::parseJson(jsonArgs);

            auto methods = self.getMethods();
            auto it = methods.begin();

            std::cout << "Trying callSerializableNativeHook " << self.getName() << "::" << methodName << std::endl;

            for (it = methods.begin(); it != methods.end(); ++it) {
                if (it->name == methodName) {
                    auto reactMethodId = std::distance(methods.begin(), it);

                    std::cout << self.getName() << " callSerializableNativeHook " << methodName << std::endl;

                    auto result = self.callSerializableNativeHook(reactMethodId, std::move(args));

                    if (result.hasValue()) {
                        std::string resultAsJson(std::make_unique<facebook::react::JSBigStdString>(folly::toJson(result.value()))->c_str());

                        return resultAsJson;
                    }

                    std::string fake;

                    return fake;
                }
            }

            throw std::invalid_argument("Method name " + methodName + " doesn't exist on NativeModule " + self.getName());
        }));

    emscripten::function("nativeCallSyncHook", &nativeCallSyncHook);
    emscripten::function("__turboModuleProxy", &turboModuleProxy, emscripten::allow_raw_pointers());
}
