#include "ViewManager.hpp"

#include <iostream>

namespace ReactNativeWasm::Components {
    ViewManager::ViewManager() {
        std::cout << "Creating ViewManager::ViewManager() " << std::endl;
    }

    const char *ViewManager::Name = "RCTView";

    std::string ViewManager::getName() {
        std::cout << "ViewManager::getName() " <<  ViewManager::Name << std::endl;

        return ViewManager::Name;
    }

    folly::dynamic ViewManager::getConstants() {
        std::cout << "ViewManager::getConstants() " << std::endl;

        folly::dynamic constants = folly::dynamic::object();

        folly::dynamic nativeProps = folly::dynamic::object();

        constants.insert("NativeProps", std::move(nativeProps));

        return constants;
    }

    void createView() {

    }
}
