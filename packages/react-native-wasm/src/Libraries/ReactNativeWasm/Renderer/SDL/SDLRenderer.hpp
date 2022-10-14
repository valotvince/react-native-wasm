#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <mutex>

#include "../Renderer.hpp"

namespace ReactNativeWasm {
class SDLRenderer : public Renderer {
public:
  SDLRenderer();

  void render(const facebook::react::ShadowView &) override;
  void flush() override;

private:
  SDL_Renderer *renderer;
  SDL_Window *window;

  std::mutex renderMutex = std::mutex();

  void renderText(const facebook::react::ShadowView &);
  void renderView(const facebook::react::ShadowView &);
};
} // namespace ReactNativeWasm
