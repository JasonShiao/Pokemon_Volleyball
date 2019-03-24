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

    float last_x, last_y; 

    float radius;

    Entity* last_collision;


    Volleyball(AnimationSet *animSet, float init_pos_x, float init_pos_y);
    virtual ~Volleyball();

    /* General method from Parent class */
    void draw(); //overriding entities draw
    void update();
    void move();
    void updateMovement();

    void changeAnimation(int newState, bool resetFrameToBeginning);
    void updateAnimation();


    /* Specific to this class */
    void updateCollision();

    void reset();
    void resetPosition(float x, float y);

    bool isLanding();

};



#endif
