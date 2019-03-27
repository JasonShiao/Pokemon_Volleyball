#include "scoreboard.h"

#include "string.h"
#include "globals.h"
#include "entity.h"
#include "voltorb.h"

Scoreboard::Scoreboard(int x, int y, int fontSize, string font_path)
{
    this->x = x;
    this->y = y;
    this->fontSize = fontSize;
    this->texW = 0;
    this->texH = 0;

    score_surface = NULL;
    score_texture = NULL;

    text_font = TTF_OpenFont(font_path.c_str(), fontSize);
    if(text_font == NULL)
    {
        cout << "Font not opened!" << endl;
        return;
    }

    text_color.r = 240;
    text_color.g = 80;
    text_color.b = 80;

}

Scoreboard::~Scoreboard()
{
    if(this->score_texture != NULL)
        SDL_DestroyTexture(this->score_texture);
    if(this->score_surface != NULL)
        SDL_FreeSurface(this->score_surface);
    if(this->text_font != NULL)
        TTF_CloseFont(this->text_font);
}

void Scoreboard::update()
{
    string score_str = "";
    bool even = false;
    for (list<Entity*>::reverse_iterator entity = Entity::entities.rbegin(); entity != Entity::entities.rend(); entity++)
    {
        // Collide with middle_net (wall)
        if((*entity)->type.compare("voltorb") == 0) {
            score_str += to_string(((Voltorb*)(*entity))->points);
            if(!even) {
                score_str.append(70u,' ');
                even = true;
            } else {
                score_str += "\n";
                even = false;
            }
            //.append(score_str.begin(), 20 - s.length(), 'X');
        }
    }
    //string test_str = "adsfajlsd;flksdf   adflkjas;";
    if(this->text_font != NULL) {
        if(this->score_surface != NULL) {
            SDL_FreeSurface(this->score_surface);
            this->score_surface = NULL;
        }
        //this->score_surface = TTF_RenderText_Solid(this->text_font, score_str.c_str(), this->text_color);
        //this->score_surface =TTF_RenderText_Blended(this->text_font, score_str.c_str(), this->text_color);
        this->score_surface = TTF_RenderText_Blended_Wrapped(this->text_font, score_str.c_str(), this->text_color, Globals::ScreenWidth);
    } else {
        return;
    }
    if(this->score_surface != NULL) {
        if(this->score_texture != NULL) {
            SDL_DestroyTexture(this->score_texture);
            this->score_texture = NULL;
        }
        this->score_texture = SDL_CreateTextureFromSurface(Globals::renderer, this->score_surface);
    } else {
        return;
    }

    if(this->score_texture != NULL) {
        SDL_QueryTexture(this->score_texture, NULL, NULL, &(this->texW), &(this->texH));
        destRect.x = this->x - (this->texW)/2 + 10;
        destRect.y = this->y - (this->texH)/2;
        destRect.w = this->texW;
        destRect.h = this->texH;
    }

    // Render some text in blended black to a new surface
    // then blit to the upper left of the screen
    // then free the text surface
    //SDL_Surface *screen;
    //SDL_Color color={0,0,0};
    //SDL_Surface *text_surface;
    //if(!(text_surface=TTF_RenderText_Blended(font,"Hello World!",color))) {
        //handle error here, perhaps print TTF_GetError at least
    //} else {
        //SDL_BlitSurface(text_surface,NULL,screen,NULL);
        //perhaps we can reuse it, but I assume not for simplicity.
        //SDL_FreeSurface(text_surface);
    //}




}

void Scoreboard::draw()
{
    if(this->score_texture != NULL)
        SDL_RenderCopy(Globals::renderer, this->score_texture, NULL, &(this->destRect));
}


void Scoreboard::loadFont()
{



}

void Scoreboard::loadPlayer(Entity* newPlayer)
{


}
