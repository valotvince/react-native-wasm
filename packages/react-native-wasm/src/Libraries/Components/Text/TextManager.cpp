#include "TextManager.hpp"

#include <iostream>

namespace ReactNativeWasm::Components {
void TextShadowNode::createView(folly::dynamic props) {}

const char *TextManager::Name = "RCTText";

std::string TextManager::getName() { return TextManager::Name; }

folly::dynamic TextManager::getConstants() {
  folly::dynamic constants = folly::dynamic::object();

  folly::dynamic nativeProps = folly::dynamic::object();

  constants.insert("NativeProps", std::move(nativeProps));

  return constants;
}

ShadowNode *TextManager::createShadow() {
  auto node = new TextShadowNode();

  nodes->push_back(node);

  return node;
}
} // namespace ReactNativeWasm::Components
