#include "UIManagerModule.hpp"

#include <iostream>
#include <cxxreact/JsArgumentHelpers.h>

namespace ReactNativeWasm {
    const char* UIManagerModule::Name = "UIManager";

    auto UIManagerModule::getName() -> std::string {
        return UIManagerModule::Name;
    }

    auto UIManagerModule::getConstants() -> std::map<std::string, folly::dynamic> {
        return {};
    }

    auto UIManagerModule::getMethods() -> std::vector<Method> {
        return {
            Method(
                "createView",
                [this](folly::dynamic args) {
                    auto reactTag = facebook::xplat::jsArgAsInt(args, 0);
                    auto viewName = facebook::xplat::jsArgAsString(args, 1);
                    auto rootTag = facebook::xplat::jsArgAsDynamic(args, 2);
                    auto props = facebook::xplat::jsArgAsDynamic(args, 3);
                }
            ),
            Method(
                "getConstantsForViewManager",
                [this](folly::dynamic args) {
                    auto viewManagerName = facebook::xplat::jsArgAsString(args, 0);

                    std::cout << "UIManager::getConstantsForViewManager(" << viewManagerName << ")" << std::endl;
                }
            )
        };
    }
}
