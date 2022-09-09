#include <emscripten/bind.h>
#include <vector>
#include <SDL2/SDL.h>

#include "reactNativeWasm.hpp"
#include "../Component/component.hpp"

namespace ReactNativeWasm {
    SDL_Renderer *renderer;
    std::vector<Component> components = {};

    void setRenderer(SDL_Renderer **r) {
        renderer = *r;
    }

    bool render(float width, float height) {
        Component component = Component("Toto", width, height);

        components.push_back(component);


        // Set a color for drawing matching the earlier `ctx.fillStyle = "green"`.
        SDL_SetRenderDrawColor(renderer, /* RGBA: green */ 0x80, 0x00, 0x00, 0xFF);
        // Create and draw a rectangle like in the earlier `ctx.fillRect()`.
        SDL_FRect rect = {.x = 50, .y = 50, .w = width, .h = height};
        SDL_RenderFillRectF(renderer, &rect);

        SDL_RenderPresent(renderer);

        return true;
    }
}

/*
 * This way the function may be called from the JavaScript code.
 * (see the updated index.html file of the parent project,
 * starting from the tag: "WASM MODULE START: HelloWorld")
 */
EMSCRIPTEN_BINDINGS(ReactNativeWasm) {
    emscripten::function("ReactNativeWasm_render", ReactNativeWasm::render);
}
