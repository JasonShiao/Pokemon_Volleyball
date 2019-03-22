#include "wall.h"


/****************************************************************
 *              Static Data Member Initialization               *
 ****************************************************************/
const string Wall::NORMAL_WALL = "normal";

const int Wall::WALL_STATE_NORNAL = 0;


/****************************************************************
 *                      Method Definition                       *
 ****************************************************************/
Wall::Wall(AnimationSet *animSet, int x, int y, int w, int h){

	this->animSet = animSet;
	this->type = "wall";

	//setup default voltorb values

	moveSpeed_x = 0; moveSpeed_y = 0;
	moveSpeedMax = 0;

    region.x = x;
    region.y = y;
    region.w = w;
    region.h = h;

	//changeAnimation(WALL_STATE_NORMAL, true);

}

void Wall::draw(){
	//if (currentFrame != NULL && active){
	//	currentFrame->Draw(animSet->spriteSheet, x, y);
	//}
	//draw collsionBox
	//if (solid && Globals::debugging){
		//sets the current drawing colour (Doesn't affect textures and what not)
		//SDL_SetRenderDrawColor(Globals::renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
		//SDL_RenderDrawRect(Globals::renderer, &lastCollisionBox);

		//SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		//SDL_RenderDrawRect(Globals::renderer, &collisionBox);

		//SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
		//SDL_RenderDrawRect(Globals::renderer, &hitBox);
	//}
}

void Wall::updateMovement() {
    // wall never move
}

void Wall::move(){
	// wall never move
}

void Wall::update(){

	/*
		inertial movement changes (user input, collision, gravity),
		also change animation here
	*/
	//updateMovement();

	//move(); // inertial movement

	//updateAnimation();
}

void Wall::changeAnimation(int newState, bool resetFrameToBeginning){
	/*state = newState;

	if (state == VOLTORB_STATE_IDLE){
		if (direction == Entity::DIR_LEFT)
			currentAnim = animSet->getAnimation(VOLTORB_ANIM_IDLE_LEFT);
		else if (direction == Entity::DIR_RIGHT)
			currentAnim = animSet->getAnimation(VOLTORB_ANIM_IDLE_RIGHT);
	}

	if (resetFrameToBeginning)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
    */
}

void Wall::updateAnimation(){ // rolling frames in an animation
	//if (currentFrame == NULL || currentAnim == NULL)
	//	return; //cant do much with animations without pointers pointing to them :S


	//frameTimer += TimeController::timeController.dT;
	//time to change frames :D
	//if (frameTimer >= currentFrame->duration)
	//{
	//	currentFrame = currentAnim->getNextFrame(currentFrame);
	//	frameTimer = 0;
	//}
}
