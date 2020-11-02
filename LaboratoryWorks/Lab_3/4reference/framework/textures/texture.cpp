#include "texture.h"
#include "SDL_image.h"

Texture::Texture() {}

Texture::Texture(SDL_Texture* texture_, int w_, int h_)
: texture(texture_), w(w_), h(h_) {}

Texture::Texture(const Texture &other) 
: texture(other.texture), w(other.w), h(other.h) {}

Texture::operator SDL_Texture*() const {
    return texture;
}
  
int Texture::getWidth() const {
    return w;
}

int Texture::getHeight() const {
    return h;
}
