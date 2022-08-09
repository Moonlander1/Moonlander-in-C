#ifndef END_H_INCLUDED
#define END_H_INCLUDED
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "structs.h"

/////////////////////////////////////////////////////////WIN / LOSE////////////////////////////////////////////////////////
bool vege(SDL_Renderer *renderer,TTF_Font *font, Komp k,Sint16 *px, int melyik);

void lampa_motor(SDL_Renderer *renderer,TTF_Font *font);
//////////////////////////////////////////PLAY AGAIN?//////////////////////////////////////////////////////
void press_again(SDL_Renderer *renderer,TTF_Font *font,SDL_Event *ev,SDL_Rect n, bool* again, bool*exit);

void play_again(SDL_Renderer *renderer, TTF_Font *font, bool* megint, bool* leave);
#endif // END_H_INCLUDED
