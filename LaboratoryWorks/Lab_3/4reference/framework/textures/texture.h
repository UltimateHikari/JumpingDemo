#pragma once
#include <memory>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

class Texture {
public:
    Texture();
    Texture(SDL_Texture* texture_, int w_, int h_);
    Texture(const Texture &other);

    operator SDL_Texture*() const;
    int getWidth() const;
    int getHeight() const;

private:
    mutable SDL_Texture *texture = nullptr;
    int w = 0;
    int h = 0;
};