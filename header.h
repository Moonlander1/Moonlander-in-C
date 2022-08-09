#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#endif // HEADER_H_INCLUDED

typedef struct adat
{
    char Nev[15];
    int perc;
    int mp10;
    int mp1;
    int Fuel;

}adat;

typedef struct ido
{
    int perc;
    int mp10;
    int mp1;
} ido;

typedef struct Sebesseg
{
    double x;
    double y;
} Sebesseg;


typedef struct Komp
{
    char *Name;
    double x;
    double y;
    Sebesseg v;
    bool motor;
    bool jobbra;
    bool balra;
    int szog;
    double a;
    double Fuel;
    ido Time;
    bool lampa_eg;
} Komp;

enum { hossz =  50};
enum { magas =  70};
double Gravity = 1.0;
SDL_Color feher = {255, 255, 255}, piros = {255,   0,   0},
          sarga = {255, 255,   0}, lila  = {205, 143, 205},
          fekete= {  0,   0,   0}, zold  = {  0, 255,   0};

/*.Name = nev_bevitel(renderer,font_n), .x = rand() % 1000 + 100, .y = rand() % 100 + 30, //Űrkomp inicializálása
                   .v = { .x = 0, .y = 0 }, .motor  = false, .jobbra = false, .balra  = false,
                   .szog = 0,.a = 0,.Fuel = 100, .Time = {.perc = 0,.mp10 = 0, .mp1 = 0}, .lampa_eg = false};*/

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
    if (k->motor)
    {
        k->a = -3;
        k->Fuel -= 0.05;
    }
    else k->a = Gravity;
    if (k->jobbra)
        k->szog += 1;
    if (k->balra)
        k->szog -= 1;
    ax =           k->a * sin(k->szog * -0.0174532925); // gyorsulas vízszintes komponense
    ay = Gravity + k->a * cos(k->szog *  0.0174532925); // gyorsulas függőleges komponense
    k->v.x += ax/75;
    k->v.y += ay/75;
    k->x   += k->v.x;
    k->y   += k->v.y;
    if (k->x > 1920)
        k->x -= 1920;
    else if (k->x < 0)
        k->x +=1920;
}

void ido_szamlal(Komp *k)
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

void rajzol_terkep(SDL_Renderer *renderer, Sint16 *x, Sint16 *y, int melyik)
{
    filledPolygonRGBA(renderer, x, y, 21, 127, 127, 127, 255);
    thickLineRGBA(renderer, x[melyik-1], y[melyik-1]+1, x[melyik], y[melyik]+1, 3, 255, 255, 255, 255);
}
void rajzol_komp(SDL_Renderer *renderer,Komp *k,SDL_Texture *kep)
{
    SDL_Rect cel = {(int)k->x-hossz/2,(int)k->y-magas/2,hossz,magas}; //Az x és az y a nyegyzet kozeppontja
    SDL_RenderCopyEx(renderer,kep,NULL,&cel,
                     k->szog,
                     NULL,SDL_FLIP_NONE);
}

void lampa_rajzol(SDL_Renderer *renderer,SDL_Texture *lampa)
{
    SDL_Rect ide = {1850,130,50,50};
    SDL_RenderCopy(renderer,lampa,NULL,&ide);
}

void ir(SDL_Renderer *renderer, SDL_Rect hova, TTF_Font *font,char *szoveg, SDL_Color szin)
{
    SDL_Surface *felirat = TTF_RenderUTF8_Solid(font, szoveg,szin);
    SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}


void adatok_kiir(SDL_Renderer *renderer, Komp k,TTF_Font *font, Sint16 *py, int melyik)
{
    SDL_Rect nev = {100,10,100,40};
    SDL_Rect szoveg_n = {5,10,100,40};
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
    sprintf(szam[0] ,"%0.1f",py[melyik]-k.y);                      //A számadatokat string-é konvertálom(y)
    sprintf(szam[1] ,"%0.2f",fabs(k.v.x*10));                      //hogy ki lehessen őket írni         (Vx)
    sprintf(szam[2] ,"%0.2f",fabs(k.v.y*10));                      //                                   (Vy)
    sprintf(szam[3] ,"%d°"   ,abs(k.szog)   );                     //                                   (szög)
    sprintf(szam[4] ,"%0.0f",k.Fuel        ); strcat(szam[4],"%" );//                                   (Fuel)
    sprintf(szam[5] ,"%d:%d%d",k.Time.perc,k.Time.mp10,k.Time.mp1);//                                   (Time)
    sprintf(szam[6] ,"%s","Off");                                  //                                   (Lampa)
    if (!k.lampa_eg)
        n = 7;
    for(int i = 0; i < n; i++)
    {
        SDL_Rect asd = {1850,10 + i *20,50,16};
        ir(renderer,asd,font,szam[i],fabs(k.v.x) > 0.5 ? piros : k.v.y > 3 ? piros : abs(k.szog) > 3 ? piros: zold);
    }
}

adat* seged(adat* data,int i)
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

adat* rendez(adat* data, int sor_sz)
{
    for(int j = sor_sz-1; j > 0; --j)
        for( int i = 0; i < j; i++)
            {
                if(data[i].perc > data[i+1].perc)
                    seged(data,i);
                if(data[i].perc == data[i+1].perc && data[i].mp10  > data[i+1].mp10 )
                    seged(data,i);
                if(data[i].perc == data[i+1].perc && data[i].mp10 == data[i+1].mp10 && data[i].mp1 > data[i+1].mp1)
                    seged(data,i);
            }
    return data;

}
