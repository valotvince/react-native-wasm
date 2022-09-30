#include "RawTextManager.hpp"

#include <iostream>

namespace ReactNativeWasm::Components {
    void RawTextShadowNode::createView(folly::dynamic props) {
        view = std::make_shared<RawTextView>(props);
    }

    const char *RawTextManager::Name = "RCTRawText";

    std::string RawTextManager::getName() {
        return RawTextManager::Name;
    }

    folly::dynamic RawTextManager::getConstants() {
        folly::dynamic constants = folly::dynamic::object();

        folly::dynamic nativeProps = folly::dynamic::object();

        constants.insert("NativeProps", std::move(nativeProps));

        return constants;
    }

    ShadowNode* RawTextManager::createShadow() {
        auto node = new RawTextShadowNode();

        nodes->push_back(node);

        return node;
    }
}
