#ifndef FILEKEZ_H_INCLUDED
#define FILEKEZ_H_INCLUDED
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

int hany_sor();

adat* betolt(int *sor_sz);

void file_kezeles(FILE *fp,Komp k);

char* nev_bevitel(SDL_Renderer *renderer,TTF_Font *font);

bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter,
                SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer);
#endif //FILEKEZ_H_INCLUDED
