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
#include "end.h"
#include "menu.h"
#include "alapfv.h"

/////////////////////////////////////////////////////////WIN / LOSE////////////////////////////////////////////////////////
bool vege(SDL_Renderer *renderer,TTF_Font *font, Komp k,Sint16 *px, int melyik)
{
    SDL_Color sarga = {255,255,0}, piros = {255,0,0}, lila  = {205, 143, 205};
    SDL_Rect sz_end= {200,200,1520,700};
    SDL_Rect sz_also = {400,900,1120,150};
    if (abs(k.szog) <= 3 && fabs(k.v.x) < 0.5 && fabs(k.v.y)< 3 && k.x-hossz/2 > px[melyik-1] && px[melyik] > k.x+ hossz/2 && k.Fuel > 0)
        {
            ir(renderer,sz_end,font,"Victory!",sarga);
        return true;
        }
    else
    {
        ir(renderer,sz_end,font,"Game Over!",piros);
        if      (k.Fuel <= 0)
            ir(renderer,sz_also,font,"Elfogyott az üzemanyag!",lila);
        else if (k.x-hossz/2 < px[melyik-1] || px[melyik] < k.x+ hossz/2)
            ir(renderer,sz_also,font,"Nem a leszálló pályán szálltál le!",lila);
        else if (fabs(k.v.y) > 2)
            ir(renderer,sz_also,font,"Túl nagy volt a függöleges irányú sebesség!",lila);
        else if (fabs(k.v.x) > 0.5)
            ir(renderer,sz_also,font,"Túl nagy volt a vízszintes irányú sebesség!",lila);
        else
            ir(renderer,sz_also,font,"Nem volt vízszintes az űrkomp és felborult leszálláskor!",lila);
        return false;
    }
}

void lampa_motor(SDL_Renderer *renderer,TTF_Font *font)
{
    SDL_Color piros = {255,0,0}, lila  = {205, 143, 205};
    SDL_Rect sz_end= {200,200,1520,700};
    SDL_Rect sz_also = {400,900,1120,150};
    ir(renderer,sz_end,font,"Game Over!",piros);
    ir(renderer,sz_also,font,"Túl alacsonyan használtad a motort, és leégetted!",lila);
}
//////////////////////////////////////////PLAY AGAIN?//////////////////////////////////////////////////////
void press_again(SDL_Renderer *renderer,TTF_Font *font,SDL_Event *ev,SDL_Rect n, bool* again, bool*exit)
{
    if (ev->button.button == SDL_BUTTON_LEFT  &&  //Play again menünek a nyomás érzékelője
        ev->button.x >= n.x       &&
        ev->button.x <= n.x + n.w &&
        ev->button.y >= n.y       &&
        ev->button.y <= n.y + n.h)
    {
        switch(n.y)
        {
            case 200:
                *again = false;
                *exit = false;
                break;
            case 680:
                *again = false;
                *exit = true;
                break;
        }
    }
}

void play_again(SDL_Renderer *renderer, TTF_Font *font, bool* megint, bool* leave)
{
    SDL_Color piros = {255,0,0}, zold = {0,255,0}, sarga = {255,255,0},fekete = {0,0,0};
    bool again = true;
    bool exit = false;
    SDL_SetRenderDrawColor(renderer,0,0,0,255); //Play again menü
    SDL_RenderClear(renderer);
    SDL_Rect Again = {500,200,920,200};
    SDL_Rect Exit  = {500,680,920,200};
    szoveg_jo(renderer,font,Again,"Play again!",zold);
    szoveg_jo(renderer,font,Exit ,"Exit!"      ,piros);
    SDL_RenderPresent(renderer);
    while(again)
    {
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        int a,b;
        switch (ev.type)
        {
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&a, &b);
                hover_jo(renderer,font,a,b,Again,"Play again!",sarga,zold ,fekete);  //hover függvény (menu.h)
                hover_jo(renderer,font,a,b,Exit ,"Exit!"      ,sarga,piros,fekete);
                break;
            case SDL_MOUSEBUTTONDOWN:
                press_again(renderer,font,&ev,Again,&again,&exit);
                press_again(renderer,font,&ev,Exit ,&again,&exit);
                break;
        }
        if(!exit)
            *megint = true;
        else
        {
            *megint = false;
            *leave = true;
        }
    }
}
