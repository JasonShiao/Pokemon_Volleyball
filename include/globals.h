#ifndef GLOBALS_H
#define GLOBALS_H


#include <string>
#include <iostream>
#include "SDL.h"
#include "randomNumber.h"

using namespace std;

class Globals{
public:
    //math helpers
    static const float PI;

    //useful for me as a dev
    static bool debugging;

    //sdl related
    static int ScreenWidth, ScreenHeight, ScreenScale;
    static SDL_Renderer* renderer;
    static const Uint8 *keyboard_state_array;

    // clips off header
    static string clipOffDataHeader(string data);
};


#endif
