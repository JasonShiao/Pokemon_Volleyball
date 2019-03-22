#include "animationSet.h"

#include "cleanup.h"
#include "platform.h"
#include <iostream>

using namespace std;

AnimationSet::~AnimationSet() {
    cleanup(spriteSheet);
    if (whiteSpriteSheet != NULL)
        cleanup(whiteSpriteSheet);
}

Animation* AnimationSet::getAnimation(string name) {
    Animation* retAnimation = NULL;
    for(list<Animation>::iterator animation = animations.begin();
            animation != animations.end();
            animation++) {
        retAnimation = &(*animation);

        if (retAnimation->name == name) {
            return retAnimation;
        }
    }
    return retAnimation;
}

void AnimationSet::loadAnimationSet(string fileName,
                        list<DataGroupType> &groupTypes,
                        bool setColourKey,
                        int transparentPixelIndex,
                        bool createWhiteTexture) {
    ifstream file;
    string resPath = getResourcePath();
    file.open(resPath + fileName);

    if(file.good()) {
#       ifdef PLATFORM_MAC
        getline(file, imageName, '\n'); // '\r' for Mac, '\n' for Windows/Unix
#       else
        getline(file, imageName, '\n'); // '\n' for Windows/Unix
#       endif
        if(setColourKey) {
            SDL_Surface* spriteSurface = loadSurface(resPath + imageName, Globals::renderer);

            cout << "load animation set stage: 1.1"  << endl;
            SDL_Color* transparentPixel = &spriteSurface->format->palette->colors[transparentPixelIndex];
            cout << "load animation set stage: 2"  << endl;
            SDL_SetColorKey(spriteSurface, 1, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));

            cout << "load animation set stage: 3"  << endl;
            whiteSpriteSheet = convertSurfaceToTexture(spriteSurface, Globals::renderer, false);

            if (createWhiteTexture) {
                SDL_Surface* whiteSurface = loadSurface(resPath + "allwhite.png", Globals::renderer);
                surfacePaletteSwap(spriteSurface, whiteSurface);
                SDL_SetColorKey(spriteSurface, 1, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));
                whiteSpriteSheet = convertSurfaceToTexture(spriteSurface, Globals::renderer, false);

                cleanup(whiteSurface);
            } else {
                whiteSpriteSheet = NULL;
            }
            cleanup(spriteSurface);
        } else {
            spriteSheet = loadTexture(resPath + imageName, Globals::renderer);
        }

        string buffer;
        // 2nd line of the .fdset file
#       ifdef PLATFORM_MAC
        getline(file, buffer, '\n'); // '\r' for Mac, '\n' for Windows/Unix
#       else
        getline(file, buffer, '\n'); // '\n' for Windows/Unix
#       endif
        stringstream ss;
        buffer = Globals::clipOffDataHeader(buffer);
        ss << buffer;
        int numberOfAnimations;
        ss >> numberOfAnimations;

        cout << "Load each animation"  << endl;
        for(int i = 0; i < numberOfAnimations; i++) {
            Animation newAnimation;
            newAnimation.loadAnimation(file, groupTypes);
            animations.push_back(newAnimation);
        }

    }

    file.close();

}
