//
// Created by Przemysław Hoszowski on 10.01.2020.
//

#ifndef KREATOR_MAP_FUNKCJE_H
#define KREATOR_MAP_FUNKCJE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "kolejka.h"
#include "wypisanie_do_pliku.h"

typedef struct konfiguracja{
    int ile_plikow;
    int dokladnosc;
    int dokladnosc_usuwania_wody;
    bool zaznaczanie_gor;
    char miejsce_zapisu[100];
    char miejsce_map[100];
    char adres_palety[100];
    char nazwa_pliku[100];
    unsigned char kolor_gory[3];
    short x;
    short y;
}konfiguracja;

void wczytaj_config(konfiguracja *config); /// Funkcja czytająca ustawienia i zwraca strukture konfiguracja do programu

void wpisz_kordynaty(char *tablica, short x, short y);

int wb(int x); /// Wartość bezwględna

bool czy_gora(int i,int j,int x, int y,short wysokosci[y][x]); /// Czy ten punkt pasuje do definicji góry

void usun_znak_nowej_lini(char *wsk); /// Zamienia znak nowej lini na znak konca stringa

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

/// Glowna funkcja odpowiada za przgotowanie informacji do dalszego rysowania
/// \param nazwy_plikow - tablica z nazwami plikow do odczytania
/// \param ile_plikow - ile jest plikow do odczytania
/// \param dokladnosc - jaka jest dokladnosc mapy
/// \param nazwa - nazwa pliku do ktorego ma byc wypisany wynik
void program (konfiguracja config,char nazwy_plikow[][20]);

int ile_wody(int i, int j, int x, int y, short wysokosci[y][x], bool vis[y][x]);

void dodaj_ziemie (int i, int j, int x, int y, short wysokosci[y][x]);

/// Usuwanie wody z lądu
/// \param x - szerokosc tablicy
/// \param y - wysokosc tablicy
/// \param wysokosci - tablica wysokosci
/// \param dokladnosc
void usun_wode(int x,int y,short wysokosci[y][x],int dokladnosc_usuwania);

#endif //KREATOR_MAP_FUNKCJE_H