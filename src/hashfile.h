#include "libs.h"

#ifndef HASHFILE_H
#define HASHFILE_H

/*
 * Modulo Hashfile (Hash Dinamico em Arquivo Binario)
 *
 * Implementa um hash dinâmico extensível ou linear gravado diretamente em disco,
 * permitindo o gerenciamento de milhares de registros sem estourar a RAM.
 *
 * Cada arquivo .hf possui um cabecalho com ponteiros e blocos (buckets) de tamanho fixo.
 * Referencia: Arquivos de Registros nao ordenados e Tecnicas de Espalhamento (Expansao dinamica).
 */

typedef void* Hashfile;

// Cria (ou sobrescreve) o arquivo binario da hashfile.
// path: "/caminho/para/arquivo.hf"
// sizeStruct: o tamanho em bytes da estrutura a ser armazenada (ex: sizeof(quadra_t))
// bucketSize: capacidade de registros por bucket 
Hashfile createHashfile(char* path, size_t sizeStruct, int bucketSize);

// Abre um hashfile binario ja existente.
Hashfile openHashfile(char* path);

// Insere um registro 'data' (com sizeof=sizeStruct fixado) referenciado por 'chave' string.
void insertHashfile(Hashfile hf, char* chave, void* data);

// Busca no hashfile pelo registro identificado por 'chave'.
// Retorna um ponteiro para uma HEAP copy do dado lido do arquivo. O chamador deve dar free().
void* getHashfile(Hashfile hf, char* chave);

bool removeHashfile(Hashfile hf, char* chave);

// Itera sobre todos os registros ativos, chamando a funcao callback para cada um
void iterateHashfile(Hashfile hf, void (*callback)(char* key, void* data, void* extra1, void* extra2), void* extra1, void* extra2);

// Imprime estatísticas e dump legível do hashfile num arquivo texto correspondente (.hfd)
void dumpHashfileTxt(Hashfile hf, char* txtPath);

// Fecha os ponteiros de arquivo e libera a estrutura opaca.
void closeHashfile(Hashfile hf);

#endif
