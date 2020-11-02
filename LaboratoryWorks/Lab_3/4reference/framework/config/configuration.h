#pragma once 
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h> 

#define CXSCREEN 0x00000000
#define CYSCREEN 0x00000001

extern const int WINDOW_FLAGS;

int initialize();
void finalize();

int convert(SDL_Renderer *renderer, 
    int field_width, 
    int field_height, 
    double value);

SDL_Point convert(SDL_Renderer *renderer, 
    int field_width, 
    int field_height, 
    const SDL_Point &point);

int getDesktopMetrics(int flag);
