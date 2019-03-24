#ifndef WALL_H
#define WALL_H

#include <string>
#include "entity.h"
#include "globals.h"
#include "animationSet.h"
#include <SDL2/SDL.h>

using namespace std;

class Wall: public Entity {
public:
    //refernce variables
    static const string NORMAL_WALL;

    static const int WALL_STATE_NORNAL;

    float w, h;  // collision region w, h


    Wall(AnimationSet *animSet, int x, int y, int w, int h);
    virtual ~Wall();

    void draw(); //overriding entities draw
    void updateMovement();
    void update();
    void move();

    void changeAnimation(int newState, bool resetFrameToBeginning);
    void updateAnimation();

    float getBoundary_Left();
    float getBoundary_Right();
    float getBoundary_Top();
    float getBoundary_Bottom();

    float getCenter_x();
    float getCenter_y();
};


#endif
