#include <emscripten/threading.h>
#include <iostream>

#include "Renderer.hpp"
#include <folly/json.h>
#include <react/renderer/components/text/ParagraphShadowNode.h>
#include <react/renderer/graphics/Color.h>
#include <thread>

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
Renderer::Renderer() {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  // Initialize a 300x300 window and a renderer.
  SDL_CreateWindowAndRenderer(300, 300, 0, &window, &renderer);
}

void Renderer::flush() {
  std::lock_guard<std::mutex> guard(renderMutex);

  // SDL_RenderClear(renderer);
  MAIN_SDL_RenderPresent(renderer);

  std::cout << "FLUSH" << std::endl;
}

void Renderer::renderText(const facebook::react::ShadowView &view) {
  std::lock_guard<std::mutex> guard(renderMutex);

  auto contentFrame = view.layoutMetrics.frame;

  const auto &newState =
    *std::static_pointer_cast<facebook::react::ParagraphShadowNode::ConcreteState const>(view.state);

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

void Renderer::renderView(const facebook::react::ShadowView &view) {
  std::lock_guard<std::mutex> guard(renderMutex);

  const auto &newViewProps = *std::static_pointer_cast<const facebook::react::ViewProps>(view.props);

  facebook::react::ColorComponents backgroundColor;

  if (newViewProps.backgroundColor) {
    backgroundColor = facebook::react::colorComponentsFromColor(newViewProps.backgroundColor);
  } else {
    backgroundColor = facebook::react::colorComponentsFromColor(facebook::react::clearColor());
  }

  // Set a color for drawing matching the earlier `ctx.fillStyle = "green"`.
  SDL_SetRenderDrawColor(
    renderer, static_cast<Uint8>(backgroundColor.red * 255), static_cast<Uint8>(backgroundColor.green * 255),
    static_cast<Uint8>(backgroundColor.blue * 255), static_cast<Uint8>(backgroundColor.alpha * 255));

  auto contentFrame = view.layoutMetrics.frame;

  // Create and draw a rectangle like in the earlier `ctx.fillRect()`.
  SDL_FRect rect = {
    .x = contentFrame.origin.x,
    .y = contentFrame.origin.y,
    .w = contentFrame.size.width,
    .h = contentFrame.size.height};
  SDL_RenderFillRectF(renderer, &rect);
}

void Renderer::render(const facebook::react::ShadowView &view) {
  std::string componentName(view.componentName);

  if (componentName == "Paragraph") {
    renderText(view);
  } else {
    renderView(view);
  }
}
} // namespace ReactNativeWasm
