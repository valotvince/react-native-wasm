#include "ViewManager.hpp"
#include "../../ReactNativeWasm/Renderer/Renderer.hpp"

#include <iostream>

namespace ReactNativeWasm::Components {
class ViewShadowNode : public ShadowNode {
public:
  ViewShadowNode(){};
  void createView(folly::dynamic props) override;
};

void ViewShadowNode::createView(folly::dynamic props) {}

const char *ViewManager::Name = "RCTView";

std::string ViewManager::getName() { return ViewManager::Name; }

folly::dynamic ViewManager::getConstants() {
  folly::dynamic constants = folly::dynamic::object();

  folly::dynamic nativeProps = folly::dynamic::object();

  constants.insert("NativeProps", std::move(nativeProps));

  return constants;
}

ShadowNode *ViewManager::createShadow() {
  auto node = new ViewShadowNode();

  return node;
}
} // namespace ReactNativeWasm::Components
