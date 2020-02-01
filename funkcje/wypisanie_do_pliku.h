//
// Created by Przemysław Hoszowski on 28.01.2020.
//

#ifndef KREATOR_MAP_WYPISANIE_DO_PLIKU_H
#define KREATOR_MAP_WYPISANIE_DO_PLIKU_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include "funkcje.h"

typedef struct kolor{
    short kod_koloru[3]; /// W RGB
    short wysokosc; /// Do jakiej wysokosci jest ten kolor
    struct kolor *next; /// Wskaznik na kolejny kolor
}kolor;

kolor* wczytaj_palete(short min_wysokosc, short maks_wysokosc,char *adres_palety);

/// Rysowanie pojedynczego wiersza w formacie jpg
/// \param linia - miejsce zapisu
/// \param rzad - ktory rzad
/// \param szerokosc - szerokosc tablicy
/// \param wysokosci - tablica wysokosci
/// \param paleta - paleta kolorow
void wypelnij_linie(unsigned char *linia,int rzad, int szerokosc, short wysokosci[][szerokosc], kolor *paleta, unsigned char kolor_gory[3]);

/// Wypisanie do pliku jpg
/// \param x - szerokosc tablicy
/// \param y - wysokosc tablicy
/// \param wysokosci - tablica wysokosci
/// \param nazwa - Nazwa pliku
void wypisz_do_pliku_1(int x, int y, short wysokosci[y][x],char nazwa[41],char *adres_palety,char *adres_zapisu, unsigned char kolor_gory[3]); /// Wypisanie do pliku samej mapy fizycznej

#endif //KREATOR_MAP_WYPISANIE_DO_PLIKU_H
