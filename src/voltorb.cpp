#include "voltorb.h"

/****************************************************************
 *              Static Data Member Initialization               *
 ****************************************************************/
const string Voltorb::VOLTORB_ANIM_IDLE_LEFT = "face_left";
const string Voltorb::VOLTORB_ANIM_IDLE_RIGHT = "face_right";
const string Voltorb::VOLTORB_ANIM_MOVE_LEFT = "move_left";
const string Voltorb::VOLTORB_ANIM_MOVE_RIGHT = "move_right";
const string Voltorb::VOLTORB_ANIM_JUMP_LEFT = "jump_left";
const string Voltorb::VOLTORB_ANIM_JUMP_RIGHT = "jump_right";
//static const string VOLTORB::VOLTORB_ANIM_SMASH_LEFT = "smash_left";
//static const string VOLTORB::VOLTORB_ANIM_SMASH_RIGHT = "smash_right";
const string Voltorb::VOLTORB_ANIM_WIN = "win";
const string Voltorb::VOLTORB_ANIM_LOSE = "lose";

const int Voltorb::VOLTORB_STATE_IDLE = 0;
const int Voltorb::VOLTORB_STATE_MOVE = 1;
const int Voltorb::VOLTORB_STATE_JUMP = 2;
//static const int VOLTORB::VOLTORB_STATE_SMASH = 3;
const int Voltorb::VOLTORB_STATE_WIN = 4;
const int Voltorb::VOLTORB_STATE_LOSE = 5;

const int Voltorb::DIR_LEFT = 0, Voltorb::DIR_RIGHT= 1;

const float Voltorb::GRAVITY = 720;

int Voltorb::total_voltorb = 0; // initial no voltorb object

list<Voltorb*> Voltorb::voltorbs;

/****************************************************************
 *                      Method Definition                       *
 ****************************************************************/
Voltorb::Voltorb(AnimationSet *animSet, int direction){

	this->animSet = animSet;
	this->type = "voltorb";

	this->solid = true;
	this->active = true;

	this->moveSpeed_x = 0; this->moveSpeed_y = 0;
	this->moveSpeedMax = 250;
    this->direction = direction;
	onTheGround = true; // jumping or on the ground

	points = 0;

	index = total_voltorb; // set the index of the new voltorb
	total_voltorb++;

	/* Default setting for the 2 voltorbs/players */
	if (index == 0) {
		setKeyConfig(SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT);
		boundary_max.x = Globals::ScreenWidth - 50;
		boundary_max.y = Globals::ScreenHeight - 50;

		boundary_min.x = Globals::ScreenWidth / 2 + 50;
		boundary_min.y = 50;

		default_x = boundary_max.x;
		default_y = boundary_max.y;

		x = boundary_max.x;
		y = boundary_max.y;

		scoring_area.x = 0;
		scoring_area.y = Globals::ScreenHeight - 50;
		scoring_area.w = Globals::ScreenWidth / 2;
		scoring_area.h = 50;

		voltorbs.push_back(this);

	} else if (index == 1) {
		setKeyConfig(SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D);
		boundary_max.x = Globals::ScreenWidth / 2 - 50;
		boundary_max.y = Globals::ScreenHeight - 50;

		boundary_min.x = 50;
		boundary_min.y = 50;

		default_x = boundary_min.x;
		default_y = boundary_max.y;

		x = boundary_min.x;
		y = boundary_max.y;

		scoring_area.x = Globals::ScreenWidth / 2;
		scoring_area.y = Globals::ScreenHeight - 50;
		scoring_area.w = Globals::ScreenWidth / 2;
		scoring_area.h = 50;

		voltorbs.push_back(this);
	} else {
		// Config not used now
		setKeyConfig(SDL_SCANCODE_I, SDL_SCANCODE_K, SDL_SCANCODE_J, SDL_SCANCODE_L);
		boundary_max.x = Globals::ScreenWidth - 50;
		boundary_max.y = Globals::ScreenHeight - 50;

		boundary_min.x = 50;
		boundary_min.y = 50;

		default_x = Globals::ScreenWidth / 2;
		default_y = boundary_max.y;

		x = Globals::ScreenWidth / 2;
		y = boundary_max.y;

		scoring_area.x = 0;
		scoring_area.y = Globals::ScreenHeight - 50;
		scoring_area.w = Globals::ScreenWidth;
		scoring_area.h = 50;

		voltorbs.push_back(this);
	}

	changeAnimation(VOLTORB_STATE_IDLE, true);

}

Voltorb::~Voltorb()
{
	;
}


/****************************************************************
 *                  Method from Parent Class                    *
 ****************************************************************/

void Voltorb::draw(){
	if (currentFrame != NULL && active){
		currentFrame->Draw(animSet->spriteSheet, x, y);
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

void Voltorb::updateMovement() {
	// inertial movement changes, change animation here

	if( Globals::keyboard_state_array[this->_up])
	{
		if (onTheGround)
		{
			moveSpeed_y = -2*moveSpeedMax;
			onTheGround = false;
		}
		//y -= moveSpeedMax * TimeController::timeController.dT;
		//screen_changed = true;
	}
	if( Globals::keyboard_state_array[this->_down])
	{
		//y += moveSpeedMax * TimeController::timeController.dT;
		//screen_changed = true;
	}
	if( Globals::keyboard_state_array[this->_left])
	{
		x -= moveSpeedMax * TimeController::timeController.dT;
		//screen_changed = true;
	}
	if( Globals::keyboard_state_array[this->_right])
	{
		x += moveSpeedMax * TimeController::timeController.dT;
		//screen_changed = true;
	}
}

void Voltorb::move(){
	// no inertial motion for voltorb

	// jumping and gravity: parabola motion
	y += (2*moveSpeed_y + Voltorb::GRAVITY * TimeController::timeController.dT) * TimeController::timeController.dT / 2;
	moveSpeed_y += Voltorb::GRAVITY * TimeController::timeController.dT;
	if(y >= Globals::ScreenHeight - 50)
	{
		// Land on the ground
		y = Globals::ScreenHeight - 50;
		moveSpeed_y = 0;
		onTheGround = true;
	}

	// Boundary limit
	if (x > boundary_max.x) {
		x = boundary_max.x;
	}else if (x < boundary_min.x) {
		x = boundary_min.x;
	}

	if (y > boundary_max.y) {
		y = boundary_max.y;
	}

}

void Voltorb::update(){

	/*
		inertial movement changes (user input, collision, gravity),
		also change animation here
	*/
	updateMovement();

	move(); // inertial movement

	updateAnimation();

}


void Voltorb::changeAnimation(int newState, bool resetFrameToBeginning){
	state = newState;

	if (state == VOLTORB_STATE_IDLE){
		if (direction == Voltorb::DIR_LEFT)
			currentAnim = animSet->getAnimation(VOLTORB_ANIM_IDLE_LEFT);
		else if (direction == Voltorb::DIR_RIGHT)
			currentAnim = animSet->getAnimation(VOLTORB_ANIM_IDLE_RIGHT);
	}
    else if (state == VOLTORB_STATE_MOVE){
        if (direction == Voltorb::DIR_LEFT)
            currentAnim = animSet->getAnimation(VOLTORB_ANIM_MOVE_LEFT);
        else if (direction == Voltorb::DIR_RIGHT)
            currentAnim = animSet->getAnimation(VOLTORB_ANIM_MOVE_RIGHT);
    }
	else if (state == VOLTORB_STATE_JUMP){
		if (direction == Voltorb::DIR_LEFT)
			currentAnim = animSet->getAnimation(VOLTORB_ANIM_JUMP_LEFT);
		else if (direction == Voltorb::DIR_RIGHT)
			currentAnim = animSet->getAnimation(VOLTORB_ANIM_JUMP_RIGHT);
	}
	/*else if (state == VOLTORB_STATE_SMASH){
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(VOLTORB_ANIM_SMASH_LEFT);
		else if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(VOLTORB_ANIM_SMASH_RIGHT);
	}*/
	else if (state == VOLTORB_STATE_WIN){
		currentAnim = animSet->getAnimation(VOLTORB_ANIM_WIN);
	}
	else if (state == VOLTORB_STATE_LOSE){
		currentAnim = animSet->getAnimation(VOLTORB_ANIM_LOSE);
	}

	if (resetFrameToBeginning)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
}

void Voltorb::updateAnimation(){ // rolling frames in an animation
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

 void Voltorb::setKeyConfig (SDL_Scancode up, SDL_Scancode down, SDL_Scancode left, SDL_Scancode right){
 	this->_up = up;
 	this->_down = down;
 	this->_left = left;
 	this->_right = right;
 }

void Voltorb::resetPosition() {
	x = default_x;
	y = default_y;
}

void Voltorb::resetPoint() {
	points = 0;
}


float Voltorb::getScoringArea_Left()
{
	return scoring_area.x;
}

float Voltorb::getScoringArea_Right()
{
    return scoring_area.x + scoring_area.w;
}

float Voltorb::getScoringArea_Top()
{
	return scoring_area.y;
}

float Voltorb::getScoringArea_Bottom()
{
    return scoring_area.y + scoring_area.h;
}
