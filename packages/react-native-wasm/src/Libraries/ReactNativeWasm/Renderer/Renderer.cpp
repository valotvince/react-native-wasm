#include <emscripten/threading.h>
#include <iostream>

#include "Renderer.hpp"
#include <folly/json.h>
#include <thread>

SDL_Texture *MAIN_SDL_CreateTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *surface) {
  return (SDL_Texture *)emscripten_sync_run_in_main_runtime_thread(
    EM_FUNC_SIG_III, SDL_CreateTextureFromSurface, (uint32_t)renderer, (uint32_t)surface);
}

void MAIN_SDL_DestroyTexture(SDL_Texture *texture) {
  emscripten_sync_run_in_main_runtime_thread(EM_FUNC_SIG_VI, SDL_DestroyTexture, (uint32_t)texture);
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
  SDL_RenderPresent(renderer);

  std::cout << "FLUSH" << std::endl;
}

void Renderer::renderText(ReactNativeWasm::Components::ShadowNode *textNode) {
  std::lock_guard<std::mutex> guard(renderMutex);

  auto view = textNode->getView();

  std::cout << "textNode Props " << folly::toJson(view->props) << std::endl;

  int fontSize = 24;

  // this opens a font style and sets a size
  TTF_Font *Sans = TTF_OpenFont("Resources/OpenSans-Regular.ttf", fontSize);

  // // this is the color in rgb format,
  // // maxing out all would give you the color white,
  // // and it will be your text's color
  SDL_Color White = {255, 255, 255};

  auto text = view->props.at("text");

  // // as TTF_RenderText_Solid could only be used on
  // // SDL_Surface then you have to create the surface first
  SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Sans, text.c_str(), White);

  // // now you can convert it into a texture
  SDL_Texture *Message = MAIN_SDL_CreateTextureFromSurface(renderer, surfaceMessage);

  SDL_FRect Message_rect;                  // create a rect
  Message_rect.x = 0;                      // controls the rect's x coordinate
  Message_rect.y = 0;                      // controls the rect's y coordinte
  Message_rect.w = fontSize * text.size(); // controls the width of the rect
  Message_rect.h = fontSize + 2;           // controls the height of the rect

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

void Renderer::renderView(ReactNativeWasm::Components::ShadowNode *node) {
  std::lock_guard<std::mutex> guard(renderMutex);

  // Set a color for drawing matching the earlier `ctx.fillStyle = "green"`.
  SDL_SetRenderDrawColor(renderer, /* RGBA: green */ 0x00, 0x80, 0x80, 0xFF);

  // Create and draw a rectangle like in the earlier `ctx.fillRect()`.
  SDL_Rect rect = {.x = 10, .y = 10, .w = 10, .h = 10};
  SDL_RenderFillRect(renderer, &rect);
}

void Renderer::render(ReactNativeWasm::Components::ShadowNode *node) {
  if (node->className == "RCTRawText") {
    renderText(node);
  } else {
    renderView(node);
  }
}
} // namespace ReactNativeWasm
