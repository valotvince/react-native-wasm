#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <mutex>
#include <thread>

#include "../Renderer.hpp"

#include <react/renderer/graphics/Color.h>

namespace ReactNativeWasm {

enum BorderDirection { LEFT, TOP, RIGHT, BOTTOM };

class SDLRenderer : public Renderer {
public:
  SDLRenderer(std::shared_ptr<facebook::react::Instance>);
  ~SDLRenderer();

  void render(ComponentView::Shared) override;
  void flush() override;

private:
  SDL_Renderer *renderer;
  SDL_Window *window;

  std::thread thread;
  std::mutex renderMutex;

  void loop();

  void renderText(ComponentView::Shared);
  void renderView(ComponentView::Shared);

  void drawViewBorders(ComponentView::Shared);
  void drawViewBorder(const facebook::react::Rect &, BorderDirection, float, const facebook::react::SharedColor);

  void setDrawColor(const facebook::react::SharedColor);
};
} // namespace ReactNativeWasm
