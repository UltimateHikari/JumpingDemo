#pragma once
#include <string>
#include "SDL.h"

struct Color {
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 0;
    Color() {}
    Color(int r_, int g_, int b_, int a_)
    : r(r_), g(g_), b(b_), a(a_) {}
    void set(int r_, int g_, int b_, int a_) {
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }
    void set(int i) {
        r = g = b = a = i;
    }
};

class Window {
public:
    Window();
    Window(std::string title_, int width_, int height_);
    bool isClosed() const;
    void PoolEvents();
    void drawRect(const SDL_Rect *rect, const Color &color) const;
    void drawLine(int x1, int y1, int x2, int y2, const Color &color) const;
    void present() const;
    void clear() const;
    int getWidth() const;
    int getHeight() const;
    operator SDL_Renderer* () const;
    ~Window();
private:
    int init();

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    bool is_closed = false;
    std::string title = "Title";
    int width = 800;
    int height = 600;
};