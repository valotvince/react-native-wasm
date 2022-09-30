#pragma once

#include <folly/dynamic.h>
#include <vector>

#include "ShadowNode.hpp"
#include "../ReactNativeWasm/Renderer/Renderer.hpp"

namespace ReactNativeWasm::Components {
    class Manager {
        public:
            Manager(std::shared_ptr<ReactNativeWasm::Renderer> renderer): renderer(renderer) {
                nodes = std::make_unique<std::vector<ReactNativeWasm::Components::ShadowNode*>>();
            };

            virtual std::string getName() = 0;
            virtual folly::dynamic getConstants() = 0;
            virtual ReactNativeWasm::Components::ShadowNode* createShadow() = 0;

            static const char *Name;

        protected:
            std::unique_ptr<std::vector<ReactNativeWasm::Components::ShadowNode*>> nodes;
            std::shared_ptr<ReactNativeWasm::Renderer> renderer;
    };
}
