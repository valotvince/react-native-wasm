#pragma once

#include "../ComponentManager.hpp"

namespace ReactNativeWasm::Components {
class ViewManager : public Manager {
public:
  ViewManager(){};

  std::string getName() override;
  folly::dynamic getConstants() override;

  static const char *Name;
};
} // namespace ReactNativeWasm::Components
