#pragma once
#include "gl_includes.hpp"

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <bitset>

#include "rmanager.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "controls.hpp"
#include "window.hpp"
#include "light.hpp"
#include "physics.hpp"
#include "entity.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>

class World{
private:
    std::vector<std::shared_ptr<Entity> > entities;
public:
    World() = default;
    void update(float deltaTime);
    void prerender();
    void render();
    void addEntity(std::shared_ptr<Entity> entity);
    std::shared_ptr<Entity> getPlayerEntity(GLuint index);
};

class Game{
private:
    std::vector<std::unique_ptr<CameraEntity>> cameras;
    std::vector<std::unique_ptr<ControllerInterface> > controllers;
    World world;
    Window& window;
    GLuint current_camera_index;
public:
    Game(Window& window_);
    void use_events();
    void update();
    void render();
};