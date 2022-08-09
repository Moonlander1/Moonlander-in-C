#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
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
#include "alapfv.h"
#include "filekez.h"

enum {P = 355};
enum {C = 495};
enum {S = 635};
///////////////////////////////////////////////////////////FŐMENU////////////////////////////////////////////////////////////

void szoveg_jo(SDL_Renderer *renderer, TTF_Font *font,SDL_Rect n,char* szoveg,SDL_Color szoveg_sz);

void hover_jo(SDL_Renderer *renderer, TTF_Font *font,int a, int b, SDL_Rect n, char* szoveg,
              SDL_Color kitolt,SDL_Color szoveg_sz,SDL_Color szel);

bool press(SDL_Renderer *renderer,SDL_Event *ev, SDL_Rect n);

bool menu(SDL_Renderer *renderer,TTF_Font *font);
////////////////////////////////////////////////CONTROLS MENU////////////////////////////////////////
void utmutato(SDL_Renderer *renderer, TTF_Font *font);
//////////////////////////////////////////////RANGLISTA MENU /////////////////////////////////////////////////////////////

void press_reset(SDL_Renderer *renderer,TTF_Font *font,SDL_Event *ev, SDL_Rect n,adat* data, int sor_sz, bool*scoreboard);

void ranglista_seged(SDL_Renderer *renderer, TTF_Font *font,int i, char* str, char* szam_str, int m);

void ranglista_kiir(SDL_Renderer *renderer, TTF_Font *font, adat* adatok,int sor_sz);

void ranglista(SDL_Renderer *renderer, TTF_Font *font);
#endif // MENU_H_INCLUDED
