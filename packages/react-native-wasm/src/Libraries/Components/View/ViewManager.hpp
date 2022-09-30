#pragma once

#include "../ComponentManager.hpp"
#include "../ShadowNode.hpp"

#include <folly/dynamic.h>

namespace ReactNativeWasm::Components {
class ViewManager : public Manager {
public:
  ViewManager(std::shared_ptr<ReactNativeWasm::Renderer> renderer) : Manager(renderer){};

  std::string getName() override;
  folly::dynamic getConstants() override;
  ShadowNode *createShadow() override;

  static const char *Name;
};
} // namespace ReactNativeWasm::Components
