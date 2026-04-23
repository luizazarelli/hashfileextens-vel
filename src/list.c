#include "list.h"

typedef struct node
{
    void*        data;
    struct node* next;
} node;

typedef struct list_info
{
    node* head;
    int   size;
} list_info;

Lista createList()
{
    list_info* l = calloc(1, sizeof(list_info));
    assert(l != NULL);
    return l;
}

void freeList(Lista lista)
{
    if (!lista) return;
    list_info* l = lista;
    node* cur = l->head;
    while (cur)
    {
        node* tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    free(l);
}

void* insertAtTail(Lista lista, void* data)
{
    list_info* l = lista;
    node* n = calloc(1, sizeof(node));
    assert(n != NULL);
    n->data = data;

    if (!l->head)
    {
        l->head = n;
    }
    else
    {
        node* cur = l->head;
        while (cur->next)
            cur = cur->next;
        cur->next = n;
    }
    l->size++;
    return n;
}

void removeNode(Lista lista, void* nd)
{
    list_info* l = lista;
    node* n = nd;

    if (l->head == n)
    {
        l->head = n->next;
    }
    else
    {
        node* cur = l->head;
        while (cur && cur->next != n)
            cur = cur->next;
        if (cur)
            cur->next = n->next;
    }
    free(n);
    l->size--;
}

void* getHead(Lista l)   { return ((list_info*)l)->head; }
void* getNext(void* nd)  { return ((node*)nd)->next; }
void* getData(void* nd)  { return ((node*)nd)->data; }
int   getSize(Lista l)   { return ((list_info*)l)->size; }
