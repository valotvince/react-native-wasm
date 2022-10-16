#pragma once

#include "../Fabric/ComponentView.hpp"

namespace ReactNativeWasm {
class Renderer {
public:
  using Shared = std::shared_ptr<Renderer>;

  Renderer(){};

  virtual void render(ComponentView::Shared) = 0;
  virtual void flush() = 0;
};
} // namespace ReactNativeWasm
