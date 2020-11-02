#include "world.h"

static auto isAlive = [](const auto &it) -> bool { return (*it)->isAlive(); };

template <class E>
constexpr auto toInt(E enumerator) {
    return static_cast<std::underlying_type_t<E>>(enumerator);
}


World::World() {}

World::~World() {}

void World::update(float dt) {
    constexpr auto DYNAMIC = toInt(TYPE::DYNAMIC);
    constexpr auto STATIC = toInt(TYPE::STATIC);

    //update all entitites
    for (int i = 0; i < 2; ++i) {
        auto iter = entities[i].begin();
        while (iter != entities[i].end()) {
            if (isAlive(iter))
                (*iter)->update(dt);
            ++iter;
        }
    }
    auto iter = entities[DYNAMIC].begin();
    while (iter != entities[DYNAMIC].end()) {
        if (isAlive(iter)) {
            //check collide between dynamic objects
            auto second_iter = iter;
            while (++second_iter != entities[DYNAMIC].end()) {
                if (isAlive(second_iter)) {
                    (*iter)->getPhysics()->
                        checkCollide((*second_iter)->getPhysics());
                    if (!isAlive(iter))
                        break;
                }
            }
        }
        if (isAlive(iter)) {
            //check collide between dynamic and static objects
            auto second_iter = entities[STATIC].begin();
            while (second_iter != entities[STATIC].end()) {
                if (isAlive(second_iter)) {
                    (*iter)->getPhysics()->
                        checkCollide((*second_iter)->getPhysics());
                    if (!isAlive(iter))
                        break;
                }
                ++second_iter;
            }
        }
        ++iter;
    }
}

void World::draw(SDL_Renderer *renderer) {
    for (int i = 0; i < 2; ++i) {
        auto it = entities[i].begin();
        while (it != entities[i].end()) {
            if (isAlive(it)) {
                GraphicObject *graphics = (*it)->getGraphics();
                if (nullptr != graphics)
                    graphics->draw(renderer);
            }
            ++it;
        }
    }
}

void World::addEntity(EntityPtr&& entity) {
    TYPE type = entity->getPhysics()->getType();
    entities[toInt(type)].push_back(std::move(entity));
}

