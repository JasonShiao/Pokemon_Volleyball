#include "volleyball.h"
#include "globals.h"
#include "timeController.h"
#include "SDL2/SDL.h"
#include "math.h"
#include "voltorb.h"
#include "wall.h"

/****************************************************************
 *              Static Data Member Initialization               *
 ****************************************************************/

const string Volleyball::VOLLEYBALL_ANIM_NORMAL = "normal";
const string Volleyball::VOLLEYBALL_ANIM_SMASH = "smash";

const int Volleyball::VOLLEYBALL_STATE_NORMAL = 0;
const int Volleyball::VOLLEYBALL_STATE_SMASH = 1;

const float Volleyball::GRAVITY = 300;


/****************************************************************
 *                      Method Definition                       *
 ****************************************************************/

 Volleyball::Volleyball(AnimationSet *animSet, float init_pos_x, float init_pos_y){

 	this->animSet = animSet;
 	type = "volleyball";
    this->solid = true;
	this->active = true;
    last_collision = NULL;

 	//setup default voltorb values

 	moveSpeed_x = 0; moveSpeed_y = 0;
 	moveSpeedMax = 400;

    boundary_max.x = Globals::ScreenWidth - 50;
    boundary_max.y = Globals::ScreenHeight - 50;
    boundary_min.x = 50;
    boundary_min.y = 50;

    default_x = init_pos_x;
    default_y = init_pos_y;

    x = default_x;
    y = default_y;

    last_x = x;
    last_y = y;

    radius = 55;

 	changeAnimation(VOLLEYBALL_STATE_NORMAL, true);

 }

Volleyball::~Volleyball()
{
    ;
}



/****************************************************************
 *                  Method from Parent Class                    *
 ****************************************************************/
 void Volleyball::draw(){
 	if (this->currentFrame != NULL && active){
 		this->currentFrame->Draw(animSet->spriteSheet, x, y);
 	}
 	//draw collsionBox
 	/*if (solid && Globals::debugging){
 		//sets the current drawing colour (Doesn't affect textures and what not)
 		//SDL_SetRenderDrawColor(Globals::renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
 		//SDL_RenderDrawRect(Globals::renderer, &lastCollisionBox);

 		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
 		SDL_RenderDrawRect(Globals::renderer, &collisionBox);

 		//SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
 		//SDL_RenderDrawRect(Globals::renderer, &hitBox);
 	}*/
 }

void Volleyball::updateMovement() {

    moveSpeed_y += Volleyball::GRAVITY * TimeController::timeController.dT;

 	if(y >= Globals::ScreenHeight - 50)
 	{
 		// Land on the ground
 		y = Globals::ScreenHeight - 50;
        if(moveSpeed_y <= 50)
            moveSpeed_y = 0;
        else
 		    moveSpeed_y = -(moveSpeed_y - 50); // 50 is air friction
 	}

 	// Boundary limit
 	if (x > boundary_max.x) {
 		x = boundary_max.x;
        moveSpeed_x = -moveSpeed_x;
 	}else if (x < boundary_min.x) {
 		x = boundary_min.x;
        moveSpeed_x = -moveSpeed_x;
 	}

 	if (y <= boundary_min.y) {
 		y = boundary_min.y;
        moveSpeed_y = -moveSpeed_y;
 	}

    for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
    {
        // Collide with middle_net (wall)
        if((*entity)->type.compare("wall") == 0)
        {
            // top
            if( ((Wall*)(*entity))->getBoundary_Top() >= last_y + radius
                    && ((Wall*)(*entity))->getBoundary_Top() < y + radius
                    && ((Wall*)(*entity))->getBoundary_Left() <= x + radius
                    && ((Wall*)(*entity))->getBoundary_Right() >= x - radius ) {
                if(moveSpeed_y > 0)
                    moveSpeed_y = -moveSpeed_y;
                //y = ((Wall*)(*entity))->getBoundary_Top() - radius;
            // left
            } else if( last_x + radius <= ((Wall*)(*entity))->getBoundary_Left()
                    && ((Wall*)(*entity))->getBoundary_Bottom() >= y - radius
                    && ((Wall*)(*entity))->getBoundary_Top() <= y + radius
                    && ((Wall*)(*entity))->getBoundary_Left() < x + radius ) {
                if(moveSpeed_x > 0)
                    moveSpeed_x = -moveSpeed_x;
                //x = ((Wall*)(*entity))->getBoundary_Left() - radius;
            // right
            }else if( last_x - radius >= ((Wall*)(*entity))->getBoundary_Right()
                    && ((Wall*)(*entity))->getBoundary_Bottom() >= y - radius
                    && ((Wall*)(*entity))->getBoundary_Top() <= y + radius
                    && x - radius < ((Wall*)(*entity))->getBoundary_Right() ) {
                if(moveSpeed_x < 0)
                    moveSpeed_x = -moveSpeed_x;
                //x = ((Wall*)(*entity))->getBoundary_Right() + radius;
            // bottom (no need for the middle net)
            } else if( last_y - radius >= ((Wall*)(*entity))->getBoundary_Bottom()
                    && y - radius < ((Wall*)(*entity))->getBoundary_Bottom()
                    && ((Wall*)(*entity))->getBoundary_Left() <= x + radius
                    && ((Wall*)(*entity))->getBoundary_Right() >= x - radius ) {
                if(moveSpeed_y < 0)
                    moveSpeed_y = -moveSpeed_y;
                //y = ((Wall*)(*entity))->getBoundary_Bottom() + radius;
            } else
                ;// do nothing
        }

        // Collide with Voltorb (player)
        if((*entity)->type.compare("voltorb") == 0)
        {
            //cout << " | an voltorb | " << endl;
            if(pow((*entity)->x - this->x, 2) + pow((*entity)->y - this->y, 2) <= pow(50, 2) )
            {
                // If collision just happen right before, ignore this one
                if(last_collision == (*entity)) {
                    // do nothing
                } else {
                    // Collide
                    last_collision = (*entity); // change the last collision object
                    if ( ((*entity)->x - this->x) <= 1 && ((*entity)->x - this->x) >= -1) {
                        this->moveSpeed_x = 0;
                    } else {
                        this->moveSpeed_x = (this->moveSpeedMax)
                                            * ( this->x - (*entity)->x)
                                            / sqrt(pow((*entity)->x - this->x, 2) + pow((*entity)->y - this->y, 2));
                    }
                    if ( ((*entity)->y - this->y) <= 1 && ((*entity)->y - this->y) >= -1 ) {
                        this->moveSpeed_y = 0;
                    } else {
                        this->moveSpeed_y = (this->moveSpeedMax)
                                            * ( this->y - (*entity)->y)
                                            / sqrt(pow((*entity)->x - this->x, 2) + pow((*entity)->y - this->y, 2));
                        if ( ((Voltorb*)(*entity))->onTheGround == false ) {
                            //
                        }
                    }
                }
            }
        }
    }

}

void Volleyball::move(){
    // no inertial motion for voltorb

    // jumping and gravity: parabola motion
    last_x = x;
    last_y = y;

    y += (2*moveSpeed_y + Volleyball::GRAVITY * TimeController::timeController.dT) * TimeController::timeController.dT / 2;
    x += (2*moveSpeed_x + Volleyball::GRAVITY * TimeController::timeController.dT) * TimeController::timeController.dT / 2;


}

void Volleyball::update(){

    /*
    	inertial movement changes (user input, collision, gravity),
    	also change animation here
    */
    updateMovement();

    move(); // inertial movement

    updateCollision(); //

    updateAnimation();

}


void Volleyball::changeAnimation(int newState, bool resetFrameToBeginning){
  	state = newState;

    if (state == VOLLEYBALL_STATE_NORMAL){
  		currentAnim = animSet->getAnimation(VOLLEYBALL_ANIM_NORMAL);
  	}
  	else if (state == VOLLEYBALL_STATE_SMASH){
  		currentAnim = animSet->getAnimation(VOLLEYBALL_ANIM_SMASH);
  	}

    if (resetFrameToBeginning)
  		currentFrame = currentAnim->getFrame(0);
    else
  		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
}

void Volleyball::updateAnimation(){ // rolling frames in an animation
  	if (currentFrame == NULL || currentAnim == NULL)
  		return; //cant do much with animations without pointers pointing to them :S


  	frameTimer += TimeController::timeController.dT;
  	//time to change frames :D
  	if (frameTimer >= currentFrame->duration)
  	{
  		currentFrame = currentAnim->getNextFrame(currentFrame);
  		frameTimer = 0;
  	}
}


/****************************************************************
 *              Specific Method for this Class                  *
 ****************************************************************/

void Volleyball::updateCollision() {

    // If the last collision leave the volleyball
    if(last_collision != NULL) {
        if(pow(last_collision->x - this->x, 2) + pow(last_collision->y - this->y, 2) > pow(50, 2)) {
            last_collision = NULL;
        }
    }

}


void Volleyball::reset()
{
    moveSpeed_x = 0;
    moveSpeed_y = 0;

	x = this->default_x;
	y = this->default_y;

    last_collision = NULL;

    // reset State
}


void Volleyball::resetPosition(float x, float y)
{
    default_x = x;
    default_y = y;

    this->x = default_x;
    this->y = default_y;
}

bool Volleyball::isLanding()
{
    return (y >= boundary_max.y);
}
