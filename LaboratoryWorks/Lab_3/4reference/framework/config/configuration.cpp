#include "configuration.h"
#include "../linear_algebra/linalg.h"

#include <iostream>
#include <stdexcept>
#include <cmath>

const int WINDOW_FLAGS = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;


static double RATIO;
static double HORIZONTAL_SPACE;
static double VERTICAL_SPACE;

using namespace linalg::aliases;

void setParameters(SDL_Renderer *renderer, int field_width, int field_height) {
    int window_width, window_height;
    SDL_GetRendererOutputSize(renderer, &window_width, &window_height);

    double r1 = static_cast<double>(window_width) / window_height;
    double r2 = static_cast<double>(field_width) / field_height;
    if (r1 > r2) 
        RATIO = static_cast<double>(window_height) / field_height;
    else 
        RATIO = static_cast<double>(window_width) / field_width;

    HORIZONTAL_SPACE = (window_width - (RATIO * field_width)) / 2.0;
    VERTICAL_SPACE = (window_height - (RATIO * field_height)) / 2.0;
}

int initialize() {
    if ((SDL_Init(SDL_INIT_EVERYTHING) != 0)) {
        std::cerr << "Could't initialize SDL: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    int IMG_FLAGS = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
    if ((IMG_Init(IMG_FLAGS) & IMG_FLAGS) != IMG_FLAGS) {
        std::cerr << "Could't initialize IMG: " << IMG_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    if(TTF_Init() == -1) {
        std::cerr << "Could't initialize TFF: " << TTF_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    int MIX_FLAGS = MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG;
    if (Mix_Init(MIX_FLAGS) & MIX_FLAGS != MIX_FLAGS) {
        std::cerr << "Could't initialize MIX: " << Mix_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 2048) != 0) {
		std::cerr << "Could't open Audio MIX: " << Mix_GetError() << std::endl;
		return EXIT_FAILURE;
	}
    return EXIT_SUCCESS;
}

void finalize() {
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int convert(SDL_Renderer *renderer, int field_width, int field_height, double value) {
    setParameters(renderer, field_width, field_height);
    return static_cast<int>(std::ceil(value * RATIO));
}   

SDL_Point convert(SDL_Renderer *renderer, int field_width, int field_height, const SDL_Point &point) {
    setParameters(renderer, field_width, field_height);
    double x = point.x;
    double y = point.y;
    double3x1 vector = {{ x, y, 1.0 }};
    double3x3 matrix = {
        { RATIO, 0.0, 0.0 }, 
        { 0.0, RATIO, 0.0 }, 
        { HORIZONTAL_SPACE, VERTICAL_SPACE, 0.0 }
    };    
    auto result = mul(matrix, vector);

    int new_x = static_cast<int>(result[0][0]);
    int new_y = static_cast<int>(result[0][1]);
    return SDL_Point{ new_x, new_y };
}

int getDesktopMetrics(int flag) {
    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return -1;
    }

    if (flag == CXSCREEN) 
        return dm.w;
    else if (flag == CYSCREEN)
        return dm.h;
    else
        return -1;
}