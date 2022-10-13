#pragma once

#include "../ComponentManager.hpp"

namespace ReactNativeWasm::Components {

class TextManager : public Manager {
public:
  TextManager() {};

  std::string getName() override;
  folly::dynamic getConstants() override;

  static const char *Name;
};
} // namespace ReactNativeWasm::Components
