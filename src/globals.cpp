#include "globals.h"

const float Globals::PI = 3.14159;

bool Globals::debugging = true;

int Globals::ScreenWidth = 1032;
int Globals::ScreenHeight = 480;
int Globals::ScreenScale = 1;
SDL_Renderer* Globals::renderer = NULL;

const Uint8* Globals::keyboard_state_array = SDL_GetKeyboardState(NULL);

string Globals::clipOffDataHeader(string data) {
    int pos = data.find(":", 0); // return the position of ":"
    if(pos != -1) {
        data = data.substr(pos + 1, data.length() - pos + 2);
    }
    return data;
}
