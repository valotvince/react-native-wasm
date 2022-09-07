// #include "functions.h"
#include <iostream>
#include <SDL2/SDL.h>

#include "Libraries/ReactNativeWasm/reactNativeWasm.hpp"

int main(int argc, char* argv[]) {
    std::cout << "Hello World" << std::endl;
    // std::cout << "The time from Epoch is: " << helloworld::time() << std::endl;

    // Initialize SDL graphics subsystem.
    SDL_Init(SDL_INIT_VIDEO);

    // Initialize a 300x300 window and a renderer.
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(300, 300, 0, &window, &renderer);

    ReactNativeWasm::setRenderer(&renderer);

    // Set a color for drawing matching the earlier `ctx.fillStyle = "green"`.
    SDL_SetRenderDrawColor(renderer, /* RGBA: green */ 0x00, 0x80, 0x00, 0xFF);
    // Create and draw a rectangle like in the earlier `ctx.fillRect()`.
    SDL_Rect rect = {.x = 10, .y = 10, .w = 10, .h = 10};
    SDL_RenderFillRect(renderer, &rect);

    // Render everything from a buffer to the actual screen.
    SDL_RenderPresent(renderer);

    return 0;
}
