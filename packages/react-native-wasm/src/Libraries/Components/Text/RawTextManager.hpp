#pragma once

#include "../ComponentManager.hpp"

namespace ReactNativeWasm::Components {

class RawTextManager : public Manager {
public:
  RawTextManager() {};

  std::string getName() override;
  folly::dynamic getConstants() override;

  static const char *Name;
};
} // namespace ReactNativeWasm::Components
