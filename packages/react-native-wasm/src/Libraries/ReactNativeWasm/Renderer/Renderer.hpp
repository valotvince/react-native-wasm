#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <mutex>

#include "../../Components/ShadowNode.hpp"

namespace ReactNativeWasm {
class Renderer {
public:
  Renderer();

  void render(ReactNativeWasm::Components::ShadowNode *);
  void flush();

private:
  SDL_Renderer *renderer;
  SDL_Window *window;

  std::mutex renderMutex = std::mutex();

  void renderText(ReactNativeWasm::Components::ShadowNode *);
  void renderView(ReactNativeWasm::Components::ShadowNode *);
};
} // namespace ReactNativeWasm
