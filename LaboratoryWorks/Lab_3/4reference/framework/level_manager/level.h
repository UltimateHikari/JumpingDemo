#pragma once 
#include "../input_handler/input_handler.h"
#include "../world/world.h"
#include <memory>

class Level : public InputHandler {
    bool running = true;
    std::unique_ptr<World> world;
public:
    Level() : world(new World) {}
    
    bool isRunning() const {
        return running;
    }

    virtual void preload(SDL_Renderer *renderer) {
        /* here you can load sounds, graphics */
    }

    virtual void clear() {
        /* here you can free unnecessary files */
    }

    virtual void update(float dt) {
        world->update(dt);
    }

    virtual void draw(SDL_Renderer *renderer) {
        world->draw(renderer);
    }

    virtual void quit() {
        running = false;
    }

    World* getWorld() const {
        return world.get();
    }

    virtual ~Level() {}
};