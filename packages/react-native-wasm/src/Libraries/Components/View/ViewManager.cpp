#include "ViewManager.hpp"

#include <iostream>

namespace ReactNativeWasm::Components {

const char *ViewManager::Name = "RCTView";

std::string ViewManager::getName() { return ViewManager::Name; }

folly::dynamic ViewManager::getConstants() {
  folly::dynamic constants = folly::dynamic::object();

  folly::dynamic nativeProps = folly::dynamic::object();

  constants.insert("NativeProps", std::move(nativeProps));

  return constants;
}
} // namespace ReactNativeWasm::Components
