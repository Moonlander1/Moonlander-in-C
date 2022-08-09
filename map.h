#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED
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

void loop(Sint16 *pontokx, Sint16 *pontoky, int i);

void random(SDL_Renderer *renderer, Sint16 *x,Sint16 *y, int melyik);
#endif // MAP_H_INCLUDED
