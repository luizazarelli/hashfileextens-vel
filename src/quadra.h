#include "libs.h"

#ifndef QUADRA_H
#define QUADRA_H

/*
 * Modulo Quadra
 * Representa uma quadra na cidade (um retangulo).
 * A ancora da quadra fica no canto inferior esquerdo (sudoeste).
 * Campos baseados no comando 'q' do arquivo .geo e 'cq' de cores.
 */

typedef void* Quadra;

// Cria e retorna uma Quadra com os dados passados.
// As strings serao compeltamente copiadas.
Quadra createQuadra(char* cep, double x, double y, double w, double h,
                    char* sw, char* cfill, char* cstrk);

// Libera os recursos alocados pela quadra.
void freeQuadra(Quadra q);

/*>>>>>>>>>>All gets<<<<<<<<<<*/

char* getQuadraCep(Quadra q);
double getQuadraX(Quadra q);
double getQuadraY(Quadra q);
double getQuadraW(Quadra q);
double getQuadraH(Quadra q);
char* getQuadraSw(Quadra q);
char* getQuadraCfill(Quadra q);
char* getQuadraCstrk(Quadra q);

size_t getQuadraSize();

// Funcoes espaciais de ancoragem para os enderecos. (Baseado na face N/S/L/O e numero)
// Calcula a coordenada real (x, y) de um endereco de uma quadra especifica.
void calcularEnderecoQuadra(Quadra q, char face, double num, double* out_x, double* out_y);

#endif
