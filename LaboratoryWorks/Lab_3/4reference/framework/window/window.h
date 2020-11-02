#pragma once
#include <string>
#include <memory>
#include "SDL.h"

class WindowDeleter {
public:
    void operator() (SDL_Window *ptr) const;
};

class RendererDeleter {
public:
    void operator() (SDL_Renderer *ptr) const;
};

class Window {
public:
    Window(
        const std::string &title, 
        int x,
        int y,
        int width, 
        int height, 
        Uint32 flags = 0
    );
    int getWidth() const;   //in pixels!
    int getHeight() const;  //in pixels!
    SDL_Renderer* getRenderer() const;
    ~Window();
private:
    int init();
    std::unique_ptr<SDL_Window, WindowDeleter> window;
    std::unique_ptr<SDL_Renderer, RendererDeleter> renderer;
};