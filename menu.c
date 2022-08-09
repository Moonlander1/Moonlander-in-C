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
#include "menu.h"

///////////////////////////////////////////////////////////FŐMENU////////////////////////////////////////////////////////////
//Kiírja egy adott szöveget egy adott Rect-velsejébe
void szoveg_jo(SDL_Renderer *renderer, TTF_Font *font,SDL_Rect n,char* szoveg,SDL_Color szoveg_sz)
{
    n.x += 10; n.w -= 20;
    n.y += 10; n.h -= 20;
    ir(renderer,n,font,szoveg,szoveg_sz);
}

//Egy adott Rect-et kiszínez, ha az egér benne van van
void hover_jo(SDL_Renderer *renderer, TTF_Font *font,int a, int b, SDL_Rect n, char* szoveg,
              SDL_Color kitolt,SDL_Color szoveg_sz,SDL_Color szel)
{
    if (a >= n.x && a <= n.x + n.w && b >= n.y && b <= n.y + n.h)
    {
        SDL_SetRenderDrawColor(renderer, kitolt.r, kitolt.g, kitolt.b, 255);
        SDL_RenderFillRect(renderer,&n);
        szoveg_jo(renderer,font,n,szoveg,szoveg_sz);
        SDL_RenderPresent(renderer);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
        SDL_RenderFillRect(renderer,&n);                                            // ujra fekete
        szoveg_jo(renderer,font,n,szoveg,szoveg_sz);
        rectangleRGBA(renderer,n.x,n.y,n.x+n.w,n.y+n.h,szel.r,szel.g,szel.b,255);   //széle
        SDL_RenderPresent(renderer);
    }
}

//Nyomást érzékel egy Rect-en belül
bool press(SDL_Renderer *renderer,SDL_Event *ev, SDL_Rect n)
{
    if (ev->button.button == SDL_BUTTON_LEFT  &&
        ev->button.x      >= n.x       &&
        ev->button.x      <= n.x + n.w &&
        ev->button.y      >= n.y       &&
        ev->button.y      <= n.y + n.h)
    {
        if (n.y == P) // Ha play
            return false;
        if (n.y == C) // Ha controls
            return true;
        if (n.y == S) // Ha scoreboard
            return true;
    }
    return false;
}

//A menü függvénye
bool menu(SDL_Renderer *renderer,TTF_Font *font)
{
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Color sarga = {255,255,0}, zold = {0,255,0}, piros = {255,0,0};
    bool menu    = true;
    bool con     = false;
    bool score_b = false;
    int a;
    int b;

    SDL_Rect Play       = {790, P, 340, 90};
    SDL_Rect Controls   = {790, C, 340, 90};
    SDL_Rect Scoreboard = {790, S, 340, 90};
    szoveg_jo(renderer,font,Play      ,"Play"      ,sarga);                   //Felrajzolom a szöveget
    szoveg_jo(renderer,font,Controls  ,"Controls"  ,sarga);
    szoveg_jo(renderer,font,Scoreboard,"Scoreboard",sarga);
    for(int i = 0; i < 3; i++)
        rectangleRGBA(renderer,790,P+i*140,790+340,(P+i*140)+90,255,0,0,255); //A Szövegdobozok
    rectangleRGBA(renderer, 760, 305, 1160, 775, 255, 0, 0, 255);             //nagy teglalap
    SDL_RenderPresent(renderer);

    while (menu)
    {
        SDL_Event ev;
        SDL_WaitEvent(&ev);

        switch(ev.type)
        {
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&a, &b);
                hover_jo(renderer,font,a,b,Play      ,"Play"      ,zold,sarga,piros);   //Play
                hover_jo(renderer,font,a,b,Controls  ,"Controls"  ,zold,sarga,piros);   //Controls
                hover_jo(renderer,font,a,b,Scoreboard,"Scoreboard",zold,sarga,piros);   //Scoreboard
                break;

            case SDL_MOUSEBUTTONDOWN:
                menu    = press(renderer, &ev, Play      ); //Play
                con     = press(renderer, &ev, Controls  ); //Controls
                score_b = press(renderer, &ev, Scoreboard); //Scoreboard
                break;

            case SDL_QUIT:
                menu = false;
                return false;
                break;

            case SDL_KEYDOWN:
                if (ev.key.keysym.sym == SDLK_ESCAPE)
                {
                    menu = false;
                    return false;
                } break;
        }
        if (con)
        {
            menu = false;
            utmutato(renderer,font);
        }
        else if(score_b)
        {
            menu = false;
            ranglista(renderer,font);
        }
    }
    return true;
}
////////////////////////////////////////////////CONTROLS MENU////////////////////////////////////////
void utmutato(SDL_Renderer *renderer, TTF_Font *font)
{
    bool controls = true;
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_Color piros = {255,0,0}, zold = {0,255,0},lila  = {205, 143, 205};
    while(controls)
    {
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        char d_szoveg[3][10] = {"Space" , "→" , "←"};
        char l_szoveg[3][24] = {"Thruster on/off switch","Rotate spacecraft right","Rotate spacecraft left"};
        for(int i = 0; i < 3; i++)
            {
                rectangleRGBA(renderer,200,200+i*200,400,300+i*200,0,0,255,255);
                SDL_Rect szoveg_doboz = {210,215+200*i,180,80};
                SDL_Rect szoveg       = {450,210+200*i,800,80};
                ir(renderer,szoveg_doboz,font,d_szoveg[i],piros);
                ir(renderer,szoveg,font,l_szoveg[i],zold);
            }
        SDL_Rect esq = {200,800,1100,100};
        ir(renderer,esq,font,"Press \"escape\" to go back to the menu",lila);
        SDL_RenderPresent(renderer);
        if (ev.type == SDL_KEYDOWN)
            if (ev.key.keysym.sym == SDLK_ESCAPE)
                controls = false;
    }
    menu(renderer,font);
}
//////////////////////////////////////////////RANGLISTA MENU /////////////////////////////////////////////////////////////

void press_reset(SDL_Renderer *renderer,TTF_Font *font,SDL_Event *ev, SDL_Rect n,adat* data, int sor_sz, bool*scoreboard)
{
    if (ev->button.button == SDL_BUTTON_LEFT  &&
        ev->button.x      >= n.x       &&
        ev->button.x      <= n.x + n.w &&
        ev->button.y      >= n.y       &&
        ev->button.y      <= n.y + n.h)
    {
        FILE *fp = fopen("Adatok.txt","wt");
        fprintf(fp," ");
        fclose(fp);
        ranglista_kiir(renderer,font,data,sor_sz);
        *scoreboard = false;
    }
}
void ranglista_seged(SDL_Renderer *renderer, TTF_Font *font,int i, char* str, char* szam_str, int m)
{
    SDL_Color zold = {0,255,0}, piros = {255,0,0};
    SDL_Rect hova = {300+650*m,200+i*80,550,50};
    SDL_Rect szam = {260+650*m,200+i*80,30,50};
    ir(renderer,hova,font,str,zold);
    ir(renderer,szam,font,szam_str,piros);
}

void ranglista_kiir(SDL_Renderer *renderer, TTF_Font *font, adat* adatok,int sor_sz)
{
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_Rect esq = {10,10,1100,100};
    SDL_Color piros = {255,0,0};
    ir(renderer,esq,font,"Press \"escape\" to go back to the menu",piros);
    for(int i = 0; i < sor_sz; i++ )
    {
        char* str      = (char*) malloc(sizeof(char) *36);
        char* szam_str = (char*) malloc(sizeof(char) * 12);
        sprintf(str,"Name: %s %dm%d%ds Fuel: %d",adatok[i].Nev,adatok[i].perc, adatok[i].mp10, adatok[i].mp1,adatok[i].Fuel);
        strcat(str,"%");
        sprintf(szam_str,"%d.",i+1);
        if(i < 10)
            ranglista_seged(renderer,font,i  , str, szam_str,0);
        else
            ranglista_seged(renderer,font,i-9, str, szam_str,1);
        free(str);
        free(szam_str);
    }
    SDL_RenderPresent(renderer);
}

void ranglista(SDL_Renderer *renderer, TTF_Font *font)
{
    SDL_Color lila  = {205, 143, 205},feher = {255,255,255}, fekete = {0,0,0};
    bool scoreboard = true;
    int sor_sz;
    adat *adatok = betolt(&sor_sz);
    adatok = rendez(adatok,sor_sz);
    ranglista_kiir(renderer,font,adatok,sor_sz);
    SDL_Rect reset = {1620,0,300,50};
    szoveg_jo(renderer,font,reset,"Reset scores",lila);
    int a,b;

    while(scoreboard)
    {
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        switch(ev.type)
        {
            case SDL_KEYDOWN:
                if (ev.key.keysym.sym == SDLK_ESCAPE)
                    scoreboard = false;
                break;
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&a, &b);
                hover_jo(renderer,font,a,b,reset,"Reset scores",feher,lila,fekete);
                break;
            case SDL_MOUSEBUTTONDOWN:
                press_reset(renderer,font,&ev,reset,adatok,sor_sz,&scoreboard);
        }

    }
    free(adatok);
    menu(renderer,font);
}
