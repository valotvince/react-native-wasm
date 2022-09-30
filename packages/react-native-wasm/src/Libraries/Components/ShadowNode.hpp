#pragma once

#include <folly/dynamic.h>
// #include "ComponentManager.hpp"
#include "ComponentView.hpp"

namespace ReactNativeWasm::Components {
class ShadowNode {
public:
  virtual void createView(folly::dynamic props) = 0;

  std::shared_ptr<ComponentView> getView() { return view; };

  std::string className;
  int64_t tag;
  int64_t rootTag;
  // std::shared_ptr<ReactNativeWasm::Components::Manager> viewManager;

protected:
  std::shared_ptr<ReactNativeWasm::Components::ComponentView> view;
};
} // namespace ReactNativeWasm::Components
