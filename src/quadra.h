#include "libs.h"

#ifndef QUADRA_H
#define QUADRA_H

/*
 * Modulo Quadra
 *
 * Representa uma quadra urbana (retangulo identificado por CEP).
 * A ancora e' o canto inferior esquerdo (sudoeste) no sistema de coordenadas SVG.
 * Dados originados dos comandos 'q' (geometria) e 'cq' (cores) do arquivo .geo.
 */

typedef void* Quadra;

/*
 * createQuadra: aloca e retorna uma Quadra com todos os campos preenchidos.
 * entrada:
 *   cep    — identificador da quadra (string, ex: "b01.1"); NULL resulta em string vazia.
 *   x, y   — coordenadas do canto inferior esquerdo.
 *   w, h   — largura e altura da quadra.
 *   sw     — espessura do contorno SVG (ex: "1.0px"); NULL usa "1.0px".
 *   cfill  — cor de preenchimento SVG; NULL usa "red".
 *   cstrk  — cor do contorno SVG; NULL usa "black".
 * saida: ponteiro opaco para a Quadra criada. Deve ser liberado com freeQuadra().
 */
Quadra createQuadra(char* cep, double x, double y, double w, double h,
                    char* sw, char* cfill, char* cstrk);

/*
 * freeQuadra: libera a memoria alocada para a quadra.
 * entrada: q — quadra a liberar (aceita NULL sem efeito).
 * saida: nenhuma.
 */
void freeQuadra(Quadra q);

/*
 * calcularEnderecoQuadra: calcula as coordenadas (x, y) de um endereco na quadra.
 * A face indica a aresta da quadra; num e' o deslocamento ao longo dessa aresta.
 *   Face N: aresta inferior (y + h), variando x.
 *   Face S: aresta superior (y),     variando x.
 *   Face L: aresta direita  (x + w), variando y.
 *   Face O: aresta esquerda (x),     variando y.
 * entrada:
 *   q        — quadra de referencia.
 *   face     — caractere 'N', 'S', 'L', 'O' (ou 'W', tratado como 'L').
 *   num      — deslocamento ao longo da face.
 *   out_x, out_y — ponteiros para receber as coordenadas calculadas.
 * saida: nenhuma (resultado escrito em *out_x e *out_y).
 */
void calcularEnderecoQuadra(Quadra q, char face, double num, double* out_x, double* out_y);

/* --- Getters: retornam os campos da quadra (nao devem ser liberados pelo chamador) --- */

/* getQuadraCep: retorna o CEP da quadra. */
char* getQuadraCep(Quadra q);

/* getQuadraX / Y / W / H: retornam posicao e dimensoes. */
double getQuadraX(Quadra q);
double getQuadraY(Quadra q);
double getQuadraW(Quadra q);
double getQuadraH(Quadra q);

/* getQuadraSw: retorna a espessura do contorno SVG. */
char* getQuadraSw(Quadra q);

/* getQuadraCfill: retorna a cor de preenchimento SVG. */
char* getQuadraCfill(Quadra q);

/* getQuadraCstrk: retorna a cor do contorno SVG. */
char* getQuadraCstrk(Quadra q);

/*
 * getQuadraSize: retorna o tamanho em bytes da estrutura interna de Quadra.
 * Utilizado pelo modulo hashfile para alocar registros de tamanho correto.
 * saida: tamanho em bytes (size_t).
 */
size_t getQuadraSize();

#endif
