//
// Created by Przemysław Hoszowski on 08.01.2020.
//
#include "funkcje.h"
int dokladnosc=1201;

void usun_znak_nowej_lini(char *wsk)
{
    wsk=strstr(wsk,"\n");
    *wsk='\0';
}

short odwroc(short tmp)
{
    short odp=0;
    for (short i=0;i<=7;i++)
    {
        if ((tmp&(1<<i))!=0)
            odp+=1<<(8+i);
    }
    for (short i=8;i<16;i++)
    {
        if ((tmp&(1<<i))!=0)
            odp+=1<<(i-8);
    }
    return odp;
}

void usrednij(int x, int y, int i, int j, short tab[y][x])
{
    short ile=0;
    int suma=0;
    if (i>0 && j>0)
        ile++, suma+=tab[i-1][j-1];
    if (i>0)
        ile++, suma+=tab[i-1][j];
    if (i>0 && j<x-1)
        ile++, suma+=tab[i-1][j+1];
    if (j>0)
        ile++, suma+=tab[i][j-1];
    if (j<x-1 && tab[i][j+1]!=-32768)
        ile++, suma+=tab[i][j+1];
    if (i<y-1 && j>0 && tab[i+1][j-1]!=-32768)
        ile++, suma+=tab[i+1][j-1];
    if (i<y-1 && tab[i+1][j]!=-32768)
        ile++, suma+=tab[i+1][j];
    if (i<y-1 && j<x-1 && tab[i+1][j+1]!=-32768)
        ile++, suma+=tab[i+1][j+1];
    if (ile==0)
    {
        printf("Nie da sie usrednic pixela.\n");
        exit(4);
    }
    tab[i][j]=suma/ile;
}

int ile_wody(int i, int j, int x, int y, short wysokosci[y][x], bool vis[y][x]) { /// To jest do niczego trzeba kolejke jakas zrobic
    int suma=0;
    kolejka *queue = malloc(sizeof(kolejka));
    queue->ostatni=NULL;
    vis[i][j] = 1;
    dodaj(queue, i, j);
    while (queue->pierwszy != NULL)
    {
        wez(queue,&i,&j);
        suma++;
        if (i>0 && vis[i-1][j]==0 && wysokosci[i-1][j]==0) /// do gory
        {
            dodaj(queue, i - 1, j);
            vis[i-1][j]=1;
        }
        if (i<y-1 && vis[i+1][j]==0 && wysokosci[i+1][j]==0) /// w dol
        {
            dodaj(queue, i + 1, j);
            vis[i+1][j]=1;
        }
        if (j>0 && vis[i][j-1]==0 && wysokosci[i][j-1]==0) /// do lewo
        {
            dodaj(queue,i,j-1);
            vis[i][j-1]=1;
        }
        if (j<x-1 && vis[i][j+1]==0 && wysokosci[i][j+1]==0) /// do prawo
        {
            dodaj(queue,i,j+1);
            vis[i][j+1]=1;
        }
    }
    return suma;
}

void dodaj_ziemie(int i, int j, int x, int y,short wysokosci[y][x])
{
    wysokosci[i][j]=1;
    if (i+1<y && wysokosci[i+1][j]==0)
        dodaj_ziemie(i+1,j, x, y, wysokosci);
    if (i>0 && wysokosci[i-1][j]==0)
        dodaj_ziemie(i-1,j,x,y,wysokosci);
    if (j+1<x && wysokosci[i][j+1]==0)
        dodaj_ziemie(i,j+1,x,y,wysokosci);
    if (j>0 && wysokosci[i][j-1]==0)
        dodaj_ziemie(i,j-1,x,y,wysokosci);
}

void usun_wode(int x, int y, short wysokosci[y][x]){

    bool (*vis)[y];
    vis=(bool(*)[x])malloc(x*y*sizeof(bool));
    for (int i=0;i<y;i++)
        for (int j=0;j<x;j++) {
            vis[i][j] = 0;
        }
    for (int i=0;i<y;i++)
        for (int j=0;j<x;j++)
            if (vis[i][j]==0 && wysokosci[i][j]==0) {
                if (ile_wody(i, j, x, y, wysokosci, vis) < dokladnosc * dokladnosc / 1000 )
                    dodaj_ziemie(i, j, x, y, wysokosci);
            }
    free(vis);
}

void program(char nazwy_plikow[][41],int ile_plikow,char nazwa[41])
{
//    struct konfiguracja config;
//    config=wczytaj_config();
//    dokladnosc=config.dokladnosc;
    int krawedz=0;
    while (krawedz*krawedz<ile_plikow)
        krawedz++;
    int ile_pixeli=krawedz*(dokladnosc-1)+1;
    ///Czytnie plików
    FILE *(pliki[ile_plikow]);
    for (int i=0;i<ile_plikow;i++)
    {
        pliki[i]=fopen(nazwy_plikow[i],"rb");
        if (pliki[i]==NULL)
        {
            printf("Nie udalo sie wczytac pliku %s\n",nazwy_plikow[i]);
            exit(2);
        }
    }
    short (*wysokosci)[ile_pixeli];
    wysokosci=(short(*)[ile_pixeli])malloc(ile_pixeli*ile_pixeli*sizeof(short));
    int rzad=0; /// Ktory rzad PLIKOW
    for (int i=0;i<ile_pixeli;i++)
    {
            for (int j=0;j<krawedz;j++) {
                if (fread(&wysokosci[i][j * dokladnosc - j], sizeof(short), dokladnosc, pliki[rzad * krawedz + j]) !=
                    dokladnosc) {
                    printf("Blad przy czytaniu plikus %i\n", i);
                    exit(3);
                }
            }
        if (i!=0 && i%(dokladnosc-1)==0)
            rzad++;
    }
    for (int i=0;i<ile_plikow;i++)
    {
        fclose(pliki[i]);
    }
    for (int i=0;i<ile_pixeli;i++) /// Zamiana big endian w little endian
        for (int j=0;j<ile_pixeli;j++)
           wysokosci[i][j]=odwroc(wysokosci[i][j]);
    ///Koniec czytania plikow
    for (int i=0;i<ile_pixeli;i++) /// Naprawa pojedynczych wysokosci
        for (int j=0;j<ile_pixeli;j++)
            if (wysokosci[i][j]<-1000)
                usrednij(ile_pixeli,ile_pixeli,i,j,wysokosci);

    usun_wode(ile_pixeli,ile_pixeli, wysokosci);
    wypisz_do_pliku_1(ile_pixeli,ile_pixeli,wysokosci,nazwa);
}