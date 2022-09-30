#pragma once

#include "../ComponentManager.hpp"
#include "../ShadowNode.hpp"

#include <folly/dynamic.h>

namespace ReactNativeWasm::Components {
class TextShadowNode : public ShadowNode {
public:
  TextShadowNode(){};
  void createView(folly::dynamic props) override;
};

class TextManager : public Manager {
public:
  TextManager(std::shared_ptr<ReactNativeWasm::Renderer> renderer) : Manager(renderer){};

  std::string getName() override;
  folly::dynamic getConstants() override;
  ShadowNode *createShadow() override;

  static const char *Name;
};
} // namespace ReactNativeWasm::Components
