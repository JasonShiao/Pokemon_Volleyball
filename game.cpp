#include "game.h"

Game::Game(){
	string resPath = getResourcePath();
	backgroundImage = loadTexture(resPath + "map.png", Globals::renderer);

	voltorbAnimSet = new AnimationSet();
	//voltorbAnimSet->loadAnimationSet("udemyCyborg.fdset", dataGroupTypes, true, 0, true);

	//wallAnimSet = new AnimationSet();
	//wallAnimSet->loadAnimationSet("wall.fdset", dataGroupTypes);

	//build hero entity
	voltorb_1 = new Voltorb(voltorbAnimSet, Entity::DIR_LEFT);
	voltorb_1->invincibleTimer = 0;
	voltorb_1->x = Globals::ScreenWidth / 2;
	voltorb_1->y = Globals::ScreenHeight / 2;
	//tells keyboard input to manage hero
	voltorbInput.voltorb = voltorb_1;
	//add hero to the entity list
	Entity::entities.push_back(voltorb_1);

	int tileSize = 32;
	//build all the walls for this game
	//first. build walls on top and bottom of screen
	/*for (int i = 0; i < Globals::ScreenWidth / tileSize; i++){
		//fills in top row
		Wall* newWall = new Wall(wallAnimSet);
		newWall->x = i * tileSize + tileSize/2;
		newWall->y = tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);

		//re-using pointer to create bottom row
		newWall = new Wall(wallAnimSet);
		newWall->x = i * tileSize + tileSize / 2;
		newWall->y = Globals::ScreenHeight - tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);
	}
	//now the sides
	for (int i = 1; i < Globals::ScreenHeight / tileSize - 1; i++){
		//fills in left column
		Wall* newWall = new Wall(wallAnimSet);
		newWall->x = tileSize / 2;
		newWall->y = i*tileSize + tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);

		//re-using pointer to create right column
		newWall = new Wall(wallAnimSet);
		newWall->x = Globals::ScreenWidth - tileSize / 2;
		newWall->y = i*tileSize + tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);
	}*/

}

Game::~Game(){
	cleanup(backgroundImage);

	Entity::removeAllFromList(&Entity::entities, false);

	delete heroAnimSet;
	delete wallAnimSet;

	delete hero;

	//delete all of the wall entities
	Entity::removeAllFromList(&walls, true);
}

void Game::update(){
	bool quit = false;

	SDL_Event e;
	//setup my time controller before the game starts
	TimeController::timeController.reset();
	//game loop!
	while (!quit){
		TimeController::timeController.updateTime();

		Entity::removeInactiveEntitiesFromList(&Entity::entities, false);

		//check for any events that might have happened
		while (SDL_PollEvent(&e)){
			//close the window
			if (e.type == SDL_QUIT)
				quit = true;
			//if keydown event
			if (e.type == SDL_KEYDOWN){
				//switch case on which button was pressed
				switch (e.key.keysym.scancode){
				case SDL_SCANCODE_ESCAPE: //esc key
					quit = true;
					break;
				case SDL_SCANCODE_SPACE:
					hero->revive();
					break;

				}
			}
			heroInput.update(&e);
		}

		//update all entities
		for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++){
			//remember how awesome polymorphism is?
			//update all entities in game world at once
			(*entity)->update();
		}

		//draw all entities
		draw();
	}

}
void Game::draw(){
	//clear the screen
	SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(Globals::renderer);

	//draw the background
	renderTexture(backgroundImage, Globals::renderer, 0, 0);

	//sort all entities based on y(depth)
	Entity::entities.sort(Entity::EntityCompare);
	//draw all of the entities
	for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++){
		(*entity)->draw();
	}

	//after we're done drawing/rendering, show it to the screen
	SDL_RenderPresent(Globals::renderer);
}