#pragma once

#include <folly/dynamic.h>
#include <vector>

namespace ReactNativeWasm::Components {
class Manager {
public:
  Manager(){};

  virtual std::string getName() = 0;
  virtual folly::dynamic getConstants() = 0;

  static const char *Name;
};
} // namespace ReactNativeWasm::Components
