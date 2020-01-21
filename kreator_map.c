///Przemysław Hoszowski Projekt nr 3 Kreator_Map Styczeń 2020
#include "funkcje.h"
int main(int argc, char **argv) {///
    if (argc < 3) {
        printf("Za malo argumentow\n");
        return 1;
    }
    int ile_plikow = stoi(argv[1]), cel = 1;
    if (ile_plikow < 1) {
        printf("Za malo plikow");
        return 1;
    }

    cel = stoi(argv[2]);
    if (cel < 1 || cel > 3) {
        printf("Bledny numer celu programu");
        return 1;
    }
    char nazwa_obrazu[41];
    if (argc>=4)
    {
        strcpy(nazwa_obrazu,argv[3]);
    } else
    {
        strcpy(nazwa_obrazu,"Stworzony_obraz");
    }
    char nazwy_plikow[ile_plikow][41], *tmp; ///Wczytanie nazw plikow
    for (int i = 0; i < ile_plikow; i++) {
        if(fgets(nazwy_plikow[i], 40, stdin)) {
            tmp = strstr(nazwy_plikow[i], "\n");
            *tmp = '\0';
        } else
        {
            printf("Nie udalo sie przeczytac nazwy pliku\n");
            return 1;
        }
    }
    program(nazwy_plikow, ile_plikow,nazwa_obrazu);

    return 0;
}
