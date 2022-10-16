#pragma once

#include <react/renderer/mounting/ShadowView.h>

namespace ReactNativeWasm {
class Renderer {
public:
  Renderer(){};

  virtual void render(const facebook::react::ShadowView &) = 0;
  virtual void flush() = 0;
};
} // namespace ReactNativeWasm
