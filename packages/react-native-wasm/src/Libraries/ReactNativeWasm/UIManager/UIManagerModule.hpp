#pragma once

#include <cxxreact/CxxModule.h>
#include <react/renderer/uimanager/UIManager.h>
#include "../../Components/Component.hpp"

using Method = facebook::xplat::module::CxxModule::Method;

namespace ReactNativeWasm {
    class UIManagerModule : public facebook::xplat::module::CxxModule {
        public:
            UIManagerModule(
                std::shared_ptr<facebook::react::UIManager> uiManager,
                std::shared_ptr<std::vector<std::shared_ptr<ReactNativeWasm::Components::Component>>> components
            ): uiManager(uiManager), components(components) {};

            std::string getName();
            virtual auto getConstants() -> std::map<std::string, folly::dynamic>;
            virtual auto getMethods() -> std::vector<Method>;

            static const char *Name;

        private:
            std::shared_ptr<facebook::react::UIManager> uiManager;
            std::shared_ptr<std::vector<std::shared_ptr<ReactNativeWasm::Components::Component>>> components;
    };
}
