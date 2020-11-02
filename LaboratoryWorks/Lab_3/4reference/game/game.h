#pragma once
#include "../framework/framework.h"

class Game : public BaseGame {
public:
    Game();
    void preload();
    void render() override;
    void handleEvents() override;
    void update(float dt) override;

    //eventHandlers
    void onQuit();
    ~Game();
};