#pragma once
#include <SDL.h>

class InputHandler {
public:
    InputHandler();
    virtual void onEvents(const SDL_Event &event); 

    //Window
    virtual void onWindowInputFocus();
    virtual void onWindowInputBlur();
    virtual void onWindowMouseFocus();
    virtual void onWindowMouseBlur();

    virtual void onWindowShown();
    virtual void onWindowHidden();
    virtual void onWindowExpose();
    virtual void onWindowClose();

    virtual void onWindowMinimize();
    virtual void onWindowMaximize();
    virtual void onWindowRestore();

    virtual void onWindowMove(Sint32 data1, Sint32 data2);

    virtual void onWindowResize(Sint32 data1, Sint32 data2);
    
    //Keyboard
    virtual void onKeyDown(const SDL_Keysym &keysym);
    virtual void onKeyUp(const SDL_Keysym &keysym);

    //Mouse
    virtual void onMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);
    virtual void onMouseWheel(bool Up, bool Down);    //Not implemented
    virtual void onLButtonDown(int mX, int mY);
    virtual void onLButtonUp(int mX, int mY);
    virtual void onRButtonDown(int mX, int mY);
    virtual void onRButtonUp(int mX, int mY);
    virtual void onMButtonDown(int mX, int mY);
    virtual void onMButtonUp(int mX, int mY);

    //Joystick
    virtual void onJoyAxis(Uint8 which, Uint8 axis, Sint16 value);
    virtual void onJoyButtonDown(Uint8 which, Uint8 button);
    virtual void onJoyButtonUp(Uint8 which, Uint8 button);
    virtual void onJoyHat(Uint8 which, Uint8 hat, Uint8 value);
    virtual void onJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel);

    //Other
    virtual void onQuit();
    virtual void onUser(Uint8 type, int code, void* data1, void* data2);

    virtual ~InputHandler();
};