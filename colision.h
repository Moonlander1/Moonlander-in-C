#ifndef COLISION_H_INCLUDED
#define COLISION_H_INCLUDED
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

Uint32 Get_Pixel(SDL_Surface *palya, int x, int y);

void felterkepez(int *szurke, SDL_Surface *palya);

void testfor(Komp k, int *szurke,SDL_Surface *palya,bool *colide);

void adott_magassag(Komp *k, int *szurke, bool *lampa);
#endif //COLISION_H_INCLUDED
