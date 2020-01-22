//
// Created by przemek on 22.01.2020.
//

#ifndef KREATOR_MAP_KOLEJKA_H
#define KREATOR_MAP_KOLEJKA_H
#include<stdlib.h>

typedef struct wagon{
    int x;
    int y;
    struct wagon *next;
}wagon;

typedef struct kolejka{
    struct wagon *pierwszy;
    struct wagon *ostatni;
}kolejka;

void dodaj(kolejka *queue,int y,int x);
void wez (kolejka *queue, int *y, int *x);

#endif //KREATOR_MAP_KOLEJKA_H
