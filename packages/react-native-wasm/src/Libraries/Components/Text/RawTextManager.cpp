#include "RawTextManager.hpp"

#include <iostream>

namespace ReactNativeWasm::Components {
const char *RawTextManager::Name = "RCTRawText";

std::string RawTextManager::getName() { return RawTextManager::Name; }

folly::dynamic RawTextManager::getConstants() {
  folly::dynamic constants = folly::dynamic::object();

  folly::dynamic nativeProps = folly::dynamic::object();

  constants.insert("NativeProps", std::move(nativeProps));

  return constants;
}
} // namespace ReactNativeWasm::Components
