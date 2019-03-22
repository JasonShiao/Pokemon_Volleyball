#ifndef ANIMATIONSET_H
#define ANIMATIONSET_H

#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include "SDL.h"
#include "groupBuilder.h"
#include "animation.h"

using namespace std;

class AnimationSet {
public:
    string imageName; // spriteSheet image
    SDL_Texture *spriteSheet;
    SDL_Texture *whiteSpriteSheet = NULL; // Show if spriteSheet is damaged
    list<Animation> animations;

    ~AnimationSet();

    Animation* getAnimation(string name);

    void loadAnimationSet(string fileName,
                            list<DataGroupType> &groupTypes,
                            bool setColourKey = false,
                            int transparentPixelIndex = 0,
                            bool createWhiteTexture = false);
};

#endif
