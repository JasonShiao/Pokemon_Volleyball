#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "voltorb.h"
#include "keyboardInput.h"
#include "drawing_functions.h"

class Game{
public:
	AnimationSet* voltorbAnimSet;
	//AnimationSet* wallAnimSet;

	SDL_Texture* backgroundImage;

	//Hero *hero;
    Voltorb voltorb_1;
	KeyboardInput voltorbInput;

	//list<Entity*> walls;

	Game();
	~Game();

	void update();
	void draw();

};


#endif
