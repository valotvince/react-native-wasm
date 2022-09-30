#pragma once

#include "../ComponentManager.hpp"
#include "../ShadowNode.hpp"
#include "../ComponentView.hpp"

#include <folly/dynamic.h>

namespace ReactNativeWasm::Components {
    class RawTextView : public ComponentView {
        public:
            RawTextView(folly::dynamic props): ComponentView(props) {};
    };

    class RawTextShadowNode : public ShadowNode {
        public:
            RawTextShadowNode() {};
            void createView(folly::dynamic props) override;
    };

    class RawTextManager : public Manager {
        public:
            RawTextManager(std::shared_ptr<ReactNativeWasm::Renderer> renderer): Manager(renderer) {};

            std::string getName() override;
            folly::dynamic getConstants() override;
            ShadowNode* createShadow() override;

            static const char *Name;
    };
}
