#include "libs.h"

#ifndef HASHFILE_H
#define HASHFILE_H

/*
 * Modulo Hashfile (Hash Estatico com Encadeamento em Arquivo Binario)
 *
 * Implementa um hash gravado diretamente em disco, com capacidade primaria
 * de 101 buckets e encadeamento por blocos de overflow ao final do arquivo.
 * Permite armazenar e recuperar registros de tamanho fixo identificados por
 * chaves string (ex: CEP, CPF), sem carregar todos os dados em memoria.
 *
 * Cada arquivo .hf possui um cabecalho (HashHeader) seguido dos buckets primarios.
 * Buckets de overflow sao encadeados ao final do arquivo conforme necessario.
 */

typedef void* Hashfile;

/*
 * createHashfile: cria (ou sobrescreve) um arquivo binario de hashfile.
 * entrada:
 *   path       — caminho do arquivo a criar (ex: "../saida/c1.hf").
 *   sizeStruct — tamanho em bytes do registro a armazenar (ex: getQuadraSize()).
 *   bucketSize — numero maximo de registros por bucket.
 * saida: ponteiro opaco para o hashfile aberto. Deve ser fechado com closeHashfile().
 *        Retorna NULL se nao conseguir criar o arquivo.
 */
Hashfile createHashfile(char* path, size_t sizeStruct, int bucketSize);

/*
 * openHashfile: abre um arquivo de hashfile existente para leitura e escrita.
 * entrada: path — caminho do arquivo .hf ja existente.
 * saida: ponteiro opaco para o hashfile. Retorna NULL se o arquivo nao existir
 *        ou nao for um hashfile valido.
 */
Hashfile openHashfile(char* path);

/*
 * insertHashfile: insere um registro no hashfile identificado por chave string.
 * Se o bucket primario estiver cheio, cria um bucket de overflow encadeado.
 * entrada:
 *   hf    — hashfile aberto.
 *   chave — string identificadora do registro (ex: CPF, CEP).
 *   data  — ponteiro para o dado a inserir (deve ter tamanho == sizeStruct).
 * saida: nenhuma.
 */
void insertHashfile(Hashfile hf, char* chave, void* data);

/*
 * getHashfile: busca e retorna uma copia do registro identificado pela chave.
 * entrada:
 *   hf    — hashfile aberto.
 *   chave — string identificadora do registro.
 * saida: ponteiro para copia alocada no heap do registro encontrado,
 *        ou NULL se a chave nao existir. O chamador deve liberar com free().
 */
void* getHashfile(Hashfile hf, char* chave);

/*
 * removeHashfile: remove logicamente o registro identificado pela chave (tombstone).
 * entrada:
 *   hf    — hashfile aberto.
 *   chave — string identificadora do registro a remover.
 * saida: true se o registro foi encontrado e removido; false se nao existir.
 */
bool removeHashfile(Hashfile hf, char* chave);

/*
 * iterateHashfile: percorre todos os registros ativos do hashfile e chama o callback.
 * entrada:
 *   hf       — hashfile aberto.
 *   callback — funcao chamada para cada registro ativo com assinatura:
 *              void cb(char* key, void* data, void* extra1, void* extra2).
 *   extra1, extra2 — ponteiros opcionais repassados ao callback.
 * saida: nenhuma.
 */
void iterateHashfile(Hashfile hf, void (*callback)(char* key, void* data, void* extra1, void* extra2), void* extra1, void* extra2);

/*
 * dumpHashfileTxt: gera um arquivo texto (.hfd) com estatisticas e conteudo do hashfile.
 * entrada:
 *   hf      — hashfile aberto.
 *   txtPath — caminho do arquivo de dump a criar.
 * saida: nenhuma (arquivo criado em txtPath).
 */
void dumpHashfileTxt(Hashfile hf, char* txtPath);

/*
 * closeHashfile: fecha o arquivo e libera a estrutura opaca.
 * entrada: hf — hashfile a fechar (aceita NULL sem efeito).
 * saida: nenhuma.
 */
void closeHashfile(Hashfile hf);

#endif
