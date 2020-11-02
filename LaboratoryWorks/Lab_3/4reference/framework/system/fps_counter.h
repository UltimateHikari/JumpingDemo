#pragma once
#include "SDL.h"

class FpsCounter {
    float seconds = 0;
    float accept_time = 0;

    float ticks = 0;

    float MAX_FPS;
    float REAL_FPS = 0;

    bool initialized = false;

    void initialize() {
        ticks = accept_time = SDL_GetTicks();
        initialized = true;
    }

    float update(float &ticks) {
        float dt = (SDL_GetTicks() - ticks) / 1000.0f;
        ticks = SDL_GetTicks();
        return dt;
    }

public:
    FpsCounter(float FPS) : MAX_FPS(FPS) {}

    bool accept() {
        if (!initialized) 
            initialize();

        const float step = 1.0f / MAX_FPS;
        
        seconds += update(ticks);
        if (seconds >= step) {
            seconds -= step;
            REAL_FPS = 1.0f / update(accept_time);
            return true;
        }
        return false;
    }

    float getFps() const {
        return REAL_FPS;
    }
};