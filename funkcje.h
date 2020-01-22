//
// Created by Przemysław Hoszowski on 10.01.2020.
//

#ifndef KREATOR_MAP_FUNKCJE_H
#define KREATOR_MAP_FUNKCJE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>
#include <stdbool.h>
typedef struct kolor{
     short kod_koloru[3]; /// W RGB
     short wysokosc; /// Do jakiej wysokosci jest ten kolor
     struct kolor *next; /// Wskaznik na kolejny kolor
}kolor;

/// Big endian format <-> little endian format
/// \param tmp - zmienna typu short
/// \return Liczba z obroconymi bitami
short odwroc(short tmp);

/// Naprawa pixeli, ktore maja wysokosc -32768
/// \param x - szerokosc tablicy
/// \param y - wysokosc tablicy
/// \param i - rzad pixela
/// \param j - kolumna pixela
/// \param tab - tablica wysokosci
void usrednij(int x, int y, int i, int j, short tab[y][x]);

///  Zamiana string na int
/// \param znaki - wskaznik na string z liczba
/// \return liczba w int
int stoi(char *znaki);

/// Glowna funkcja odpowiada za przgotowanie informacji do dalszego rysowania
/// \param nazwy_plikow - tablica z nazwami plikow do odczytania
/// \param ile_plikow - ile jest plikow do odczytania
/// \param dokladnosc - jaka jest dokladnosc mapy
/// \param nazwa - nazwa pliku do ktorego ma byc wypisany wynik
void program (char nazwy_plikow[][41], int ile_plikow,char nazwa[41]);

/// Wczytanie palety kolorow z pliku oraz dostosowanie skali
/// \param min_wysokosc
/// \param maks_wysokosc
/// \return Wskaznik na palete kolorow
kolor* wczytaj_palete(short min_wysokosc, short maks_wysokosc);

/// Rysowanie pojedynczego wiersza w formacie jpg
/// \param linia - miejsce zapisu
/// \param rzad - ktory rzad
/// \param szerokosc - szerokosc tablicy
/// \param wysokosci - tablica wysokosci
/// \param paleta - paleta kolorow
void wypelnij_linie(unsigned char *linia,int rzad, int szerokosc, short wysokosci[][szerokosc], kolor *paleta);

void ile_wody(int i, int j, int *x, int *y, short wysokosci[*y][*x], bool vis[*y][*x]);

void dodaj_ziemie (int i, int j, int x, int y, short wysokosci[y][x]);

/// Usuwanie wody z lądu
/// \param x - szerokosc tablicy
/// \param y - wysokosc tablicy
/// \param wysokosci - tablica wysokosci
/// \param dokladnosc
void usun_wode(int x,int y,short wysokosci[y][x],int dokladnosc);

/// Wypisanie do pliku jpg
/// \param x - szerokosc tablicy
/// \param y - wysokosc tablicy
/// \param wysokosci - tablica wysokosci
/// \param nazwa - Nazwa pliku
void wypisz_do_pliku_1(int x, int y, short wysokosci[y][x],char nazwa[41]); /// Wypisanie do pliku samej mapy fizycznej
#endif //KREATOR_MAP_FUNKCJE_H