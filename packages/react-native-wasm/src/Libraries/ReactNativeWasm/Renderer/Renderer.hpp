#pragma once

#include "../Fabric/ComponentView.hpp"
#include <cxxreact/Instance.h>

namespace ReactNativeWasm {
class Renderer {
public:
  using Shared = std::shared_ptr<Renderer>;

  Renderer(std::shared_ptr<facebook::react::Instance> instance) : reactInstance(instance){};

  virtual void render(ComponentView::Shared) = 0;
  virtual void flush() = 0;

protected:
  std::shared_ptr<facebook::react::Instance> reactInstance;
};
} // namespace ReactNativeWasm
