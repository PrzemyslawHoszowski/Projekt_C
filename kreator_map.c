///Przemysław Hoszowski Projekt nr 3 Kreator_Map Styczeń 2020
#include "funkcje/funkcje.h"
int main(int argc, char **argv) {
    konfiguracja config;
    config.ile_plikow=atoi(argv[1]);
    wczytaj_config(&config);
    /// Sprawdzenie flag
    printf("TU dziala");
    int i=2;
    if (strstr(argv[2],"l")!=NULL)
        config.dokladnosc=3601, i=3;
    else config.dokladnosc=1201;

    if (strstr(argv[2],"g")!=NULL)
        config.zaznaczanie_gor=TRUE, i=3;
    else config.zaznaczanie_gor=FALSE;

    if (strstr(argv[2],"m")!=NULL)
        strcpy(config.miejsce_map,""), i=3;

    if (strstr(argv[2],"o")!=NULL)
        strcpy(config.miejsce_zapisu,""), i=3;

    if (i>=argc+1)
    {
        printf("Za mało argumentów\n");
        return 1;
    }
    config.nazwa_pliku[0]='\0';
    if (strstr(argv[i],"-t")!=NULL)
    {
        if (argc<=i+2)
        {
            printf("Za mało argumentów\n");
            return 1;
        }
        strcpy(config.nazwa_pliku,argv[i+1]);
        i+=2;
    }
    if (strstr(argv[i],"-p")!=NULL)
    {
        if (argc<=i+2)
        {
            printf("Za mało argumentów\n");
            return 1;
        }
        strcpy(config.adres_palety,argv[i+1]);
        i+=2;
    }
    if (strstr(argv[i],"-t")!=NULL) /// Jakby na odwrot byłó wpisane
    {
        if (argc<=i+2)
        {
            printf("Za mało argumentów\n");
            return 1;
        }
        strcpy(config.nazwa_pliku,argv[i+1]);
        i+=2;
    }
    if (argc<=i)
    {
        printf("Prosze podac wspolrzedne w formacie RxPy\n");
        char linia[10];
        if(fgets(linia,10,stdin)==NULL)
        {
            printf("Nie udalo sie wczytac\n");
            return 1;
        }
        strcpy(config.nazwa_pliku,linia);
        config.x=atoi(&linia[1]);
        if (linia[0]=='S')
            config.x*=-1;
        int j=1;
        while (linia[j]!='E' && linia[j]!='W')
            j++;
        config.y=atoi(&linia[j+1]);
        if (linia[j]=='W')
            config.y*=-1;
    }
    else
    {
        config.x=atoi(&argv[i][1]);
        if (argv[i][0]=='S')
            config.x*=-1;
        int j;
        strcpy(config.nazwa_pliku,argv[i]);
        while (argv[i][j]!='E'&& argv[i][j]!='e'&& argv[i][j]!='w' && argv[i][j]!='W')
            j++;
        config.y=atoi(&argv[i][j+1]);
        if (argv[i][j]=='W')
            config.y*=-1;
    }

    char nazwy_plikow[config.ile_plikow*config.ile_plikow][20];
    for (int i1=0;i1<config.ile_plikow;i1++)
    {
        for (int i2=0;i2<config.ile_plikow;i2++)
        {
            wpisz_kordynaty(nazwy_plikow[i1*config.ile_plikow+i2], config.x -i1 , config.y +i2);
        }
    }
    for (int i1=0;i1<config.ile_plikow*config.ile_plikow;i1++)
        printf("%s\n",nazwy_plikow[i1]);
    printf ("%hi %hi\n",config.x,config.y);
    printf(">>>%s\n",config.miejsce_map);
    program(config, nazwy_plikow);
    return 0;
}
