#include "libs.h"

#ifndef LIST_H
#define LIST_H

/*
 * Modulo List (Lista Encadeada Simples)
 *
 * Implementa uma lista encadeada generica que armazena ponteiros void*.
 * Os nos sao alocados internamente; o dado (void*) nao e' copiado nem liberado
 * automaticamente — e' responsabilidade do chamador gerenciar a memoria do dado.
 */

typedef void* Lista;

/*
 * createList: aloca e retorna uma lista vazia.
 * entrada: nenhuma.
 * saida: ponteiro opaco para a lista criada. Deve ser liberado com freeList().
 */
Lista createList();

/*
 * freeList: libera todos os nos internos e a propria lista.
 * Nao libera os dados apontados pelos nos.
 * entrada: l — lista a ser liberada.
 * saida: nenhuma.
 */
void freeList(Lista l);

/*
 * insertAtTail: insere um dado no final da lista.
 * entrada: l — lista destino; data — ponteiro para o dado a inserir.
 * saida: ponteiro opaco para o no criado (usado em removeNode()).
 */
void* insertAtTail(Lista l, void* data);

/*
 * removeNode: remove e libera um no da lista (nao libera o dado apontado).
 * entrada: l — lista; node — no retornado por insertAtTail().
 * saida: nenhuma.
 */
void removeNode(Lista l, void* node);

/*
 * getHead: retorna o primeiro no da lista.
 * entrada: l — lista.
 * saida: ponteiro opaco para o primeiro no, ou NULL se a lista estiver vazia.
 */
void* getHead(Lista l);

/*
 * getNext: retorna o proximo no apos o no dado.
 * entrada: node — no atual (obtido via getHead ou getNext).
 * saida: ponteiro opaco para o proximo no, ou NULL se for o ultimo.
 */
void* getNext(void* node);

/*
 * getData: retorna o dado armazenado em um no.
 * entrada: node — no da lista.
 * saida: ponteiro void* para o dado inserido em insertAtTail().
 */
void* getData(void* node);

/*
 * getSize: retorna a quantidade de elementos na lista.
 * entrada: l — lista.
 * saida: numero inteiro com a quantidade de nos ativos.
 */
int getSize(Lista l);

#endif
