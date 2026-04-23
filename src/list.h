#include "libs.h"

#ifndef LIST_H
#define LIST_H

typedef void* Lista;

Lista createList();

void freeList(Lista l);

void* insertAtTail(Lista l, void* data);

void removeNode(Lista l, void* node);

void* getHead(Lista l);

void* getNext(void* node);

void* getData(void* node);

int   getSize(Lista l);

#endif
