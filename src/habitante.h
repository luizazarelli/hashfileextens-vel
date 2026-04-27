#include "libs.h"

#ifndef HABITANTE_H
#define HABITANTE_H

/*
 * Modulo Habitante
 * Representa uma pessoa na cidade. Pode ou nao ser moradora de um endereco.
 * Campos baseados nos comandos 'p' e 'm' do arquivo .pm.
 */

typedef void* Habitante;

// Cria e retorna um Habitante com os dados base (comando 'p').
// As strings serao compeltamente copiadas.
Habitante createHabitante(char* cpf, char* nome, char* sobrenome, char sexo, char* nasc);

// Libera os recursos alocados para o habitante e, se houver, seu endereco.
void freeHabitante(Habitante h);

// Define que um habitante mora em um endereco (comando 'm').
// As strings passadas serao copiadas para a estrutura interna.
void setHabitanteEndereco(Habitante hab, char* cep, char face, double num, char* compl);

// Remove o endereco de um habitante (torna-o sem-teto/despejado).
void removerEnderecoHabitante(Habitante hab);

// Verifica se o habitante possui endereco registrado (nao e' sem-teto).
bool isMorador(Habitante hab);

/*>>>>>>>>>>All gets<<<<<<<<<<*/

char* getHabitanteCpf(Habitante h);
char* getHabitanteNome(Habitante h);
char* getHabitanteSobrenome(Habitante h);
char  getHabitanteSexo(Habitante h);
char* getHabitanteNasc(Habitante h);

// Seguros para chamada apenas se isMorador() == true
char* getHabitanteEndCep(Habitante h);
char  getHabitanteEndFace(Habitante h);
double getHabitanteEndNum(Habitante h);
char* getHabitanteEndCompl(Habitante h);

size_t getHabitanteSize();

#endif
