#include "VirtualTextManager.hpp"

#include <iostream>

namespace ReactNativeWasm::Components {

const char *VirtualTextManager::Name = "RCTVirtualText";

std::string VirtualTextManager::getName() { return VirtualTextManager::Name; }

folly::dynamic VirtualTextManager::getConstants() {
  folly::dynamic constants = folly::dynamic::object();

  folly::dynamic nativeProps = folly::dynamic::object();

  constants.insert("NativeProps", std::move(nativeProps));

  return constants;
}
} // namespace ReactNativeWasm::Components
