#include <SDL2/SDL.h>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

namespace ReactNativeWasm::Renderer {
    void setRenderer(SDL_Renderer **r);

    bool render(float top, float left, float width, float height);
}

#endif
