#ifndef STRUCT_COLOR_H_INCLUDED
#define STRUCT_COLOR_H_INCLUDED

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

enum { hossz = 50 };
enum { magas = 80 };


#endif // STRUCT COLOR_H_INCLUDED
