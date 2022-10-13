#pragma once

#include "../ComponentManager.hpp"

namespace ReactNativeWasm::Components {

class VirtualTextManager : public Manager {
public:
  VirtualTextManager() {};

  std::string getName() override;
  folly::dynamic getConstants() override;

  static const char *Name;
};
} // namespace ReactNativeWasm::Components
