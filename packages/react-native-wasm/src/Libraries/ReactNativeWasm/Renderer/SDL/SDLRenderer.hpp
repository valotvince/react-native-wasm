#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <mutex>

#include "../Renderer.hpp"

#include <react/renderer/graphics/Color.h>

namespace ReactNativeWasm {

enum BorderDirection { LEFT, TOP, RIGHT, BOTTOM };

class SDLRenderer : public Renderer {
public:
  SDLRenderer();

  void render(ComponentView::Shared) override;
  void flush() override;

private:
  SDL_Renderer *renderer;
  SDL_Window *window;

  std::mutex renderMutex = std::mutex();

  void renderText(ComponentView::Shared);
  void renderView(ComponentView::Shared);

  void drawViewBorders(ComponentView::Shared);
  void drawViewBorder(const facebook::react::Rect &, BorderDirection, float, const facebook::react::SharedColor);

  void setDrawColor(const facebook::react::SharedColor);
};
} // namespace ReactNativeWasm
