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
#include "alapfv.h"

double Gravity = 1.2;

void urkomp_innit(Komp *k)
{
    k->x = rand() % 1000 + 10; k->y = rand() % 100 + 10; k->v.x = 0      ; k->v.y = 0      ;
    k->motor = false         ; k->jobbra = false       ; k->balra = false; k->szog = 0     ;
    k->a = 0                 ; k->Fuel = 100.0         ; k->Time.perc = 0; k->Time.mp10 = 0;
    k->Time.mp1 = 0          ; k->lampa_eg = false     ;
}

Uint32 timer(Uint32 ms, void *param)
{
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

Uint32 mp(Uint32 ms, void *param)
{
    SDL_Event ev;
    ev.type = SDL_USEREVENT + 1;
    SDL_PushEvent(&ev);
    return ms;
}

void szamol(Komp *k)
{
    double ax,ay;
    if (k->motor)           //Ha space le van nyomva
    {
        k->a     = -3;      //A komp orr irányú gyorsulása
        k->Fuel -= 0.04;    //Fogy az üzemanyag
        ay       = (Gravity + k->a) * cos(k->szog *  0.0174532925); // gyorsulas függőleges komponense
    }
    else
    {
        k->a = Gravity;     //Szabadesés
        ay   = Gravity;
    }
    if (k->jobbra)          //Jobb nyíl lenyomva
        k->szog += 1;
    if (k->balra)           //Bal  nyíl lenyomva
        k->szog -= 1;
    if (abs(k->szog) == 360)                                //Körbe lehessen forogni
        k->szog = 0;
    ax      = k->a  * sin(k->szog * -0.0174532925);         // gyorsulas vízszintes komponense
    k->v.x += ax/75;                                        //Vízszintes sebesség
    k->v.y += ay/75;                                        //Függöleges sebesség
    k->x   += k->v.x;                                       // komp x
    k->y   += k->v.y;                                       // komp y

    if      (k->x > 1920) //Ha kimegy jobb oldalt
        k->x -= 1920;
    else if (k->x < 0)    //Ha kimegy bal  oldalt
        k->x +=1920;
}

void ido_szamlal(Komp *k) //Az időt méri, a kezdő pillanattól
{
    k->Time.mp1 += 1;
    if (k->Time.mp1 == 10)
    {
        k->Time.mp1 = 0;
        k->Time.mp10 += 1;
    }
    if (k->Time.mp10 == 6)
    {
        k->Time.mp10 = 0;
        k->Time.perc += 1;
    }
}


//Ablak létrehozása
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}
// Kirajzolja a random által generált polygont
void rajzol_terkep(SDL_Renderer *renderer, Sint16 *x, Sint16 *y, int melyik)
{
    filledPolygonRGBA(renderer, x, y, 21, 127, 127, 127, 255);
    thickLineRGBA(renderer, x[melyik-1], y[melyik-1]+1, x[melyik], y[melyik]+1, 3, 255, 255, 255, 255);
}

//Kirajzolja a kompot
void rajzol_komp(SDL_Renderer *renderer,Komp *k,SDL_Texture *kep)
{
    SDL_Rect cel = {(int)k->x-hossz/2,(int)k->y-magas/2,hossz,magas}; //Az x és az y a téglalap kozeppontja
    SDL_RenderCopyEx(renderer,kep,NULL,&cel,
                     k->szog,
                     NULL,SDL_FLIP_NONE);
}

//Megjeleníti a képet a jobb felső sarokban
void lampa_rajzol(SDL_Renderer *renderer,SDL_Texture *lampa)
{
    SDL_Rect ide = {1850,130,50,50};
    SDL_RenderCopy(renderer,lampa,NULL,&ide);
}

//A komp mögé rajzolja a képeket
void animacio(SDL_Renderer *renderer,Komp k, SDL_Texture *anim,int *i)
{
    SDL_Rect cel = {(int)k.x-hossz/2+15,(int)k.y+magas/2,20,45};
    SDL_Rect forras = {0+21* *i,0,20,45};
    SDL_Point kp = {10,-magas/2};
    SDL_RenderCopyEx(renderer,anim,&forras,&cel,k.szog,&kp,SDL_FLIP_NONE);
    *i += 1;
    if (*i == 3)
        *i = 0;
}

//Kiír egy tetszőleges szöveget egy tetszőleges színnel, egy adott helyre
void ir(SDL_Renderer *renderer, SDL_Rect hova, TTF_Font *font,char *szoveg, SDL_Color szin)
{
    SDL_Surface *felirat = TTF_RenderUTF8_Solid(font, szoveg,szin);
    SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}

//Kiírja az adatokat a jobb felső sarokba
void adatok_kiir(SDL_Renderer *renderer, Komp k,TTF_Font *font, Sint16 *py, int melyik)
{
    SDL_Rect nev = {100,10,100,40};
    SDL_Rect szoveg_n = {5,10,100,40};
    SDL_Color feher = {255,255,255}, piros = {255,0,0}, zold = {0,255,0};
    ir(renderer,szoveg_n,font,"Name: ",feher);
    ir(renderer,nev,font,k.Name,feher);
    char szoveg[7][11] = {"Altitude: ","SpeedV: ","Speedh: ","Angle: ","Fuel: ","Time: ","Lamp: "};
    char szam[7][10];
    int n = 6;
    for(int i = 0; i < 7; i++)
    {
        SDL_Rect ide = {1740,10 + i * 20, 110,16};
        ir(renderer,ide,font,szoveg[i],feher);
    }
    sprintf(szam[0] ,"%0.1f"  ,py[melyik]-k.y);                      //A számadatokat string-é konvertálom(y)
    sprintf(szam[1] ,"%0.2f"  ,fabs(k.v.x*10));                      //hogy ki lehessen őket írni         (Vx)
    sprintf(szam[2] ,"%0.2f"  ,fabs(k.v.y*10));                      //                                   (Vy)
    sprintf(szam[3] ,"%d°"    ,abs(k.szog)   );                      //                                   (szög)
    sprintf(szam[4] ,"%0.0f"  ,k.Fuel        ); strcat(szam[4],"%" );//                                   (Fuel)
    sprintf(szam[5] ,"%d:%d%d",k.Time.perc,k.Time.mp10,k.Time.mp1);  //                                   (Time)
    sprintf(szam[6] ,"%s"     ,"Off"         );                      //                                   (Lampa)
    if (!k.lampa_eg)
        n = 7;
    for(int i = 0; i < n; i++)  //Piros, ha valamelyik adat nem egyezik meg a leszállási feltételeknek. Zöld, ha minden megfelel
    {
        SDL_Rect asd = {1850,10 + i *20,50,16};
        ir(renderer,asd,font,szam[i],fabs(k.v.x) > 0.5 ? piros : k.v.y > 3 ? piros : abs(k.szog) > 3 ? piros: zold);
    }
}

//Felcseréli az adatait egy tömb két szomszedos tagjának
adat* rendez_seged(adat* data,int i)
{
    char* nev_t = (char*) malloc(sizeof(char*) * 15);
    int perc_t;
    int mp10_t;
    int mp1_t ;
    int Fuel_t;
    strcpy(nev_t,data[i].Nev);
    perc_t = data[i].perc;
    mp10_t = data[i].mp10;
    mp1_t  = data[i].mp1 ;
    Fuel_t = data[i].Fuel;

    strcpy(data[i].Nev,data[i+1].Nev);
    data[i].perc = data[i+1].perc;
    data[i].mp10 = data[i+1].mp10;
    data[i].mp1  = data[i+1].mp1;
    data[i].Fuel = data[i+1].Fuel;

    strcpy(data[i+1].Nev,nev_t);
    data[i+1].perc = perc_t;
    data[i+1].mp10 = mp10_t;
    data[i+1].mp1  = mp1_t ;
    data[i+1].Fuel = Fuel_t;

    free(nev_t);
    return data;
}

//Sorba rendezi egy tömb elemeit idő aztán üzemanyag alapján
adat* rendez(adat* data, int sor_sz)
{
    for(int j = sor_sz-1; j > 0; --j)
        for( int i = 0; i < j; i++)
            {
                if(data[i].perc > data[i+1].perc)
                    rendez_seged(data,i);
                if(data[i].perc == data[i+1].perc && data[i].mp10  > data[i+1].mp10 )
                    rendez_seged(data,i);
                if(data[i].perc == data[i+1].perc && data[i].mp10 == data[i+1].mp10 && data[i].mp1 > data[i+1].mp1)
                    rendez_seged(data,i);
                if(data[i].perc == data[i+1].perc && data[i].mp10 == data[i+1].mp10 && data[i].mp1 == data[i+1].mp1 && data[i].Fuel < data[i+1].Fuel)
                    rendez_seged(data,i);
            }
    return data;
}
