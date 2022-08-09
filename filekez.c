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
#include "filekez.h"
#include "alapfv.h"

int hany_sor()
{
    char c;
    int sor_sz = 0;
    FILE *fp;
    fp = fopen("Adatok.txt","rt");
    for(c = getc(fp);c != EOF; c = getc(fp))
        if (c == '\n')
            sor_sz++;
    fclose(fp);
    return sor_sz;
}

adat* betolt(int *sor_sz)
{
    *sor_sz = hany_sor();
    FILE *fp;
    fp = fopen("Adatok.txt","rt");
    adat* tomb = (adat*) malloc(sizeof(adat) * *sor_sz);
    for (int i = 0; i < *sor_sz; i++)
    {
        fscanf(fp,"%s %1d:%1d%1d %2d",tomb[i].Nev, &tomb[i].perc, &tomb[i].mp10, &tomb[i].mp1, &tomb[i].Fuel);
    }
    fclose(fp);
    return tomb;

}

void file_kezeles(FILE *fp,Komp k)
{
    fp = fopen("Adatok.txt","at");
    fprintf(fp,"%s %d:%d%d %d\n",k.Name,k.Time.perc,k.Time.mp10,k.Time.mp1,(int)k.Fuel);
    fclose(fp);
}

char* nev_bevitel(SDL_Renderer *renderer,TTF_Font *font)
{
    SDL_Color sarga = {255,255,0}, fekete = {0,0,0}, zold = {0,255,0};
    char* nev = (char*) malloc(sizeof(char) * 15);
    SDL_Rect nev_doboz = {250,400,240,150};
    SDL_Rect szoveg_doboz = {500,400,920,150};
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    ir(renderer,nev_doboz,font,"Name:",sarga);
    input_text(nev,15,szoveg_doboz,fekete,zold,font,renderer);
    return nev;
}

bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter,
                SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer)
{
    char composition[SDL_TEXTEDITINGEVENT_TEXT_SIZE];
    composition[0] = '\0';
    char textandcomposition[hossz + SDL_TEXTEDITINGEVENT_TEXT_SIZE + 1];//asd
    int maxw = teglalap.w - 2;
    int maxh = teglalap.h - 2;

    dest[0] = '\0';

    bool enter = false;
    bool kilep = false;

    SDL_StartTextInput();
    while (!kilep && !enter) {
        /* doboz kirajzolasa */
        boxRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, hatter.r, hatter.g, hatter.b, 255);
        rectangleRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, 255, 0, 0, 255);
        /* szoveg kirajzolasa */
        int w;
        strcpy(textandcomposition, dest);
        strcat(textandcomposition, composition);
        if (textandcomposition[0] != '\0') {
            SDL_Surface *felirat = TTF_RenderUTF8_Blended(font, textandcomposition, szoveg);
            SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
            SDL_Rect cel = { teglalap.x, teglalap.y, felirat->w < maxw ? felirat->w : maxw, felirat->h < maxh ? felirat->h : maxh };
            SDL_RenderCopy(renderer, felirat_t, NULL, &cel);
            SDL_FreeSurface(felirat);
            SDL_DestroyTexture(felirat_t);
            w = cel.w;
        } else {
            w = 0;
        }
        /* kurzor kirajzolasa */
        if (w < maxw) {
            vlineRGBA(renderer, teglalap.x + w + 2, teglalap.y + 2, teglalap.y + teglalap.h - 3, szoveg.r, szoveg.g, szoveg.b, 192);
        }
        /* megjeleniti a képernyon az eddig rajzoltakat */
        SDL_RenderPresent(renderer);

        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            /* Kulonleges karakter */
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    int textlen = strlen(dest);
                    do {
                        if (textlen == 0) {
                            break;
                        }
                        if ((dest[textlen-1] & 0x80) == 0x00)   {
                            /* Egy bajt */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0x80) {
                            /* Bajt, egy tobb-bajtos szekvenciabol */
                            dest[textlen-1] = 0x00;
                            textlen--;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0xC0) {
                            /* Egy tobb-bajtos szekvencia elso bajtja */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                    } while(true);
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    enter = true;
                }
                break;
            case SDL_TEXTINPUT:
                if (strlen(dest) + strlen(event.text.text) < hossz) {
                    strcat(dest, event.text.text);
                }
                composition[0] = '\0';
                break;
            case SDL_TEXTEDITING:
                strcpy(composition, event.edit.text);
                break;
            case SDL_QUIT:
                SDL_PushEvent(&event);
                kilep = true;
                break;
        }
    }
    SDL_StopTextInput();
    return enter;
}
