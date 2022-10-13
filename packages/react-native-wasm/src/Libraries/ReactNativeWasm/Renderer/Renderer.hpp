#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <mutex>

#include <react/renderer/mounting/ShadowView.h>

namespace ReactNativeWasm {
class Renderer {
public:
  Renderer();

  void render(const facebook::react::ShadowView &);
  void flush();

private:
  SDL_Renderer *renderer;
  SDL_Window *window;

  std::mutex renderMutex = std::mutex();

  void renderText(const facebook::react::ShadowView &);
  void renderView(const facebook::react::ShadowView &);
};
} // namespace ReactNativeWasm
