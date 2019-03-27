#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include "string.h"
#include "entity.h"

class Scoreboard {
public:

    int x, y;
    int fontSize;
    int texW, texH;

    TTF_Font* text_font;
    SDL_Color text_color;

    SDL_Surface * score_surface;
    SDL_Texture * score_texture;

    SDL_Rect destRect;

    list<Entity*> entities;

    Scoreboard(int x, int y, int fontSize, string font_path);
    ~Scoreboard();

    void update();
    void draw();
    void loadFont();
    void loadPlayer(Entity*);

};



#endif
