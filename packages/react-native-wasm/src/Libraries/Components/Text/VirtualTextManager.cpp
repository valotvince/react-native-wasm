#include "VirtualTextManager.hpp"

#include <iostream>

namespace ReactNativeWasm::Components {
    void VirtualTextShadowNode::createView(folly::dynamic props) {

    }

    const char *VirtualTextManager::Name = "RCTVirtualText";

    std::string VirtualTextManager::getName() {
        return VirtualTextManager::Name;
    }

    folly::dynamic VirtualTextManager::getConstants() {
        folly::dynamic constants = folly::dynamic::object();

        folly::dynamic nativeProps = folly::dynamic::object();

        constants.insert("NativeProps", std::move(nativeProps));

        return constants;
    }

    ShadowNode* VirtualTextManager::createShadow() {
        auto node = new VirtualTextShadowNode();

        nodes->push_back(node);

        return node;
    }
}
