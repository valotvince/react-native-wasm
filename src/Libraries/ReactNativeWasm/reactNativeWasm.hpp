#include <SDL2/SDL.h>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

namespace ReactNativeWasm {
    void setRenderer(SDL_Renderer **r);

    bool render(float width, float height);
}

#endif
