#pragma once
#include <SDL.h>
#include <memory>
#include <type_traits>

class World;

class TankPhysics;
class BulletPhysics;
class BrickPhysics;

enum class TYPE : char {
    STATIC = 0, DYNAMIC
};


class Object {
public:
    virtual void update(float dt) = 0;
    virtual ~Object() {}
};


class GraphicObject : public Object {
public:
    virtual void draw(SDL_Renderer *renderer) = 0;
};


class PhysicalObject : public Object {
public:
    virtual TYPE getType() const = 0;

    virtual void checkCollide(PhysicalObject *other) = 0;
};


class SoundObject : public Object {
};


class Entity : public Object {
    World *world;
public:
    Entity(World *w) : world(w) {}

    World* getWorld() const { return world; }

    virtual bool isAlive() const = 0;
    
    virtual GraphicObject   *getGraphics()  const { return nullptr; }
    virtual PhysicalObject  *getPhysics()   const { return nullptr; }
    virtual SoundObject     *getSounds()    const { return nullptr; }
};

template <typename Entity_Type, typename... Args> 
auto createEntity(Args&&... args) {
    static_assert(
        std::is_base_of<Entity, 
            std::decay_t<Entity_Type>
        >::value
    );
    
    return std::make_unique<Entity_Type>(
        std::forward<Args>(args)...
    );
}