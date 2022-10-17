#include <emscripten/threading.h>
#include <iostream>

#include "SDLRenderer.hpp"
#include <react/renderer/components/text/ParagraphShadowNode.h>

SDL_Texture *MAIN_SDL_CreateTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *surface) {
  if (emscripten_is_main_runtime_thread()) {
    return SDL_CreateTextureFromSurface(renderer, surface);
  }

  return (SDL_Texture *)emscripten_sync_run_in_main_runtime_thread(
    EM_FUNC_SIG_III, SDL_CreateTextureFromSurface, (uint32_t)renderer, (uint32_t)surface);
}

void MAIN_SDL_DestroyTexture(SDL_Texture *texture) {
  if (emscripten_is_main_runtime_thread()) {
    SDL_DestroyTexture(texture);

    return;
  }

  emscripten_sync_run_in_main_runtime_thread(EM_FUNC_SIG_VI, SDL_DestroyTexture, (uint32_t)texture);
}

void MAIN_SDL_RenderPresent(SDL_Renderer *renderer) {
  if (emscripten_is_main_runtime_thread()) {
    SDL_RenderPresent(renderer);

    return;
  }

  emscripten_sync_run_in_main_runtime_thread(EM_FUNC_SIG_VI, SDL_RenderPresent, (uint32_t)renderer);
}

namespace ReactNativeWasm {
SDLRenderer::SDLRenderer(std::shared_ptr<facebook::react::Instance> instance) : Renderer(instance) {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  // Initialize a 300x300 window and a renderer.
  SDL_CreateWindowAndRenderer(300, 300, 0, &window, &renderer);

  thread = std::thread(&SDLRenderer::loop, std::ref(*this));
}

SDLRenderer::~SDLRenderer() { pthread_cancel(thread.native_handle()); }

void SDLRenderer::loop() {
  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        std::cout << "Key quit" << std::endl;
        break;
      case SDL_KEYDOWN: {
        folly::dynamic params = folly::dynamic::array(
          "keydown", folly::dynamic::object("key", std::string(SDL_GetKeyName(event.key.keysym.sym))));

        reactInstance->callJSFunction("RCTDeviceEventEmitter", "emit", std::move(params));
        break;
      }

      case SDL_KEYUP:
        folly::dynamic params = folly::dynamic::array(
          "keyup", folly::dynamic::object("key", std::string(SDL_GetKeyName(event.key.keysym.sym))));

        reactInstance->callJSFunction("RCTDeviceEventEmitter", "emit", std::move(params));
        break;
      }
    }
  }
}

void SDLRenderer::flush() {
  std::lock_guard<std::mutex> guard(renderMutex);

  // SDL_RenderClear(renderer);
  MAIN_SDL_RenderPresent(renderer);
}

void SDLRenderer::renderText(ComponentView::Shared view) {
  std::lock_guard<std::mutex> guard(renderMutex);

  auto contentFrame = view->layoutMetrics.frame;

  const auto &newState =
    *std::static_pointer_cast<facebook::react::ParagraphShadowNode::ConcreteState const>(view->state);

  float previousX = contentFrame.origin.x;

  for (const auto &fragment : newState.getData().attributedString.getFragments()) {
    auto textAttributes = fragment.textAttributes;

    auto fontSize = textAttributes.fontSize;
    facebook::react::ColorComponents foregroundColor;

    if (textAttributes.foregroundColor) {
      foregroundColor = facebook::react::colorComponentsFromColor(textAttributes.foregroundColor);
    } else {
      foregroundColor = facebook::react::colorComponentsFromColor(facebook::react::blackColor());
    }

    auto text = fragment.string;

    // this opens a font style and sets a size
    TTF_Font *Sans = TTF_OpenFont("Resources/OpenSans-Regular.ttf", fontSize);

    // // this is the color in rgb format,
    // // maxing out all would give you the color white,
    // // and it will be your text's color
    SDL_Color textColor = {
      static_cast<Uint8>(foregroundColor.red * 255), static_cast<Uint8>(foregroundColor.green * 255),
      static_cast<Uint8>(foregroundColor.blue * 255)};

    // // as TTF_RenderText_Solid could only be used on
    // // SDL_Surface then you have to create the surface first
    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Sans, text.c_str(), textColor);

    // // now you can convert it into a texture
    SDL_Texture *Message = MAIN_SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    auto textWidth = fontSize * text.size();

    SDL_FRect Message_rect;                    // create a rect
    Message_rect.x = previousX;                // controls the rect's x coordinate
    Message_rect.y = contentFrame.origin.y;    // controls the rect's y coordinte
    Message_rect.w = textWidth;                // controls the width of the rect
    Message_rect.h = contentFrame.size.height; // controls the height of the rect

    // Add one separator character
    previousX += textWidth + fontSize;

    // (0,0) is on the top left of the window/screen,
    // think a rect as the text's box,
    // that way it would be very simple to understand

    // Now since it's a texture, you have to put RenderCopy
    // in your game loop area, the area where the whole code executes

    // you put the renderer's name first, the Message,
    // the crop size (you can ignore this if you don't want
    // to dabble with cropping), and the rect which is the size
    // and coordinate of your texture
    SDL_RenderCopyF(renderer, Message, NULL, &Message_rect);

    // // Don't forget to free your surface and texture
    SDL_FreeSurface(surfaceMessage);
    MAIN_SDL_DestroyTexture(Message);
  }
}

void SDLRenderer::setDrawColor(const facebook::react::SharedColor color) {
  facebook::react::ColorComponents drawColor;

  if (color) {
    drawColor = facebook::react::colorComponentsFromColor(color);
  } else {
    drawColor = facebook::react::colorComponentsFromColor(facebook::react::clearColor());
  }

  SDL_SetRenderDrawColor(
    renderer, static_cast<Uint8>(drawColor.red * 255), static_cast<Uint8>(drawColor.green * 255),
    static_cast<Uint8>(drawColor.blue * 255), static_cast<Uint8>(drawColor.alpha * 255));
}

void SDLRenderer::drawViewBorder(
  const facebook::react::Rect &contentFrame, BorderDirection direction, float borderWidth,
  const facebook::react::SharedColor borderColor) {
  if (!borderWidth || !borderColor) {
    return;
  }

  SDL_FRect rect;

  switch (direction) {
  case BorderDirection::LEFT:
    rect = SDL_FRect{
      .x = contentFrame.origin.x, .y = contentFrame.origin.y, .w = borderWidth, .h = contentFrame.size.height};
    break;
  case BorderDirection::TOP:
    rect =
      SDL_FRect{.x = contentFrame.origin.x, .y = contentFrame.origin.y, .w = contentFrame.size.width, .h = borderWidth};
    break;
  case BorderDirection::RIGHT:
    rect = SDL_FRect{
      .x = contentFrame.origin.x + contentFrame.size.width - borderWidth,
      .y = contentFrame.origin.y,
      .w = borderWidth,
      .h = contentFrame.size.height};
    break;
  case BorderDirection::BOTTOM:
    rect = SDL_FRect{
      .x = contentFrame.origin.x,
      .y = contentFrame.origin.y + contentFrame.size.height - borderWidth,
      .w = contentFrame.size.width,
      .h = borderWidth};
    break;

  default:
    break;
  }

  setDrawColor(borderColor);
  SDL_RenderFillRectF(renderer, &rect);
}

void SDLRenderer::drawViewBorders(ComponentView::Shared view) {
  const auto &props = *std::static_pointer_cast<const facebook::react::ViewProps>(view->props);
  const auto contentFrame = view->layoutMetrics.frame;
  const auto borderMetrics = props.resolveBorderMetrics(view->layoutMetrics);

  drawViewBorder(contentFrame, BorderDirection::LEFT, borderMetrics.borderWidths.left, borderMetrics.borderColors.left);
  drawViewBorder(
    contentFrame, BorderDirection::TOP, borderMetrics.borderWidths.right, borderMetrics.borderColors.right);
  drawViewBorder(contentFrame, BorderDirection::RIGHT, borderMetrics.borderWidths.top, borderMetrics.borderColors.top);
  drawViewBorder(
    contentFrame, BorderDirection::BOTTOM, borderMetrics.borderWidths.bottom, borderMetrics.borderColors.bottom);
}

void SDLRenderer::renderView(ComponentView::Shared view) {
  std::lock_guard<std::mutex> guard(renderMutex);

  const auto &props = *std::static_pointer_cast<const facebook::react::ViewProps>(view->props);

  if (props.backgroundColor) {
    auto contentFrame = view->layoutMetrics.frame;

    SDL_FRect rect = {
      .x = contentFrame.origin.x,
      .y = contentFrame.origin.y,
      .w = contentFrame.size.width,
      .h = contentFrame.size.height};

    setDrawColor(props.backgroundColor);
    SDL_RenderFillRectF(renderer, &rect);
  }

  drawViewBorders(view);
}

void SDLRenderer::render(ComponentView::Shared componentView) {
  if (componentView->name == "Paragraph") {
    renderText(componentView);
  } else {
    renderView(componentView);

    for (auto it = componentView->children.begin(); it != componentView->children.end(); ++it) {
      render(it->second);
    }
  }
}
} // namespace ReactNativeWasm
