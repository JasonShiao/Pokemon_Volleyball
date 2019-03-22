#ifndef FRAME_H
#define FRAME_H

#include <sstream>
#include <list>
#include "SDL.h"
#include "drawing_functions.h"
#include "globals.h"
#include "groupBuilder.h"

using namespace std;

class Frame {
public:
    int frameNumber; // identify the frame
    SDL_Rect clip; // specify the region for the frame on the spritesheet
    float duration;
    SDL_Point offset; // help align the frame in the animation
    list<Group*> frameData; // additional frameData

    void Draw(SDL_Texture *spriteSheet, float x, float y);

    // TODO:
    void loadFrame(ifstream &file, list<DataGroupType> &groupTypes);
};

#endif
