#include "TextManager.hpp"

#include <iostream>

namespace ReactNativeWasm::Components {
    TextManager::TextManager() {}

    const char *TextManager::Name = "RCTVirtualText";

    std::string TextManager::getName() {
        return TextManager::Name;
    }

    folly::dynamic TextManager::getConstants() {
        folly::dynamic constants = folly::dynamic::object();

        folly::dynamic nativeProps = folly::dynamic::object();

        constants.insert("NativeProps", std::move(nativeProps));

        return constants;
    }

    void createView() {

    }
}
