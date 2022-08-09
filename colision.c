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
#include "colision.h"

Uint32 Get_Pixel(SDL_Surface *palya, int x, int y)
{
    SDL_LockSurface(palya);
    Uint32 *p = (Uint32*) palya->pixels;
    SDL_UnlockSurface(palya);
    return p[(y * palya->w) + x];
}

void felterkepez(int *szurke, SDL_Surface *palya)
{
    Uint8 r, b, g;
    SDL_LockSurface(palya);
    for(int x = 0; x < 1920;x++)
        for(int y = 0; y < 1080;y++)
        {
            Uint32 p = Get_Pixel(palya,x,y);
            SDL_GetRGB(p,palya->format,&r,&g,&b);
            if (r == 127 && g == 127 && b == 127)
            {
                szurke[x] = y;
                break;
            }
        }
    SDL_UnlockSurface(palya);
}

void testfor(Komp k, int *szurke,SDL_Surface *palya,bool *colide)
{
    Uint8 r,g,b;
    SDL_LockSurface(palya);
    int n = k.x + hossz/2; //Ez és az abs függvény arra, szolgál, hogy a két szélén ki tudjon menni az űrkomp
    if (n > 1920)
        n = 1920;
    for(int i = abs(k.x-hossz/2); i < n;i++)
    {
        Uint32 p = Get_Pixel(palya,i,szurke[i]);
        SDL_GetRGB(p,palya->format,&r,&g,&b);
        if (r != 127 || g != 127 || b != 127)
            *colide = true;
    }
    SDL_UnlockSurface(palya);
}

void adott_magassag(Komp *k, int *szurke, bool *lampa)
{
    int n = k->x + hossz/3; //Ez és az abs függvény arra, szolgál, hogy a két szélén ki tudjon menni az űrkomp
    if (n > 1920)
        n = 1920;
    for (int i = abs(k->x-hossz/3); i < n;i++)
    {
        if (szurke[i] - (k->y-magas/2) < 115 && abs(k->szog) < 15) //Ez azért szükséges, hogy legyen egy kisebb head's UP a játékosnak
            k->lampa_eg = true;
        if (szurke[i] - (k->y-magas/2) < 100 && abs(k->szog) < 15)
            *lampa = true;
    }
}
