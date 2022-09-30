#pragma once

#include "../ComponentManager.hpp"
#include "../ShadowNode.hpp"

#include <folly/dynamic.h>

namespace ReactNativeWasm::Components {
    class VirtualTextShadowNode : public ShadowNode {
        public:
            VirtualTextShadowNode() {};
            void createView(folly::dynamic props) override;
    };

    class VirtualTextManager : public Manager {
        public:
            VirtualTextManager(std::shared_ptr<ReactNativeWasm::Renderer> renderer): Manager(renderer) {};

            std::string getName() override;
            folly::dynamic getConstants() override;
            ShadowNode* createShadow() override;

            static const char *Name;
    };
}
