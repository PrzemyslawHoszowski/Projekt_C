//
// Created by Przemysław Hoszowski on 08.01.2020.
//
#include "funkcje.h"
int dokladnosc=1201;

void wczytaj_config(konfiguracja *config)
{
    FILE *plik=fopen("../config","r");
    char linia[100];

    if (fgets(linia,100,plik)==NULL){
        printf("Blad podczas wczytywania konfiguracji\n");
        exit(5);
    }
    config->dokladnosc_usuwania_wody=atoi(strstr(linia,"=")+2);
    if (fgets(linia,100,plik)==NULL){
        printf("Blad podczas wczytywania konfiguracji\n");
        exit(5);
    }
    strcpy(config->miejsce_zapisu,strstr(linia,"=")+2);
    if (fgets(linia,100,plik)==NULL){
        printf("Blad podczas wczytywania konfiguracji\n");
        exit(5);
    }
    strcpy(config->miejsce_map,strstr(linia,"=")+2);
    if (fgets(linia,100,plik)==NULL){
        printf("Blad podczas wczytywania konfiguracji\n");
        exit(5);
    }
    strcpy(config->adres_palety,strstr(linia,"=")+2);
    if (fgets(linia,100,plik)==NULL){
        printf("Blad podczas wczytywania konfiguracji\n");
        exit(5);
    }
    char *wsk;
    wsk=strstr(linia,"=")+1;
    config->kolor_gory[0]=atoi(wsk);
    wsk=strstr(wsk+2," ");
    config->kolor_gory[1]=atoi(wsk);
    wsk=strstr(wsk+2," ");
    config->kolor_gory[2]=atoi(wsk);
    usun_znak_nowej_lini(config->adres_palety);
    usun_znak_nowej_lini(config->miejsce_map);
    usun_znak_nowej_lini(config->miejsce_zapisu);
}

void wpisz_kordynaty(char *tablica, short x, short y)
{
    if (x>=0)
        tablica[0]='N';
    tablica[1]=x/10+48;
    tablica[2]=x%10+48;
    if (y>=0)
        tablica[3]='E';
    tablica[4]=y/100+48;
    tablica[5]=(y/10)%10+48;
    tablica[6]=y%10+48;
    tablica[7]='\0';
    strcat(tablica,".hgt");
}

bool czy_gora(int i, int j, int x, int y,short wysokosci[y][x]) ///https://pl.wikipedia.org/wiki/G%C3%B3ra
{
    kolejka *queue = malloc (sizeof(kolejka));
    queue->ostatni=NULL;
    if (i>0) {
        if (wysokosci[i][j] > wb(wysokosci[i - 1][j]))
            dodaj(queue, i - 1, j);
        else return FALSE;
    }
    if (j>0){
        if  (wysokosci[i][j] > wb(wysokosci[i][j-1]))
            dodaj(queue,i,j-1);
        else return FALSE;
    }
    if (i<y-1){
        if (wysokosci[i][j]>wysokosci[i+1][j])
            dodaj(queue,i+1,j);
        else return FALSE;
    }
    if (j<x-1){
        if (wysokosci[i][j]>wysokosci[i][j+1])
            dodaj(queue,i,j+1);
        else return FALSE;
    }
    int a,b,zasieg;
    bool wybitne=FALSE;
    zasieg=dokladnosc/8;
    while (queue->ostatni!=NULL)
    {
        wez(queue,&a,&b);
        if (wb(a-i)+wb(b-j)>zasieg)  return wybitne;

        if (a > i &&  a < y-1 && b==j ) /// w dol
        {
            if (wysokosci[i][j]>wb(wysokosci[a+1][b]))
            {
                if (wb(wysokosci[a+1][b])-wysokosci[i][j]<-300) {
                    wybitne=TRUE;
                    continue;
                }
                dodaj(queue,a+1,b);
            } else return FALSE;
        }
        else if (a>0 && a<i && b==j) /// w gore
        {
            if (wysokosci[i][j]>wb(wysokosci[a-1][b]))
            {
                if (wb(wysokosci[a-1][b])-wysokosci[i][j]<-300)
                {
                    wybitne=TRUE;
                    continue;
                }
                dodaj(queue,a-1,b);
            } else return FALSE;
        }
        if (b>j && b<x-1) /// W prawo
        {
            if (wysokosci[i][j]>wb(wysokosci[a][b+1]))
            {
                if (wb(wysokosci[a][b+1])-wysokosci[i][j]<-300)
                {
                    wybitne=TRUE;
                    continue;
                }
                dodaj(queue,a,b+1);
            } else return FALSE;
        }
        if (b>0 && b<j) /// W lewo
        {
            if (wysokosci[i][j]>wb(wysokosci[a][b-1]))
            {
                if (wb(wysokosci[a][b-1])-wysokosci[i][j]<-300)
                {
                    wybitne=TRUE;
                    continue;
                }
                dodaj(queue,a,b-1);
            } else return FALSE;
        }
    }
    return wybitne;
}

int wb(int x)
{
    if (x>0)
        return x;
    else return -x;
}

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
        if (i==0 || i==y-1 || j==0 ||j==x-1) /// Z powodu braku informacji o terenie obok zakładam, że jest tam morze
            suma+=50;
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

void usun_wode(int x, int y, short wysokosci[y][x],int dokladnosc_usuwania){
    dokladnosc_usuwania=dokladnosc*dokladnosc/dokladnosc_usuwania;
    bool (*vis)[y];
    vis=(bool(*)[x])malloc(x*y*sizeof(bool));
    for (int i=0;i<y;i++)
        for (int j=0;j<x;j++) {
            vis[i][j] = 0;
        }
    for (int i=0;i<y;i++)
        for (int j=0;j<x;j++)
            if (vis[i][j]==0 && wysokosci[i][j]==0) {
                if (ile_wody(i, j, x, y, wysokosci, vis) < dokladnosc_usuwania )
                    dodaj_ziemie(i, j, x, y, wysokosci);
            }
    free(vis);
}

void program(konfiguracja config,char nazwy_plikow[][20])
{
    dokladnosc=config.dokladnosc;
    int krawedz=0, ile_plikow=config.ile_plikow*config.ile_plikow;
    while (krawedz*krawedz<ile_plikow)
        krawedz++;
    int ile_pixeli=krawedz*(dokladnosc-1)+1;
    ///Czytnie plików
    FILE *(pliki[ile_plikow]);
    char sciezka[150];
    for (int i=0;i<ile_plikow;i++)
    {
        strcpy(sciezka,config.miejsce_map);
        strcat(sciezka,nazwy_plikow[i]);
        pliki[i]=fopen(sciezka,"rb");
        if (pliki[i]==NULL && config.dokladnosc==1201)
        {
            printf ("Próbuje pobrać mapy\n");
            char komenda[200]="cd ";
            strcat(komenda,config.miejsce_map);
            strcat (komenda, "\n wget https://dds.cr.usgs.gov/srtm/version2_1/SRTM3/Eurasia/");
            strcat (komenda, nazwy_plikow[i]);
            strcat(komenda,".zip");
            system(komenda);
            strcpy(komenda,"cd ");
            strcat(komenda,config.miejsce_map);
            printf("%s",komenda);
            strcat(komenda,"\nunzip ");
            strcat(komenda,nazwy_plikow[i]);
            strcat(komenda,".zip");
            system(komenda);
            strcpy(komenda,"cd ");
            strcat(komenda, config.miejsce_map);
            strcat(komenda, "\n rm *.zip");
            system(komenda);
            printf(">%s<\n",sciezka);
            pliki[i]=fopen(sciezka,"rb");
            if (pliki[i]==NULL) {
                printf("Nie udalo sie wczytac pliku %s<\n", sciezka);
                exit(2);
            }
        } else if (pliki[i]==NULL)
        {
            printf("Nie udało sie wczytac pliku\n");
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

    usun_wode(ile_pixeli,ile_pixeli, wysokosci,config.dokladnosc_usuwania_wody);
    if (config.zaznaczanie_gor)
        for (int i=0;i<ile_pixeli;i++)
            for (int j=0;j<ile_pixeli;j++)
            {
                if (wysokosci[i][j]>500 && czy_gora(i,j,ile_pixeli,ile_pixeli,wysokosci)) {
                    wysokosci[i][j] *= -1;
                }
            }
    wypisz_do_pliku_1(ile_pixeli,ile_pixeli,wysokosci,config.nazwa_pliku, config.adres_palety, config.miejsce_zapisu,config.kolor_gory);
}
