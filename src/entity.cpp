#include "entity.h"


list<Entity*> Entity::entities;

Entity::Entity()
{

	type = "entity";
	state = 0;

	x = 0;
	y = 0;
	solid = true;
	active = true;

	//bool moving;
    moveSpeed_x = 0;
    moveSpeed_y = 0;
    moveSpeedMax_x = 0;
	moveSpeedMax_y = 0;

	//SDL_Rect collisionBox;
	//SDL_Rect lastCollisionBox; // where our collisionbox was last
	//int collisionBoxW, collisionBoxH; // our default collisionBox size
	//float collisionBoxYOffset;

	animSet = NULL;
	currentAnim = NULL;
	currentFrame = NULL;
	frameTimer = 0;

}


Entity::~Entity()
{
	// nothing
}

void Entity::update() { ; }//override me to do something useful

void Entity::draw() {
	//override me if you want something else or more specific to happen
	//draws current frame
	if (currentFrame != NULL && active){
		currentFrame->Draw(animSet->spriteSheet, x, y);
	}
	//draw collsionBox
	/*if (solid && Globals::debugging){
		//sets the current drawing colour (Doesn't affect textures and what not)
		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &collisionBox);
	}*/

}

void Entity::move(){
	// inertial motion
}
void Entity::updateMovement(){
	// if moving angle or speed changes (collide) or user input
}

bool Entity::EntityCompare(const Entity* const &a, const Entity * const &b){
	if (a != 0 && b != 0)
	{
		return (a->y < b->y);
	}
	else
	{
		return false;
	}
}
