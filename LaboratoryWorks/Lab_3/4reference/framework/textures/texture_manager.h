#pragma once
#include <string>
#include <memory>
#include <map>
#include <filesystem>
#include <vector>
#include "texture.h"

namespace filesystem = std::filesystem;

class TextureDeleter {
public:
    void operator()(SDL_Texture *ptr) const {
        SDL_DestroyTexture(ptr);
    }
};


class TextureManager {
    static TextureManager *p_instance;
    std::vector<std::unique_ptr<SDL_Texture, TextureDeleter>> removable;
    std::map<std::string, Texture> textures;
    TextureManager();
    ~TextureManager();

    filesystem::path root;
public:
    static TextureManager* getInstance();
    static void shutdown();
    
    void load(SDL_Renderer *renderer, const std::string &id, 
        const std::string &path, int frame_width, int frame_height);
    void setDefaultPath(const std::string &root);
    Texture get(const std::string &name) const;
    void clear();
};