#ifndef ENTITY_H
#define ENTITY_H

#include "globals.h"
#include "timeController.h"
#include "animationSet.h"

using namespace std;

// Abstract class
class Entity {
public:
    // reference constants

    /* Not change once assign */
    string type; // type of entity


    /* Might change after assign */
    int state;     // state label (for animation or other decision)

    float x, y;
    bool solid; // is this thing solid, can thigns pass through me
    //bool collideWithSolids = true; // sometimes we are solid, but I pass through other collideWithSolids
    bool active; // entity turned on or off

    bool moving; // is the entity moving
    //float angle;
    float moveSpeed_x;
    float moveSpeed_y;
    float moveSpeedMax_x;
    float moveSpeedMax_y;
    //float slideAngle; // direction pushed in
    //float slideAmount; // amount of push in the slideAngle
    //float moveLerp = 4;
    //float totalXMove, totalYMove; // keep track of total x, y movement per movement turn

    //SDL_Rect collisionBox;
    //SDL_Rect lastCollisionBox; // where our collisionbox was last
    //int collisionBoxW, collisionBoxH; // our default collisionBox size
    //float collisionBoxYOffset;

    AnimationSet *animSet; //set of all animations this entity can have
    Animation *currentAnim; //current animation the entity is using
    Frame *currentFrame; //the current frame in the above animation the entity using
    float frameTimer; //helps animate frame to frame

    Entity();
    virtual ~Entity();

    //VIRTUAL FUNCTIONS
	virtual void update();
	virtual void draw();

	virtual void move();
	virtual void updateMovement();
    virtual void changeAnimation(int newState, bool resetFrameToBeginning) = 0;//abstract function
    //virtual void updateCollisions(); //how we bump into stuff in the world

    //HELP FUNCTIONS
	//static float distanceBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2);
	//static float distanceBetweenTwoEntities(Entity *e1, Entity *e2);
	//static float angleBetweenTwoEntities(Entity *e1, Entity *e2);
	//static bool checkCollision(SDL_Rect cbox1, SDL_Rect cbox2);
	//static int angleToDirection(float angle);
	//static float angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2);
	//static float angleBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2);

    //global entities list I can refer to at anytime
    static list<Entity*> entities;
    static bool EntityCompare(const Entity* const &a, const Entity * const &b); //compare 2 entities in a list to help sorting (sorts based on y value)
    //static void removeInactiveEntitiesFromList(list<Entity*> *entityList, bool deleteEntities);
    //static void removeAllFromList(list<Entity*> *entityList, bool deleteEntities);

};

#endif
