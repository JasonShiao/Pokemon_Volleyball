#ifndef VOLLEYBALL_H
#define VOLLEYBALL_H

#include "entity.h"
#include "SDL2/SDL.h"

using namespace std;

class Volleyball: public Entity {
public:
    static const string VOLLEYBALL_ANIM_NORMAL;
    static const string VOLLEYBALL_ANIM_SMASH;

    static const int VOLLEYBALL_STATE_NORMAL;
    static const int VOLLEYBALL_STATE_SMASH;

    static const float GRAVITY;

    SDL_Point boundary_max, boundary_min;
    float default_x, default_y; // reset (default) position

    Entity* last_collision = NULL;

    Volleyball(AnimationSet *animSet, float init_pos_x, float init_pos_y);

    void draw(); //overriding entities draw
    void update();
    void move();
    void updateMovement();
    void updateCollision();

    void resetPosition();
    void resetSpeed();

    void changeAnimation(int newState, bool resetFrameToBeginning);
    void updateAnimation();

};



#endif