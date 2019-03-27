#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include "cleanup.h"
#include "res_path.h"
#include "drawing_functions.h"
#include "SDL2/SDL_mixer.h"
#include "globals.h"
#include "frame.h"
#include "animationSet.h"
#include "timeController.h"
#include "voltorb.h"
#include "volleyball.h"
#include "wall.h"
#include <SDL2/SDL_ttf.h>
#include "scoreboard.h"

using namespace std;


int main() {
    cout << "Hello World" << endl;

    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        cout << "Error initializing SDL" << endl;
        return 1;
    }

        // Setup window
    SDL_Window *window = SDL_CreateWindow("Pokemon Volley",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          Globals::ScreenWidth*Globals::ScreenScale,
                                          Globals::ScreenHeight*Globals::ScreenScale,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        SDL_Quit();
        cout << "Error window create" << endl;
        return 1;
    }

    //renderer
    Globals::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (Globals::renderer == nullptr) {
        cleanup(window);
        SDL_Quit();
        cout << "renderer error" << endl;
        return 1;
    }

    SDL_RenderSetLogicalSize(Globals::renderer,
                         Globals::ScreenWidth,
                         Globals::ScreenHeight);

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_Quit();
        cout << "SDL imsage did not initialize" << endl;
        return 1;
    }
    if (TTF_Init() != 0) {
        SDL_Quit();
        cout << "SDL ttf did not initialize" << endl;
        return 1;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        SDL_Quit();
        cout << "Mixer did not initialize" << endl;
        return 1;
    }


    /* TTF for scoreboard */
    /*TTF_Font * scoreboard_font = TTF_OpenFont("./res/open_sans/OpenSans-ExtraBold.ttf", 40);
    if(scoreboard_font == NULL)
    {
        cout << "Font not opened!" << endl;
        return 1;
    }*/


    /***************************************************************************
    *                         USER CODE                                       *
    ***************************************************************************/
    string resPath = getResourcePath();
    // Background image
    SDL_Texture* texture = loadTexture(resPath + "map5.png", Globals::renderer);


	list<DataGroupType> dataGroupTypes;

	DataGroupType colBoxType;
	colBoxType.groupName = "collisionBox";
	colBoxType.dataType = DataGroupType::DATATYPE_BOX;

	dataGroupTypes.push_back(colBoxType);

    //list<DataGroupType> dataGroupTypes2;

    //DataGroupType colBoxType2;
    //colBoxType2.groupName = "collisionBox";
    //colBoxType2.dataType = DataGroupType::DATATYPE_BOX;

    //dataGroupTypes2.push_back(colBoxType);

    /*  Animation Set */
    AnimationSet* voltorbAnimSet;
    voltorbAnimSet = new AnimationSet();
    voltorbAnimSet->loadAnimationSet("voltorb.fdset", dataGroupTypes);

    AnimationSet* volleyballAnimSet;
    volleyballAnimSet = new AnimationSet();
    volleyballAnimSet->loadAnimationSet("volleyball.fdset", dataGroupTypes);

    /* Entity instances */
    Voltorb *voltorb_1 = new Voltorb(voltorbAnimSet, Voltorb::DIR_LEFT);
    Voltorb *voltorb_2 = new Voltorb(voltorbAnimSet, Voltorb::DIR_RIGHT);

    Volleyball *volleyball = new Volleyball(volleyballAnimSet, 150, 50);

    Wall *middle_net = new Wall(NULL, Globals::ScreenWidth/2 - 2, Globals::ScreenHeight - 150, 4, 150);

    Entity::entities.push_back(voltorb_1);
    Entity::entities.push_back(voltorb_2);
    Entity::entities.push_back(volleyball);
    Entity::entities.push_back(middle_net);


    /*******  Scoreboard  *******/
    Scoreboard* scoreboard = new Scoreboard(Globals::ScreenWidth/2, 30, 30, "./res/open_sans/OpenSans-ExtraBold.ttf");



    /* Game loop */
    SDL_Event e;
    bool quit = false;
    bool screen_changed = false;

    quit = false;


    while(!quit){ // Quit the game, break this loop

        // ======= Menu =======
        // ====================
        // Menu();

        // a new set
        voltorb_1->resetPoint();
        voltorb_2->resetPoint();

        bool AnyOneWon = false;

        while(!quit){ // If a set (game) is finished, break this loop

            //cout << "Check point 0" << endl;

            voltorb_1->resetPosition();
            voltorb_2->resetPosition();
            volleyball->reset();

            SDL_Delay(1500); // Delay 2000 ms
            TimeController::timeController.reset();

            bool AnyOneScored = false;
            // a new point

            /* Score Panel */
            //string score_str = to_string(voltorb_2->points) + "   " + to_string(voltorb_1->points);
            //SDL_Surface * score_surface = TTF_RenderText_Solid(scoreboard_font, score_str.c_str(), scoreboard_color);
            //SDL_Texture * score_texture = SDL_CreateTextureFromSurface(Globals::renderer, score_surface);
            //SDL_QueryTexture(score_texture, NULL, NULL, &texW, &texH);
            //SDL_Rect dstrect = { 280, 0, texW, texH };
            scoreboard->update();

            while(!quit){ // If a point is finished, break this loop

                while(SDL_PollEvent(&e)){
                    if(e.type == SDL_QUIT){
                        quit = true;
                    }
                }

                TimeController::timeController.updateTime();

                //cout << "Check point 1" << endl;

                voltorb_1->update();
                voltorb_2->update();
                volleyball->update();

                //cout << "Check point 3" << endl;

                // Render and Draw the screen
                //if (screen_changed == true) {
                SDL_RenderClear(Globals::renderer);

                renderTexture(texture, Globals::renderer, 0, 0); // background image

                voltorb_1->draw();
                voltorb_2->draw();
                volleyball->draw();

                //DL_RenderCopy(Globals::renderer, score_texture, NULL, &dstrect);
                 scoreboard->draw();

                SDL_RenderPresent(Globals::renderer);
                    //screen_changed = false;
                //}

                //SDL_Delay(5); // Delay n ms


                // Determine whether someone wins the point and break the while loop
                if(volleyball->isLanding()) {
                    for (list<Voltorb*>::iterator voltorb = Voltorb::voltorbs.begin(); voltorb != Voltorb::voltorbs.end(); voltorb++)
                    {
                        if(volleyball->x > (*voltorb)->getScoringArea_Left() && volleyball->x < (*voltorb)->getScoringArea_Right())
                        {
                            (*voltorb)->points++;
                            AnyOneScored = true;
                        }
                    }

                    if(AnyOneScored == true)
                        break;
                }

            }

            /* Clear Score Texture */
            //SDL_DestroyTexture(score_texture);
            //SDL_FreeSurface(score_surface);

            // Determine whether someone wins the set and break the while loop
            for (list<Voltorb*>::iterator voltorb = Voltorb::voltorbs.begin(); voltorb != Voltorb::voltorbs.end(); voltorb++)
            {
                cout << "Player " << (*voltorb)->index + 1 << " gets " <<  (*voltorb)->points << " points" << " | ";
                if ((*voltorb)->points >= 15)
                {
                    cout << endl;
                    cout << "====== Player " << (*voltorb)->index << " wins! =====" << endl;
                    AnyOneWon = true;
                }
            }
            cout << endl;

            if(AnyOneWon == true)
                break;
            // Winner/Loser animation

        }

    }




    delete voltorbAnimSet;
    delete volleyballAnimSet;

    for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
    {
        delete *entity;
    }

    delete scoreboard;

    cleanup(Globals::renderer);
    cleanup(window);
    cleanup(texture);

    //TTF_CloseFont(score_panel_font);
    /***************************************************************************
    *                          END OF USER CODE                               *
    ***************************************************************************/


    SDL_Quit();

    cout << "Goodbye" << endl;

    return 0;
}
