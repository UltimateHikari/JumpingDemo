#include "level_manager.h"

static bool returnFalse(...) {
    return false;
}

LevelChangeStrategy::LevelChangeStrategy()
: change_on_update(returnFalse), change_on_event(returnFalse) {}

bool LevelChangeStrategy::isReadyToChange(float dt) const {
    return change_on_update(dt);
}  

bool LevelChangeStrategy::isReadyToChange(const SDL_Event &event) const {
    return change_on_event(event);    
}

void LevelChangeStrategy::setStrategy(const std::function<bool(float)> &strategy) {
    change_on_update = strategy;
}

void LevelChangeStrategy::setStrategy(const std::function<bool(const SDL_Event&)> &strategy) {
    change_on_event = strategy;
}

LevelChangeStrategy::~LevelChangeStrategy() {}



bool LevelManager::levelRunning() {
    if (!current_level)
        return false;
    return current_level->isRunning();
}

void LevelManager::nextLevel() {
    if (level_index == creators.size()) {
        quit();
        return;
    }
    current_level.reset(creators[level_index]->create());
    current_level->preload(renderer);
    
    ++level_index;
}

LevelManager::LevelManager() {}

LevelManager& LevelManager::getInstance() {
    static LevelManager instance;
    return instance;
}

void LevelManager::start(SDL_Renderer *renderer_) {
    renderer = renderer_;
    nextLevel();
}

void LevelManager::quit() {
    current_level.reset();
    creators.clear();
    level_index = 0;
    renderer = nullptr;
    is_running = false;
}

bool LevelManager::isRunning() {
    return is_running;
}

void LevelManager::render() {
    if (levelRunning()) {
        current_level->draw(renderer);
        return;
    }
    if (loading_screen)
        loading_screen->draw(renderer);
}

void LevelManager::update(float dt) {
    if (levelRunning()) {
        current_level->update(dt);
        return;
    }

    if (level_changer) {
        if (level_changer->isReadyToChange(dt)) 
            nextLevel();
    } 
    else 
        nextLevel();

    if (loading_screen)
        loading_screen->update(dt);
}

void LevelManager::onEvents(const SDL_Event &event) {
    if (levelRunning()) {
        current_level->onEvents(event);
        return;
    }                
    if (level_changer) {
        if (level_changer->isReadyToChange(event)) 
            nextLevel();
    }
    else
        nextLevel();
}

void LevelManager::setLevelChangeStrategy(LevelChangeStrategy *strategy) {
    level_changer.reset(strategy);
}

void LevelManager::setLoadingScreen(LoadingScreen *screen) {
    loading_screen.reset(screen);
}

LevelManager::~LevelManager() {}