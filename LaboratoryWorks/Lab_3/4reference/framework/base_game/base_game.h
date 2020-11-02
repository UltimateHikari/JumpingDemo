#pragma once
#include <memory>
#include "../window/window.h"
#include "../config/configuration.h"
#include "../input_handler/input_handler.h"

class BaseGame : public InputHandler {
public:
    BaseGame();
    float getWidth() const;     
    float getHeight() const; 
    SDL_Renderer* getRenderer() const;
    bool isRunning() const;
    void quit();
    virtual void render() = 0;
    virtual void update(float dt) = 0;
    virtual void handleEvents() = 0;
    virtual ~BaseGame();
private:
    bool running = true;
    std::unique_ptr<Window> window;
};