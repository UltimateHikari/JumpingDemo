#pragma once
#include <vector>
#include <memory>
#include "../entity/entity.h"

class World {
    using EntityPtr = std::unique_ptr<Entity>;
    std::vector<EntityPtr> entities[2];
public:
    World();
    ~World();

    void update(float dt);
    void draw(SDL_Renderer *renderer);
    void addEntity(EntityPtr&& entity);
};