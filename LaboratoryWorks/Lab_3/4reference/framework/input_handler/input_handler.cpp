#include "input_handler.h"

InputHandler::InputHandler() {}

void InputHandler::onEvents(const SDL_Event &event) {
    switch (event.type)
    {
    case SDL_WINDOWEVENT:
        switch (event.window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            onWindowShown();
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            onWindowHidden();
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            onWindowExpose();
            break;
        case SDL_WINDOWEVENT_MOVED:
            onWindowMove(event.window.data1, event.window.data2);
            break;
        case SDL_WINDOWEVENT_RESIZED:
            onWindowResize(event.window.data1, event.window.data2);
            break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            onWindowResize(event.window.data1, event.window.data2);
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            onWindowMinimize();
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            onWindowMaximize();
            break;
        case SDL_WINDOWEVENT_RESTORED:
            onWindowRestore();
            break;
        case SDL_WINDOWEVENT_ENTER:
            onWindowMouseFocus();
            break;
        case SDL_WINDOWEVENT_LEAVE:
            onWindowMouseBlur();
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            onWindowInputFocus();
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            onWindowInputBlur();
            break;
        case SDL_WINDOWEVENT_CLOSE:
            onWindowClose();
            break;

        default:
            SDL_Log("Window got unknown event %d", event.window.event);
            break;
        }
    
    case SDL_KEYDOWN:
        onKeyDown(event.key.keysym);
        break;
    
    case SDL_KEYUP:
        onKeyUp(event.key.keysym);
        break;
    
    case SDL_MOUSEBUTTONDOWN:
        switch (event.button.button) {
        case SDL_BUTTON_LEFT:
            onLButtonDown(event.button.x, event.button.y);
            break;
        case SDL_BUTTON_RIGHT:
            onRButtonDown(event.button.x, event.button.y);
            break;
        case SDL_BUTTON_MIDDLE:
            onMButtonDown(event.button.x, event.button.y);
        }
        break;

    case SDL_MOUSEBUTTONUP:
        switch (event.button.button) {
        case SDL_BUTTON_LEFT:
            onLButtonUp(event.button.x, event.button.y);
            break;
        case SDL_BUTTON_RIGHT:
            onRButtonUp(event.button.x, event.button.y);
            break;
        case SDL_BUTTON_MIDDLE:
            onMButtonUp(event.button.x, event.button.y);
        }
        break;

    case SDL_MOUSEMOTION: 
        onMouseMove(
            event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel, 
            (event.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0, 
            (event.motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0,
            (event.motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0
        );
        break;

    case SDL_JOYAXISMOTION: 
        onJoyAxis(event.jaxis.which, event.jaxis.axis, event.jaxis.value);
        break;
 
    case SDL_JOYBALLMOTION: 
        onJoyBall(event.jball.which, event.jball.ball, event.jball.xrel, event.jball.yrel);
        break;
 
    case SDL_JOYHATMOTION: 
        onJoyHat(event.jhat.which, event.jhat.hat, event.jhat.value);
        break;

    case SDL_JOYBUTTONDOWN: 
        onJoyButtonDown(event.jbutton.which, event.jbutton.button);
        break;
 
    case SDL_JOYBUTTONUP: 
        onJoyButtonUp(event.jbutton.which, event.jbutton.button);
        break;
 
    case SDL_QUIT: {
        onQuit();
        break;
    }
 
    case SDL_SYSWMEVENT: {
        //Ignore
        break;
    }

    default: 
        onUser(event.user.type, event.user.code, event.user.data1, event.user.data2);
        break;
    }
}

void InputHandler::onWindowInputFocus() {}

void InputHandler::onWindowInputBlur() {}

void InputHandler::onWindowMouseFocus() {}

void InputHandler::onWindowMouseBlur() {}

void InputHandler::onWindowShown() {}

void InputHandler::onWindowHidden() {}

void InputHandler::onWindowExpose() {}

void InputHandler::onWindowClose() {}

void InputHandler::onWindowMinimize() {}

void InputHandler::onWindowMaximize() {}

void InputHandler::onWindowRestore() {}

void InputHandler::onWindowMove(Sint32 data1, Sint32 data2) {}

void InputHandler::onWindowResize(Sint32 data1, Sint32 data2) {}
    

void InputHandler::onKeyDown(const SDL_Keysym &keysym) {}

void InputHandler::onKeyUp(const SDL_Keysym &keysym) {}


void InputHandler::onMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle) {}

void InputHandler::onMouseWheel(bool Up, bool Down) {}    //Not implemented

void InputHandler::onLButtonDown(int mX, int mY) {}

void InputHandler::onLButtonUp(int mX, int mY) {}

void InputHandler::onRButtonDown(int mX, int mY) {}

void InputHandler::onRButtonUp(int mX, int mY) {}

void InputHandler::onMButtonDown(int mX, int mY) {}

void InputHandler::onMButtonUp(int mX, int mY) {}


void InputHandler::onJoyAxis(Uint8 which, Uint8 axis, Sint16 value) {}

void InputHandler::onJoyButtonDown(Uint8 which, Uint8 button) {}

void InputHandler::onJoyButtonUp(Uint8 which, Uint8 button) {}

void InputHandler::onJoyHat(Uint8 which, Uint8 hat, Uint8 value) {}

void InputHandler::onJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel) {}


void InputHandler::onQuit() {}

void InputHandler::onUser(Uint8 type, int code, void* data1, void* data2) {}

InputHandler::~InputHandler() {}