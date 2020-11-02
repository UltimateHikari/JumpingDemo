#include "window.h"
#include <exception>

void WindowDeleter::operator() (SDL_Window *ptr) const {
    SDL_DestroyWindow(ptr);
}

void RendererDeleter::operator() (SDL_Renderer *ptr) const {
    SDL_DestroyRenderer(ptr);
}

Window::Window(const std::string &title, int x, int y, int width, int height, Uint32 flags) {
    SDL_Window *window_ptr = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);
    if (nullptr == window_ptr) 
        throw std::exception("Couldn't create window");
    
    SDL_Renderer *renderer_ptr = SDL_CreateRenderer(window_ptr, -1, SDL_RENDERER_ACCELERATED);
    if (nullptr == renderer_ptr) 
         throw std::exception("Couldn't create renderer");
    window.reset(window_ptr);
    renderer.reset(renderer_ptr);
}

int Window::getWidth() const {
        int w, h;
        SDL_GetRendererOutputSize(renderer.get(), &w, &h);
        return w;
}

int Window::getHeight() const {
        int w, h;
        SDL_GetRendererOutputSize(renderer.get(), &w, &h);
        return h;
}

SDL_Renderer* Window::getRenderer() const {
    return renderer.get();
}

Window::~Window() {}
