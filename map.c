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
#include "map.h"

void loop(Sint16 *pontokx, Sint16 *pontoky, int i) //A polygon következő csúcsának a helyét random határozza meg
{
    int n = rand() % 2;
    pontokx[i] =  pontokx[i-1] + rand() % 50 + 70;
    pontoky[i] = (pontoky[i-1] + ((int)pow(-1,n) * rand() % 150 + 150));
    if (pontoky[i] > 1000)
        pontoky[i] = ((rand() % 10)+1)*30 + 550;
    if (pontoky[i] < 600)
        pontoky[i] = ((rand() % 10)+1)*30 + 650;
}

void random(SDL_Renderer *renderer, Sint16 *x,Sint16 *y, int melyik)
{
    int csucs = 20;
    Sint16 *pontokx = (Sint16*) malloc(sizeof(int) * csucs);
    Sint16 *pontoky = (Sint16*) malloc(sizeof(int) * csucs);
    pontokx[0] =   0;
    pontoky[0] = 980;
    for (int i = 1; i < melyik; i++)
        loop(pontokx,pontoky,i);

    if (pontoky[melyik-1] < 800)                //Alacsonyan legyen a leszállópálya
        pontoky[melyik-1] += 200;
    pontokx[melyik] = pontokx[melyik-1] + 1920;   //Egyenes leszállópálya (80 hosszú)
    pontoky[melyik] = pontoky[melyik-1];

    for (int i = melyik + 1; i < csucs-2; i++)
        loop(pontokx,pontoky,i);

    pontokx[csucs] =    0; pontokx[csucs-2] = 1920; pontokx[csucs-1] = 1920; pontokx[csucs+1] =   0; //Polygon rögzített csúcsai
    pontoky[csucs] = 1080; pontoky[csucs-2] =  980; pontoky[csucs-1] = 1080; pontoky[csucs+1] = 980;

    for(int i = 0; i < csucs+1;i++)
    {
        x[i] = pontokx[i];
        y[i] = pontoky[i];
    }
    free(pontokx);
    free(pontoky);
}
