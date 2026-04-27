#include "libs.h"

#ifndef HABITANTE_H
#define HABITANTE_H

/*
 * Modulo Habitante
 *
 * Representa uma pessoa cadastrada na cidade.
 * Um habitante pode ser morador (possui endereco) ou sem-teto (sem endereco).
 * Dados originados dos comandos 'p' (pessoa) e 'm' (moradia) do arquivo .pm,
 * e dos comandos 'nasc', 'rip', 'mud', 'dspj' do arquivo .qry.
 */

typedef void* Habitante;

/*
 * createHabitante: aloca e retorna um Habitante com os dados pessoais.
 * O habitante e' criado sem endereco (sem-teto).
 * entrada:
 *   cpf        — identificador unico do habitante (string, ex: "123.456.789-00").
 *   nome       — primeiro nome.
 *   sobrenome  — sobrenome.
 *   sexo       — 'M' (masculino) ou 'F' (feminino).
 *   nasc       — data de nascimento (string, ex: "01/01/2000").
 * saida: ponteiro opaco para o Habitante criado. Deve ser liberado com freeHabitante().
 */
Habitante createHabitante(char* cpf, char* nome, char* sobrenome, char sexo, char* nasc);

/*
 * freeHabitante: libera a memoria alocada para o habitante e seu endereco (se houver).
 * entrada: h — habitante a liberar (aceita NULL sem efeito).
 * saida: nenhuma.
 */
void freeHabitante(Habitante h);

/*
 * setHabitanteEndereco: registra um endereco para o habitante (torna-o morador).
 * Substitui qualquer endereco anterior.
 * entrada:
 *   hab   — habitante a atualizar.
 *   cep   — CEP da quadra onde mora.
 *   face  — face da quadra ('N', 'S', 'L' ou 'O').
 *   num   — numero ao longo da face.
 *   compl — complemento do endereco (ex: "apto 2").
 * saida: nenhuma.
 */
void setHabitanteEndereco(Habitante hab, char* cep, char face, double num, char* compl);

/*
 * removerEnderecoHabitante: remove o endereco do habitante (torna-o sem-teto).
 * entrada: hab — habitante a atualizar.
 * saida: nenhuma.
 */
void removerEnderecoHabitante(Habitante hab);

/*
 * isMorador: verifica se o habitante possui endereco registrado.
 * entrada: hab — habitante a consultar.
 * saida: true se possui endereco; false se sem-teto.
 */
bool isMorador(Habitante hab);

/* --- Getters pessoais: retornam os campos basicos do habitante --- */

/* getHabitanteCpf: retorna o CPF do habitante. */
char* getHabitanteCpf(Habitante h);

/* getHabitanteNome: retorna o primeiro nome. */
char* getHabitanteNome(Habitante h);

/* getHabitanteSobrenome: retorna o sobrenome. */
char* getHabitanteSobrenome(Habitante h);

/* getHabitanteSexo: retorna o sexo ('M' ou 'F'). */
char getHabitanteSexo(Habitante h);

/* getHabitanteNasc: retorna a data de nascimento. */
char* getHabitanteNasc(Habitante h);

/* --- Getters de endereco: validos somente se isMorador() == true --- */

/* getHabitanteEndCep: retorna o CEP da quadra onde mora. */
char* getHabitanteEndCep(Habitante h);

/* getHabitanteEndFace: retorna a face da quadra ('N', 'S', 'L' ou 'O'). */
char getHabitanteEndFace(Habitante h);

/* getHabitanteEndNum: retorna o numero ao longo da face. */
double getHabitanteEndNum(Habitante h);

/* getHabitanteEndCompl: retorna o complemento do endereco. */
char* getHabitanteEndCompl(Habitante h);

/*
 * getHabitanteSize: retorna o tamanho em bytes da estrutura interna de Habitante.
 * Utilizado pelo modulo hashfile para alocar registros de tamanho correto.
 * saida: tamanho em bytes (size_t).
 */
size_t getHabitanteSize();

#endif
