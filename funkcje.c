//
// Created by Przemysław Hoszowski on 08.01.2020.
//
#include "funkcje.h"
int dokladnosc;

struct konfiguracja wczytaj_config()
{
    struct konfiguracja config;
    FILE *plik=fopen("../config","r");
    char linia[100];
    if (fgets(linia,100,plik)==NULL){
        printf("Blad podczas wczytywania konfiguracji\n");
        exit(5);
    }
    config.dokladnosc= stoi(strstr(linia,"=")+2);
    if (fgets(linia,100,plik)==NULL){
        printf("Blad podczas wczytywania konfiguracji\n");
        exit(5);
    }
    config.dokladnosc_usuwania_wody=stoi(strstr(linia,"=")+2);
    if (fgets(linia,100,plik)==NULL){
        printf("Blad podczas wczytywania konfiguracji\n");
        exit(5);
    }
    strcpy(config.miejsce_zapisu,strstr(linia,"=")+2);
    if (fgets(linia,100,plik)==NULL){
        printf("Blad podczas wczytywania konfiguracji\n");
        exit(5);
    }
    strcpy(config.miejsce_map,strstr(linia,"=")+2);
    if (fgets(linia,100,plik)==NULL){
        printf("Blad podczas wczytywania konfiguracji\n");
        exit(5);
    }
    strcpy(config.adres_palety,strstr(linia,"=")+2);
    if (fgets(linia,100,plik)==NULL){
        printf("Blad podczas wczytywania konfiguracji\n");
        exit(5);
    }
    char *wsk;
    wsk=strstr(linia,"=")+2;
    config.kolor_gory[0]=stoi(wsk);
    wsk=strstr(wsk+2," ")+1;
    config.kolor_gory[1]=stoi(wsk);
    wsk=strstr(wsk+2," ")+1;
    config.kolor_gory[2]=stoi(wsk);
    usun_znak_nowej_lini(config.adres_palety);
    usun_znak_nowej_lini(config.miejsce_map);
    usun_znak_nowej_lini(config.miejsce_zapisu);
    printf("Config:\n"  ///*USUN
           "dokladnosc %i\n"
           "dokladnosc usuwania wody %i\n"
           "miejsce zapisu %s\n"
           "miejsce map %s\n"
           "adres palety %s\n"
           "kolor gory %i %i %i",config.dokladnosc, config.dokladnosc_usuwania_wody,config.miejsce_zapisu,config.miejsce_map,config.adres_palety,config.kolor_gory[0],config.kolor_gory[1],config.kolor_gory[1]);
    return config;
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

int stoi(char *znaki){
    int wynik=0;
    for (int i=0; 48<=znaki[i] && znaki[i]<58; i++)
    {
        wynik*=10;
        wynik+=(znaki[i]-48);
    }
    return wynik;
}

kolor* wczytaj_palete(short min_wysokosc, short maks_wysokosc){
    FILE *plik=fopen("/home/przemek/Dokumenty/WDC/Kreator_map/kolory","r");
    if(plik==NULL)
    {
        printf("Nie udalo sie otworzyc pliku z kolorami\n");
        exit(1);
    }
    kolor paleta;
    paleta.next=malloc(sizeof(kolor));
    kolor *akt_kolor=paleta.next;
    int ile_kolorow=0;
    char linia[50];
    char *wsk;
    if (fgets(linia,50,plik)==NULL)
    {
        printf("Blad przy czytaniu pliku z kolorami");
        exit(2);
    }
    while (!feof(plik))
    {
        wsk=linia;
        akt_kolor->kod_koloru[0]=stoi(wsk);
        wsk=strstr(wsk," ")+1;
        akt_kolor->kod_koloru[1]=stoi(wsk);
        wsk=strstr(wsk," ")+1;
        akt_kolor->kod_koloru[2]=stoi(wsk);
        akt_kolor->next=malloc(sizeof(kolor));
        akt_kolor=akt_kolor->next;
        ile_kolorow++;
        if (fgets(linia,50,plik)==NULL)
            break;
    }
    free(akt_kolor);
    akt_kolor=paleta.next;
    for (int i=0;i<ile_kolorow;i++)
    {
        akt_kolor->wysokosc=((i*(i/10+10)+6)*(maks_wysokosc-min_wysokosc))/(ile_kolorow*(ile_kolorow/10+10)+6)+min_wysokosc;
        akt_kolor=akt_kolor->next;
    }
    akt_kolor->next=NULL;
    fclose(plik);
    return paleta.next;
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

void wypelnij_linie(unsigned char *linia,int rzad, int szerokosc, short wysokosci[][szerokosc], kolor *paleta){
    kolor *akt;

    for (int i=0; i<szerokosc; i++){
        akt=paleta;
        if (wysokosci[rzad][i]==0 || wysokosci[rzad][i]==-32768)
        {
            linia[3*i]=akt->kod_koloru[0];
            linia[3*i+1]=akt->kod_koloru[1];
            linia[3*i+2]=akt->kod_koloru[2];
            continue;
        }
        akt=akt->next;
        while (akt->next!=NULL && (akt->next)->wysokosc<wysokosci[rzad][i])
            akt=akt->next;

        linia[3*i]=akt->kod_koloru[0];
        linia[3*i+1]=akt->kod_koloru[1];
        linia[3*i+2]=akt->kod_koloru[2];
    }
}

void wypisz_do_pliku_1(int x, int y, short wysokosci[y][x],char nazwa[41]){
    short min_wysokosc=3222,maks_wysokosc=0;
    for (int i=0;i<y;i++) ///Znalezienie najmniejszej i najwiekszej wysokosci
        for (int j=0;j<x;j++) {
            if (wysokosci[i][j] < min_wysokosc && wysokosci[i][j]!=-32768)
                min_wysokosc = wysokosci[i][j];
            else if (wysokosci[i][j] > maks_wysokosc)
                maks_wysokosc = wysokosci[i][j];
        }

    kolor *paleta=wczytaj_palete(min_wysokosc,maks_wysokosc); ///Wczytanie palety i ustawienie dodatkowego wskaznika na nia
    kolor *akt=paleta;

    while (akt->next!=NULL)/// Wypisanie legendy w konsoli
    {
        printf("%hi %hi\n",akt->kod_koloru[0], akt->wysokosc);
        akt=akt->next;
    }

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW row_pointer[1];

    char nazwa2[46]="";
    if (strstr(nazwa,".jpeg")==NULL && strstr(nazwa,".jpg")==NULL) ///Dorzucam koncowki
    {
        strcat(nazwa2,nazwa);
        strcat(nazwa2,".jpg");
    }
    else strcpy(nazwa2,nazwa);
    FILE *obraz =fopen(nazwa2,"wb");

    if (!obraz)
    {
        perror("Blad przy otworzeniu pliku wyjsciowego\n");
        exit(4);
    }
    cinfo.err =jpeg_std_error( &jerr );
    jpeg_create_compress( &cinfo );
    jpeg_stdio_dest( &cinfo, obraz );

    ///Ustawienie parametrow obrazu
    cinfo.image_height = y;
    cinfo.image_width = x;
    cinfo.input_components = 3;
    cinfo.in_color_space=2;
    jpeg_set_quality( &cinfo, 100, 1);
    jpeg_set_defaults( &cinfo );
    jpeg_start_compress( &cinfo , TRUE );
    int rzad=0;
    unsigned char linia[3*x];
    while ( cinfo.next_scanline < cinfo.image_height)
    {
        wypelnij_linie(linia,rzad,x,wysokosci, paleta);
        row_pointer[0]=linia;
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
        rzad++;
    }
    jpeg_finish_compress( &cinfo );
    jpeg_destroy_compress( &cinfo );
    fclose(obraz);
}

void program(char nazwy_plikow[][41],int ile_plikow,char nazwa[41])
{
    struct konfiguracja config;
    config=wczytaj_config();
    dokladnosc=config.dokladnosc;
    int krawedz=0;
    while (krawedz*krawedz<ile_plikow)
        krawedz++;
    int ile_pixeli=krawedz*(dokladnosc-1)+1;

    ///Czytnie plików
    FILE *(pliki[ile_plikow]);
    FILE *wynik=fopen("wynik","w");
    char adres_odczytu[109]="/home/przemek/Dokumenty/WDC/Kreator_map/mapy/",
         adres_tmp[150];
    for (int i=0;i<ile_plikow;i++)
    {
        strcpy(adres_tmp,adres_odczytu);
        strcat(adres_tmp,nazwy_plikow[i]);
        pliki[i]=fopen(adres_tmp,"rb");
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
    {
        for (int j=0;j<ile_pixeli;j++)
        {
            if (wysokosci[i][j]<-1000)
                usrednij(ile_pixeli,ile_pixeli,i,j,wysokosci);
            fprintf(wynik,"%4hi ",wysokosci[i][j]);
        }
        fprintf(wynik,"\n");
    }
    fclose(wynik);

    usun_wode(ile_pixeli,ile_pixeli, wysokosci);
    wypisz_do_pliku_1(ile_pixeli,ile_pixeli,wysokosci,nazwa);
}