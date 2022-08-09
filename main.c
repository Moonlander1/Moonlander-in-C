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
#include "debugmalloc.h"
#include "alapfv.h"
#include "menu.h"
#include "map.h"
#include "colision.h"
#include "end.h"
#include "filekez.h"
#include "structs.h"

 int main(int argc, char *argv[])
 {
    /* ablak letrehozasa */
    SDL_Window   *window;
    SDL_Renderer *renderer;
    sdl_init("Moonlander", 1920, 1080, &window, &renderer);
    #ifdef __WIN32__
        freopen("CON", "w", stdout);
        freopen("CON", "w", stderr);
    #endif
    //Pályához szükséges változók
    srand(time(0));
    Sint16 px[21];
    Sint16 py[21];
    int melyik = (rand() % 15) + 1;
    int szurke[1920];
    random(renderer,px,py,melyik); //Pálya felrajzolása
    //Boolok
    bool run    = true;
    bool colide = false;
    bool quit   = false;
    bool lampa  = false;
    bool megint = false;
    bool exit   = false;
    int c = 0;

    //Képek betöltése
    SDL_Surface *palyaS = NULL;
    SDL_Texture *komp  = IMG_LoadTexture(renderer,"rocket.png" );
    SDL_Texture *lamp  = IMG_LoadTexture(renderer,"lampa.png");
    SDL_Texture *anim  = IMG_LoadTexture(renderer,"anim.png");
    //Betűstílusok betöltése
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("BASKVILL.ttf", 16);
    TTF_Font *font_n = TTF_OpenFont("BASKVILL.ttf",150);

    run = menu(renderer,font);

    FILE *fp = NULL;

    SDL_TimerID idozit = SDL_AddTimer(100/7.5,timer,NULL);
    SDL_TimerID every_sec = SDL_AddTimer(1000,mp,NULL);

    Komp urkomp;
    urkomp_innit(&urkomp);
    if(run)
    {
        urkomp.Name = nev_bevitel(renderer,font_n);
        rajzol_terkep(renderer,px,py,melyik);
        SDL_RenderPresent(renderer);
        palyaS = SDL_GetWindowSurface(window);
        felterkepez(szurke,palyaS);
    }

    while (run)
    {
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch(event.type)
        {
            case SDL_USEREVENT:
                if (urkomp.Fuel <= 0) //Ha nicns üzemanyag
                {
                    vege(renderer,font,urkomp,px,melyik);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(3000);
                    play_again(renderer,font,&megint,&exit);
                    run = false;
                    break;
                }
                if (lampa && urkomp.motor) //Ha tul közel kerül a földhöz és használja a motort
                {
                    lampa_motor(renderer,font);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(3000);
                    play_again(renderer,font,&megint,&exit);
                    run = false;
                    break;
                }
                if (!colide) //Amíg nincs ütközés
                {
                    lampa = false;
                    urkomp.lampa_eg = false;
                    szamol (&urkomp);
                    adott_magassag(&urkomp,szurke,&lampa);
                    //Lepucolja a pályát
                    SDL_SetRenderDrawColor(renderer,0,0,0,255);
                    SDL_RenderClear(renderer);
                    //kirajzolja amiket kell
                    if(urkomp.lampa_eg)
                        lampa_rajzol(renderer,lamp);
                    adatok_kiir  (renderer,urkomp,font,py,melyik);
                    if(urkomp.motor)
                        animacio (renderer,urkomp,anim,&c);
                    rajzol_terkep(renderer,px,py,melyik);
                    rajzol_komp  (renderer,&urkomp,komp);
                    SDL_RenderPresent(renderer);
                    //Megnézi, hogy volt-e ütközés
                    testfor(urkomp,szurke,palyaS,&colide);
                }
                else
                    run = false;
                break;

            case SDL_USEREVENT+1:
                ido_szamlal(&urkomp);
                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                    case SDLK_SPACE: urkomp.motor  = false; break;
                    case SDLK_RIGHT: urkomp.jobbra = false; break;
                    case SDLK_LEFT : urkomp.balra  = false; break;
                } break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_SPACE : urkomp.motor      = true;  break;
                    case SDLK_RIGHT : urkomp.jobbra     = true;  break;
                    case SDLK_LEFT  : urkomp.balra      = true;  break;
                    case SDLK_ESCAPE: run = false; quit = true;  break;
                } break;

            case SDL_QUIT: quit = true; run = false; break;
        }
        if (!quit && colide) //Ha nem kilépett és ütközött
        {
            bool win = vege(renderer,font,urkomp,px,melyik);
            SDL_RenderPresent(renderer);
            SDL_Delay(3000);
            if(win)
                file_kezeles(fp,urkomp);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
            play_again(renderer,font,&megint,&exit); //Újra?
        }
        if(exit)
            SDL_Quit();
        if(megint) //UJ JATEK
        {
            run = menu(renderer,font);
            colide = false;
            urkomp_innit(&urkomp);
            melyik = (rand() % 15) + 1;
            random(renderer,px,py,melyik);
            rajzol_terkep(renderer,px,py,melyik);
            SDL_RenderPresent(renderer);
            palyaS = SDL_GetWindowSurface(window);
            felterkepez(szurke,palyaS);
            megint = false;
        }
    }
    SDL_DestroyTexture(komp); //Felszabadítások
    SDL_DestroyTexture(lamp);
    SDL_RemoveTimer(idozit);
    SDL_RemoveTimer(every_sec);
    TTF_CloseFont(font);
    /*if(strlen(urkomp.Name) > 1)
        free(urkomp.Name);*/

    SDL_Quit();
    return 0;
}

