#include "texture_manager.h"
#include <iostream>

TextureManager::TextureManager() {}

TextureManager::~TextureManager() {}

TextureManager* TextureManager::getInstance() {
    if (!p_instance) 
        p_instance = new TextureManager;
    return p_instance;
}

void TextureManager::shutdown() {
    delete p_instance;
    p_instance = nullptr;
}

void TextureManager::load(SDL_Renderer *renderer, const std::string &id, 
const std::string &path, int frame_width, int frame_height) {
    filesystem::path file_path = filesystem::canonical(root / path);
    SDL_Texture *ptr = IMG_LoadTexture(renderer, file_path.string().c_str());

    if (!ptr) 
        throw std::exception("Couldn't load texture");
    textures[id] = {ptr, frame_width, frame_height};
    removable.emplace_back(ptr, TextureDeleter{});
}

void TextureManager::setDefaultPath(const std::string &root_) {
    root = filesystem::canonical(filesystem::absolute(root_));
}

Texture TextureManager::get(const std::string &name) const {
    auto it = textures.find(name);
    if (it == textures.end()) 
        throw std::exception("Texture not found");
    return it->second;
}

void TextureManager::clear() {
    textures.clear();
    removable.clear();
}

TextureManager* TextureManager::p_instance = nullptr;