//
// Created by przemek on 22.01.2020.
//

#include "kolejka.h"

void dodaj(kolejka *queue, int x, int y)
{
    wagon *ostatni= malloc(sizeof(wagon));
    ostatni->x=x;
    ostatni->y=y;
    ostatni->next=NULL;
    if (queue->ostatni==NULL)
    {
        queue->pierwszy=ostatni;
        queue->ostatni=ostatni;
        return;
    }
    queue->ostatni->next=ostatni;
    queue->ostatni=ostatni;
}

void wez(kolejka *queue, int *x, int *y)
{
    wagon *tmp=queue->pierwszy;
    *x=tmp->x;
    *y=tmp->y;
    if(queue->pierwszy->next!=NULL)
        queue->pierwszy=queue->pierwszy->next;
    else
    {
        queue->pierwszy=NULL;
        queue->ostatni=NULL;
    }
    free(tmp);
}
