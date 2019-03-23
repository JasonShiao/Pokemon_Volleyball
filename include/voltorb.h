#ifndef VOLTORB_H
#define VOLTORB_H

#include <string>
#include "entity.h"
#include "globals.h"
#include "animationSet.h"
#include <SDL2/SDL.h>

using namespace std;

class Voltorb: public Entity {
public:
    // Refernce variables
    /* For animation */
    static const string VOLTORB_ANIM_IDLE_LEFT;
    static const string VOLTORB_ANIM_IDLE_RIGHT;
	static const string VOLTORB_ANIM_MOVE_LEFT;
	static const string VOLTORB_ANIM_MOVE_RIGHT;
	static const string VOLTORB_ANIM_JUMP_LEFT;
    static const string VOLTORB_ANIM_JUMP_RIGHT;
    //static const string VOLTORB_ANIM_SMASH_LEFT;
    //static const string VOLTORB_ANIM_SMASH_RIGHT;
    static const string VOLTORB_ANIM_WIN;
    static const string VOLTORB_ANIM_LOSE;
    /* For state */
    static const int VOLTORB_STATE_IDLE;
    static const int VOLTORB_STATE_MOVE;

    static const int VOLTORB_STATE_JUMP;
	//static const int VOLTORB_STATE_SMASH;
    static const int VOLTORB_STATE_WIN;
    static const int VOLTORB_STATE_LOSE;

    static const int DIR_LEFT, DIR_RIGHT; // Face left or Face right

    static const float GRAVITY;

    static int total_voltorb; // keep track of the total number of voltorb object
    static list<Voltorb*> voltorbs; // might not need to keep total_voltorb now

    /* Not changed once assign */
    int index; // use to distinguish each voltorb object
    SDL_Point boundary_max, boundary_min;
    SDL_Rect scoring_area;

    float default_x, default_y; // reset (default) position
    SDL_Scancode _up, _down, _left, _right;

    /* Might changed after assign */
    int direction; //face direction
    int points; // points acquired
    bool onTheGround;


    //static bool VoltorbCompare(const Voltorb* const &a, const Voltorb * const &b);

    Voltorb(AnimationSet *animSet, int direction);
    virtual ~Voltorb();

    void draw(); //overriding entities draw

    void update();
    void move();
    void updateMovement();

    void changeAnimation(int newState, bool resetFrameToBeginning);
    void updateAnimation();


    void setKeyConfig (SDL_Scancode up, SDL_Scancode down, SDL_Scancode left, SDL_Scancode right);

    void resetPosition();
    void resetPoint();

    float getScoringArea_Left();
    float getScoringArea_Right();
    float getScoringArea_Top();
    float getScoringArea_Bottom();

};


#endif
