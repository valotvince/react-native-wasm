#pragma once

#include <folly/dynamic.h>

namespace ReactNativeWasm::Components {
class ComponentView {
public:
  ComponentView(folly::dynamic props) : props(props){};

  folly::dynamic props;
};
} // namespace ReactNativeWasm::Components
