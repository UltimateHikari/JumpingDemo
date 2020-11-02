#include "base_game.h"

BaseGame::BaseGame() 
: window(std::make_unique<Window>(
        "Game", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 
        1600, 900, 
        WINDOW_FLAGS
    )
) {}
    
float BaseGame::getWidth() const {  
    return window->getWidth();
}

float BaseGame::getHeight() const { 
    return window->getHeight();
}

SDL_Renderer* BaseGame::getRenderer() const {
    return window->getRenderer();
}

bool BaseGame::isRunning() const {
    return running;
}

void BaseGame::quit() {
    running = false;
}

BaseGame::~BaseGame() {}