//
// Created by Przemysław Hoszowski on 28.01.2020.
//
#include "wypisanie_do_pliku.h"

kolor* wczytaj_palete(short min_wysokosc, short maks_wysokosc,char *adres_palety){
    FILE *plik=fopen(adres_palety,"r");
    if(plik==NULL)
    {
        printf("Nie udalo sie otworzyc pliku z kolorami %s\n",adres_palety);
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
        akt_kolor->kod_koloru[0]=atoi(wsk);
        wsk=strstr(wsk," ")+1;
        akt_kolor->kod_koloru[1]=atoi(wsk);
        wsk=strstr(wsk," ")+1;
        akt_kolor->kod_koloru[2]=atoi(wsk);
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

void wypelnij_linie(unsigned char *linia,int rzad, int szerokosc, short wysokosci[][szerokosc], kolor *paleta, unsigned char kolor_gory[3]){
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
        if (wysokosci[rzad][i]<-500)
        {
            linia[3*i]=kolor_gory[0];
            linia[3*i+1]=kolor_gory[1];
            linia[3*i+2]=kolor_gory[2];
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

void wypisz_do_pliku_1(int x, int y, short wysokosci[y][x],char nazwa[41], char *adres_palety, char *adres_zapisu,unsigned char kolor_gory[3]){
    short min_wysokosc=3222,maks_wysokosc=0;
    for (int i=0;i<y;i++) ///Znalezienie najmniejszej i najwiekszej wysokosci
        for (int j=0;j<x;j++) {
            if (wysokosci[i][j] < min_wysokosc && wysokosci[i][j]>-500)
                min_wysokosc = wysokosci[i][j];
            else if (wysokosci[i][j] > maks_wysokosc)
                maks_wysokosc = wysokosci[i][j];
        }

    kolor *paleta=wczytaj_palete(min_wysokosc,maks_wysokosc,adres_palety); ///Wczytanie palety i ustawienie dodatkowego wskaznika na nia
    kolor *akt=paleta;

    while (akt->next!=NULL)/// Wypisanie legendy w konsoli
    {
        printf("%hi %hi\n",akt->kod_koloru[0], akt->wysokosc);
        akt=akt->next;
    }
    // Zródło: https://stackoverflow.com/questions/4559648/write-to-memory-buffer-instead-of-file-with-libjpeg

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW row_pointer[1];

    char nazwa2[150]="";
    strcpy(nazwa2,adres_zapisu);
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
        wypelnij_linie(linia,rzad,x,wysokosci, paleta, kolor_gory);
        row_pointer[0]=linia;
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
        rzad++;
    }
    jpeg_finish_compress( &cinfo );
    jpeg_destroy_compress( &cinfo );
    fclose(obraz);
}