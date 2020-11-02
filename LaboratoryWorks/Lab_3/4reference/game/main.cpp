#include "game.h"

b2Vec2 gravity{0.0, 0.0};
b2World collide_world(gravity); 

#ifdef _WIN32 
struct DebugConsole {
    DebugConsole() {
        AllocConsole();
        freopen_s(&old_, "CONOUT$", "wt", stdout);
    }
    ~DebugConsole() {
        fflush(stdout);
        FreeConsole();
        fclose(old_);
    }
private:
    FILE *old_ = nullptr;
};
#endif

int SDL_main(int argc, char ** argv) {
    DebugConsole debug_console;   //delete it to hide console
    if (initialize() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    {
        Game game;
        game.preload();
        Uint32 ticks = SDL_GetTicks();
        while (game.isRunning()) {
            float dt = (SDL_GetTicks() - ticks) / 1000.0f;
            ticks = SDL_GetTicks();
            game.render();
            game.update(dt);
		    game.handleEvents();
        }
    } 
    finalize();
    return EXIT_SUCCESS;
}

