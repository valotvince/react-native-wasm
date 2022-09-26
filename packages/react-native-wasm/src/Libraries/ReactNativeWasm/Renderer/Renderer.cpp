#include <emscripten/bind.h>
#include <vector>
#include <SDL2/SDL.h>

namespace ReactNativeWasm::Renderer {
    SDL_Renderer *renderer;
    // std::vector<Component> components = {};

    void setRenderer(SDL_Renderer **r) {
        renderer = *r;
    }

    bool render(float top, float left, float width, float height) {
        // Component component = Component("Toto", width, height);

        // components.push_back(component);


        // Set a color for drawing matching the earlier `ctx.fillStyle = "green"`.
        SDL_SetRenderDrawColor(renderer, /* RGBA: green */ 0x80, 0x00, 0x00, 0xFF);
        // Create and draw a rectangle like in the earlier `ctx.fillRect()`.
        SDL_FRect rect = {.x = 50, .y = 50, .w = width, .h = height};
        SDL_RenderFillRectF(renderer, &rect);

        SDL_RenderPresent(renderer);

        return true;
    }
}
