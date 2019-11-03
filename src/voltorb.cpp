#include "voltorb.h"

/****************************************************************
 *              Static Data Member Initialization               *
 ****************************************************************/
const string Voltorb::VOLTORB_ANIM_IDLE_LEFT = "face_left";
const string Voltorb::VOLTORB_ANIM_IDLE_RIGHT = "face_right";
//const string Voltorb::VOLTORB_ANIM_MOVE_LEFT = "move_left";
//const string Voltorb::VOLTORB_ANIM_MOVE_RIGHT = "move_right";
const string Voltorb::VOLTORB_ANIM_JUMP_LEFT = "jump_left";
const string Voltorb::VOLTORB_ANIM_JUMP_RIGHT = "jump_right";
const string Voltorb::VOLTORB_ANIM_ANGRY_LEFT = "angry_left";
const string Voltorb::VOLTORB_ANIM_ANGRY_RIGHT = "angry_right";
const string Voltorb::VOLTORB_ANIM_WIN = "win";
const string Voltorb::VOLTORB_ANIM_LOSE = "lose";

const int Voltorb::VOLTORB_STATE_IDLE = 0;
//const int Voltorb::VOLTORB_STATE_MOVE = 1;
const int Voltorb::VOLTORB_STATE_JUMP = 2;
const int Voltorb::VOLTORB_STATE_ANGRY= 3;
const int Voltorb::VOLTORB_STATE_WIN = 4;
const int Voltorb::VOLTORB_STATE_LOSE = 5;

const int Voltorb::DIR_LEFT = 0, Voltorb::DIR_RIGHT= 1;

const float Voltorb::GRAVITY = 450;

static const float MOVESPEED_MAX_X = 175;
static const float MOVESPEED_MAX_Y = 480;

static const float RADIUS = 50;

static const float SMASHSPEED_X = 790;
static const float SMASHSPEED_Y = 500;

static const float SMASHSPEED_IDLE_X = 400;
static const float SMASHSPEED_IDLE_Y = 0;

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
	this->moveSpeedMax_x = MOVESPEED_MAX_X;
	this->moveSpeedMax_y = MOVESPEED_MAX_Y;

    this->direction = direction;

	this->state = VOLTORB_STATE_IDLE;

	onTheGround = true; // jumping or on the ground
	angry = false;
	angry_duration = 0.8;
	angry_timer = 0;

	points = 0;

	index = total_voltorb; // set the index of the new voltorb
	total_voltorb++;

	/* Default setting for the 2 voltorbs/players */
	if (index == 0) {
		setKeyConfig(SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, \
						SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, \
						SDL_SCANCODE_RETURN);
		boundary_max.x = Globals::ScreenWidth - RADIUS;
		boundary_max.y = Globals::ScreenHeight - RADIUS;

		boundary_min.x = Globals::ScreenWidth / 2 + RADIUS;
		boundary_min.y = RADIUS;

		default_x = boundary_max.x;
		default_y = boundary_max.y;

		x = boundary_max.x;
		y = boundary_max.y;

		scoring_area.x = 0;
		scoring_area.y = Globals::ScreenHeight - RADIUS;
		scoring_area.w = Globals::ScreenWidth / 2;
		scoring_area.h = RADIUS;

		voltorbs.push_back(this);

	} else if (index == 1) {
		setKeyConfig(SDL_SCANCODE_W, SDL_SCANCODE_S, \
						SDL_SCANCODE_A, SDL_SCANCODE_D, \
						SDL_SCANCODE_SPACE);
		boundary_max.x = Globals::ScreenWidth / 2 - RADIUS;
		boundary_max.y = Globals::ScreenHeight - RADIUS;

		boundary_min.x = RADIUS;
		boundary_min.y = RADIUS;

		default_x = boundary_min.x;
		default_y = boundary_max.y;

		x = boundary_min.x;
		y = boundary_max.y;

		scoring_area.x = Globals::ScreenWidth / 2;
		scoring_area.y = Globals::ScreenHeight - RADIUS;
		scoring_area.w = Globals::ScreenWidth / 2;
		scoring_area.h = RADIUS;

		voltorbs.push_back(this);
	} else {
		// Config not used now
		setKeyConfig(SDL_SCANCODE_I, SDL_SCANCODE_K, \
						SDL_SCANCODE_J, SDL_SCANCODE_L, \
						SDL_SCANCODE_Y);
		boundary_max.x = Globals::ScreenWidth - RADIUS;
		boundary_max.y = Globals::ScreenHeight - RADIUS;

		boundary_min.x = RADIUS;
		boundary_min.y = RADIUS;

		default_x = Globals::ScreenWidth / 2;
		default_y = boundary_max.y;

		x = Globals::ScreenWidth / 2;
		y = boundary_max.y;

		scoring_area.x = 0;
		scoring_area.y = Globals::ScreenHeight - RADIUS;
		scoring_area.w = Globals::ScreenWidth;
		scoring_area.h = RADIUS;

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
			moveSpeed_y = -moveSpeedMax_y;
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
		x -= moveSpeedMax_x * TimeController::timeController.dT;
		//screen_changed = true;
	}
	if( Globals::keyboard_state_array[this->_right])
	{
		x += moveSpeedMax_x * TimeController::timeController.dT;
		//screen_changed = true;
	}
}

void Voltorb::updateState()
{
	// Update landing state
	if(y >= Globals::ScreenHeight - RADIUS)
	{
		// Land on the ground
		if(onTheGround == false) {
			onTheGround = true;
			state = VOLTORB_STATE_IDLE;
			changeAnimation(state, true);
		}
	}
	else
	{
		if(onTheGround == true) {
			onTheGround = false;
			state = VOLTORB_STATE_JUMP;
			changeAnimation(state, true);
		}
	}

	// Update angry state
	if(angry == true){
		if(onTheGround == true)
			// relief on landing
			angry = false;
		else
		{
			// relief on timeout
			angry_timer -= TimeController::timeController.dT;
			if(angry_timer <= 0){
				angry_timer = 0;
				angry = false;
				state = VOLTORB_STATE_JUMP;
				changeAnimation(state, true);
			}
		}
	}

	// angry/smash key pressed
	if( Globals::keyboard_state_array[this->_smash])
	{
		if(angry == false && onTheGround == false)
		{
			angry = true;
			angry_timer = angry_duration;
			state = VOLTORB_STATE_ANGRY;
			changeAnimation(state, true);
		}
	}



}

void Voltorb::move(){
	// no inertial motion for voltorb

	// jumping and gravity: parabola motion
	y += (2*moveSpeed_y + Voltorb::GRAVITY * TimeController::timeController.dT) * TimeController::timeController.dT / 2;
	moveSpeed_y += Voltorb::GRAVITY * TimeController::timeController.dT;
	if(y >= Globals::ScreenHeight - RADIUS)
	{
		// Land on the ground
		y = Globals::ScreenHeight - RADIUS;
		moveSpeed_y = 0;
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

	updateState();

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
    /*else if (state == VOLTORB_STATE_MOVE){
        if (direction == Voltorb::DIR_LEFT)
            currentAnim = animSet->getAnimation(VOLTORB_ANIM_MOVE_LEFT);
        else if (direction == Voltorb::DIR_RIGHT)
            currentAnim = animSet->getAnimation(VOLTORB_ANIM_MOVE_RIGHT);
    }*/
	else if (state == VOLTORB_STATE_JUMP){
		if (direction == Voltorb::DIR_LEFT)
			currentAnim = animSet->getAnimation(VOLTORB_ANIM_JUMP_LEFT);
		else if (direction == Voltorb::DIR_RIGHT)
			currentAnim = animSet->getAnimation(VOLTORB_ANIM_JUMP_RIGHT);
	}
	else if (state == VOLTORB_STATE_ANGRY){
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(VOLTORB_ANIM_ANGRY_LEFT);
		else if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(VOLTORB_ANIM_ANGRY_RIGHT);
	}
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

 void Voltorb::setKeyConfig (SDL_Scancode up, SDL_Scancode down, SDL_Scancode left, SDL_Scancode right, SDL_Scancode smash){
 	this->_up = up;
 	this->_down = down;
 	this->_left = left;
 	this->_right = right;
	this->_smash = smash;
 }

void Voltorb::resetState() {
	angry = false;
	angry_timer = 0;
	state = VOLTORB_STATE_IDLE;
	changeAnimation(state, true);
	onTheGround = true;
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


bool Voltorb::checkCollision(Volleyball *ball)
{

	// TODO: Modify this condition later
	if(pow(this->x - ball->x, 2) + pow(this->y - ball->y, 2) <= pow(70, 2) )
	{	// If collision just happen right before, ignore this one
		if(ball->last_collision == this) {
			return false;
		}
		ball->last_collision = this; // change the last collision object
		return true;
	}
	else
		return false;

}

void Voltorb::hitTheBall(Volleyball *ball)
{
	if(angry == true) // smash
	{
		ball->setState(Volleyball::VOLLEYBALL_STATE_SMASH);

		if( Globals::keyboard_state_array[this->_up])
		{
			ball->moveSpeed_y = -SMASHSPEED_Y;

			if( Globals::keyboard_state_array[this->_left] )
				ball->moveSpeed_x = -SMASHSPEED_X;
			else if (Globals::keyboard_state_array[this->_right] ) // up + left/right
				ball->moveSpeed_x = SMASHSPEED_X;
			else // direct up
			{
				if (this->direction == DIR_LEFT)
					ball->moveSpeed_x = -SMASHSPEED_IDLE_X;
				else if(this->direction == DIR_RIGHT)
					ball->moveSpeed_x = SMASHSPEED_IDLE_X;
				else
					;// should not fall into this condition
			}
		}
		else if( Globals::keyboard_state_array[this->_down])
		{
			ball->moveSpeed_y = SMASHSPEED_Y;

			if( Globals::keyboard_state_array[this->_left] )
				ball->moveSpeed_x = -SMASHSPEED_X;
			else if (Globals::keyboard_state_array[this->_right] ) // up + left/right
				ball->moveSpeed_x = SMASHSPEED_X;
			else // direct down
			{
				if (this->direction == DIR_LEFT)
					ball->moveSpeed_x = -SMASHSPEED_IDLE_X;
				else if(this->direction == DIR_RIGHT)
					ball->moveSpeed_x = SMASHSPEED_IDLE_X;
				else
					;// should not fall into this condition
			}
		}
		else if( Globals::keyboard_state_array[this->_left]
			|| Globals::keyboard_state_array[this->_right] ) // simple forward smash
		{
			ball->moveSpeed_y = SMASHSPEED_IDLE_Y;

			if (this->direction == DIR_LEFT)
				ball->moveSpeed_x = -SMASHSPEED_X;
			else if(this->direction == DIR_RIGHT)
				ball->moveSpeed_x = SMASHSPEED_X;
			else
				;// should not fall into this condition
		}
		else // idle smash
		{
			ball->moveSpeed_y = SMASHSPEED_IDLE_Y;

			// simple forward smash
			if (this->direction == DIR_LEFT)
				ball->moveSpeed_x = -SMASHSPEED_IDLE_X;
			else if(this->direction == DIR_RIGHT)
				ball->moveSpeed_x = SMASHSPEED_IDLE_X;
			else
				;// should not fall into this condition
		}
	}
	else // normal shot
	{
		if(ball->getState() == Volleyball::VOLLEYBALL_STATE_SMASH)
			ball->setState(Volleyball::VOLLEYBALL_STATE_NORMAL);
		// Collide
		if ( (this->x - ball->x) <= 1 && (this->x - ball->x) >= -1)
			ball->moveSpeed_x = 0.85*(ball->moveSpeed_x);
		else
			ball->moveSpeed_x = (ball->moveSpeedMax_x)
								* ( ball->x - this->x)
								/ sqrt(pow(this->x - ball->x, 2) + pow(this->y - ball->y, 2));

		if ( (this->y - ball->y) <= 1 && (this->y - ball->y) >= -1 )
			ball->moveSpeed_y = 0;
		else
			ball->moveSpeed_y = -( (ball->moveSpeedMax_y) - 0.35*(Globals::ScreenHeight - this->y) );

	}

}
