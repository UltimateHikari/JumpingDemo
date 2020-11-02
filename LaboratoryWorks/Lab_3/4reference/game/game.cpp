#include "game.h"

Game::Game() : BaseGame() {}

void Game::preload() {}

void Game::render() {
    SDL_Renderer *renderer = getRenderer();
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        onEvents(event);
    }
}

void Game::update(float dt) {
    static constexpr float freq  = 120.0f;
    static float time_accumulator = 0.0f;

    float ifreq = 1.0f / freq;    
    time_accumulator += dt;

    while (time_accumulator > ifreq) {
        time_accumulator -= ifreq;
        //object.update(ifreq);
    }    
}

void Game::onQuit() {
    quit();
}

Game::~Game() {}