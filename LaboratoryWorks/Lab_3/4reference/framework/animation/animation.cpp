#include "animation.h"

constexpr int EXPIRED = -1;

Animation::Animation(float framerate_, int frames_count_, bool repeat_)
: framerate(framerate_), frames_count(frames_count_), repeat(repeat_) {} 

void Animation::update(float dt) {
    if (frames_count <= 0) {
        current_frame = EXPIRED;
    }
	seconds += dt;
    const float step = 1.0f / framerate;
    while (seconds - step > 0) {
        seconds -= step;
        ++current_frame;
        if (current_frame >= frames_count) {
            current_frame = (repeat) ? 0 : EXPIRED;
        }
    }
}

int Animation::getCurrentFrame() const {
    return current_frame;
}

int Animation::getFramesCount() const {
    return frames_count;
}

void Animation::setFramesCount(int count) {
    frames_count = count;
    if (current_frame >= frames_count)
        current_frame = 0;
}

int Animation::nextFrame() {
    if (frames_count <= 0)
        return EXPIRED; 
    ++current_frame;
    if (current_frame >= frames_count)
        return EXPIRED;
    return current_frame;
}


SpriteManager::SpriteManager() {}

SpriteManager::~SpriteManager() {}

SpriteManager* SpriteManager::getInstance() {
    if (!instance)
        instance = new SpriteManager();
    return instance;
}

void SpriteManager::shutdown() {
    delete instance;
}

void SpriteManager::draw(SDL_Renderer *renderer) {
    auto it = sprites.begin();
    while (it != sprites.end()) {
        (*it)->draw(renderer);
        ++it;
    }
}

void SpriteManager::update(float dt) {
    for (auto it = sprites.begin(); it != sprites.end();) {
        auto &ref = *it;
		ref->update(dt);
		if (ref->getCurrentFrame() < 0) 
			it = sprites.erase(it);
		else
			++it;
	}
}

SpriteManager* SpriteManager::instance = nullptr;

