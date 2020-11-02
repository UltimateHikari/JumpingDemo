#pragma once 
#include <memory>
#include <vector>
#include "../entity/entity.h"

class Animation : public GraphicObject {
    int frames_count = 1;
    int current_frame = 0;
    int frame_inc = 1;
 
    float framerate;
    float seconds = 0.0f;

    bool repeat;
public:
	Animation(float framerate_, int frames_count_, bool repeat_ = false);
	void update(float dt) override;

    int getCurrentFrame() const;    
    int getFramesCount() const;

    void setFramesCount(int count);
    int nextFrame();
};

class SpriteManager : public GraphicObject {
    std::vector<std::unique_ptr<Animation>> sprites;
    static SpriteManager *instance;
    SpriteManager();
    ~SpriteManager();
public:
    static SpriteManager* getInstance();
    static void shutdown();

    template<class Animation_type, class... Args>
    void addSprite(Args&&... args);

    void draw(SDL_Renderer *renderer) override;
    void update(float dt) override;
};

template<class Animation_type, class... Args>
void SpriteManager::addSprite(Args&&... args) {
    auto sprite = std::make_unique<Animation_type>(
        std::forward<Args>(args)...
    );

    sprites.push_back(std::move(sprite));
}