#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
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

void urkomp_innit(Komp *k);

Uint32 timer(Uint32 ms, void *param);

Uint32 mp(Uint32 ms, void *param);

void szamol(Komp *k);

void ido_szamlal(Komp *k);

void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);

void rajzol_terkep(SDL_Renderer *renderer, Sint16 *x, Sint16 *y, int melyik);

void rajzol_komp(SDL_Renderer *renderer,Komp *k,SDL_Texture *kep);

void lampa_rajzol(SDL_Renderer *renderer,SDL_Texture *lampa);

void animacio(SDL_Renderer *renderer,Komp k, SDL_Texture *anim,int *i);

void ir(SDL_Renderer *renderer, SDL_Rect hova, TTF_Font *font,char *szoveg, SDL_Color szin);

void adatok_kiir(SDL_Renderer *renderer, Komp k,TTF_Font *font, Sint16 *py, int melyik);

adat* rendez_seged(adat* data,int i);

adat* rendez(adat* data, int sor_sz);
#endif //HEADER_H_INCLUDED
