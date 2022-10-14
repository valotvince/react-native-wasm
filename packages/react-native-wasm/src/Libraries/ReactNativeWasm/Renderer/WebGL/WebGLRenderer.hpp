#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <mutex>

#include "../Renderer.hpp"

namespace ReactNativeWasm {
class WebGLRenderer : public Renderer {
public:
  WebGLRenderer();

  void render(const facebook::react::ShadowView &) override;
  void flush() override;
};
} // namespace ReactNativeWasm
