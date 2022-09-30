#pragma once

#include <map>
#include <cxxreact/CxxModule.h>
#include <folly/dynamic.h>
#include <react/renderer/uimanager/UIManager.h>
#include "../../Components/ShadowNode.hpp"
#include "../../Components/ComponentManager.hpp"

using Method = facebook::xplat::module::CxxModule::Method;

namespace ReactNativeWasm {
    class UIManagerModule : public facebook::xplat::module::CxxModule {
        public:
            UIManagerModule(
                std::shared_ptr<facebook::react::UIManager> uiManager,
                std::shared_ptr<std::vector<std::shared_ptr<ReactNativeWasm::Components::Manager>>> componentManagers,
                std::shared_ptr<ReactNativeWasm::Renderer> renderer
            ):
                uiManager(uiManager),
                componentManagers(componentManagers),
                renderer(renderer) {
                    std::cout << "UIManagerModule constructor" << std::endl;
                    nodes = std::make_unique<std::map<int64_t, ReactNativeWasm::Components::ShadowNode*>>();
            };

            std::string getName();
            auto getConstants() -> std::map<std::string, folly::dynamic>;
            auto getMethods() -> std::vector<Method>;

            static const char *Name;

        private:
            std::shared_ptr<facebook::react::UIManager> uiManager;
            std::shared_ptr<std::vector<std::shared_ptr<ReactNativeWasm::Components::Manager>>> componentManagers;
            std::shared_ptr<ReactNativeWasm::Renderer> renderer;
            std::unique_ptr<std::map<int64_t, ReactNativeWasm::Components::ShadowNode*>> nodes;

            auto getManager(std::string) -> std::shared_ptr<ReactNativeWasm::Components::Manager>;
    };
}
