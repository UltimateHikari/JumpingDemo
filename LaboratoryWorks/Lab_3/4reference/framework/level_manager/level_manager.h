#pragma once
#include "level.h"
#include <memory>
#include <vector>
#include <tuple>
#include <type_traits>
#include <stdexcept>
#include <functional>

class LevelChangeStrategy {
    std::function<bool(float)> change_on_update;
    std::function<bool(const SDL_Event&)> change_on_event;

public:
    LevelChangeStrategy();
    bool isReadyToChange(float dt) const;
    bool isReadyToChange(const SDL_Event &event) const;

    void setStrategy(const std::function<bool(float dt)> &strategy);
    void setStrategy(const std::function<bool(const SDL_Event&)> &strategy);

    virtual ~LevelChangeStrategy();
};

class LoadingScreen {
public:
    virtual void update(float dt) = 0;
    virtual void draw(SDL_Renderer *renderer) = 0;

    virtual ~LoadingScreen() {}
};



class AbstractLevelCreator {
public:
    virtual Level *create() const = 0;
    virtual ~AbstractLevelCreator() {}
};

template <class level_type>
class LevelCreator : public AbstractLevelCreator {
public:
    Level *create() const {
        return new level_type();
    }
};

class LevelManager : public InputHandler { 
    using CreatorPtr = std::unique_ptr<AbstractLevelCreator>;
    std::vector<CreatorPtr> creators;
    std::unique_ptr<Level> current_level;

    template <size_t index, class... level_types>
    struct LevelPusher {
        static void push(std::vector<CreatorPtr> &v) {
            LevelPusher<index - 1, level_types...>::push(v);

            using level_type = typename std::tuple_element<index, std::tuple<level_types...>>::type;
            auto level_ptr = std::make_unique<LevelCreator<level_type>>();
            v.push_back(std::move(level_ptr));
        }
    };

    template <class... level_types>
    struct LevelPusher<0, level_types...> {
        static void push(std::vector<CreatorPtr> &v) {
            using level_type = typename std::tuple_element<0, std::tuple<level_types...>>::type;
            auto level_ptr = std::make_unique<LevelCreator<level_type>>();
            v.push_back(std::move(level_ptr));
        }
    };

    std::unique_ptr<LevelChangeStrategy> level_changer;
    std::unique_ptr<LoadingScreen> loading_screen;

    int level_index = 0;
    bool is_running = true;

    SDL_Renderer *renderer = nullptr;

    bool levelRunning();
    void nextLevel();
    
    LevelManager();
public:
    static LevelManager& getInstance();

    template <class... level_types> void addLevel();


    void start(SDL_Renderer *renderer_);
    void quit();
    bool isRunning();

    void render();
    void update(float dt);
    void onEvents(const SDL_Event &event);
   
    void setLevelChangeStrategy(LevelChangeStrategy *strategy);
    void setLoadingScreen(LoadingScreen *screen);

    ~LevelManager();
};

template <class... level_types> 
void LevelManager::addLevel() {
    LevelPusher<sizeof...(level_types) - 1, level_types...>::push(creators);
}
