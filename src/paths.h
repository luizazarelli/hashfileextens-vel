#include "libs.h"

#ifndef PATHS_H
#define PATHS_H

/*
 * Modulo Paths (Gerenciamento de Caminhos)
 *
 * Centraliza todos os caminhos de entrada e saida do programa:
 * diretorio base de entrada (bed), diretorio de saida (bsd),
 * e os caminhos completos para os arquivos .geo, .pm, .qry e seus SVG/TXT gerados.
 *
 * A estrutura interna e' opaca — acesso somente via setters e getters.
 */

/*
 * createAllPaths: aloca e retorna uma estrutura de caminhos zerada.
 * entrada: nenhuma.
 * saida: ponteiro opaco para a estrutura. Deve ser liberado com freeAllPaths().
 */
void* createAllPaths();

/*
 * freeAllPaths: libera todas as strings internas e a propria estrutura.
 * entrada: ap — ponteiro retornado por createAllPaths().
 * saida: nenhuma.
 */
void freeAllPaths(void* ap);

/*
 * prepareDir: garante que o caminho de diretorio termina com '/'.
 * Realoca a string se necessario.
 * entrada: dir — string alocada no heap com o caminho do diretorio.
 * saida: ponteiro para a string (possivelmente realocada) com '/' ao final.
 */
char* prepareDir(char* dir);

/*
 * prepareDoc: extrai o nome base de um arquivo sem extensao.
 * Exemplo: "c1/nasc.qry" -> "nasc", "arquivo.geo" -> "arquivo".
 * entrada: asread — caminho completo como lido; onlyname — buffer de saida (min 256 bytes).
 * saida: nenhuma (resultado escrito em onlyname).
 */
void prepareDoc(char* asread, char* onlyname);

/* --- Setters: armazenam strings ja alocadas na estrutura --- */

/* bed: diretorio base de entrada (ex: "../testes-t1/") */
void setBed(void* ap, char* bed);

/* bsd: diretorio base de saida (ex: "../saida/") */
void setBsd(void* ap, char* bsd);

/* bedgeo: caminho completo do arquivo .geo de entrada */
void setBedGeo(void* ap, char* bedgeo);

/* geoarq: nome do arquivo .geo como passado em -f (ex: "c1.geo") */
void setGeoArq(void* ap, char* geoarq);

/* geoname: nome base do .geo sem extensao (ex: "c1") */
void setGeoName(void* ap, char* geoname);

/* bedpm: caminho completo do arquivo .pm de entrada */
void setBedPm(void* ap, char* bedpm);

/* pmarq: nome do arquivo .pm como passado em -pm */
void setPmArq(void* ap, char* pmarq);

/* pmname: nome base do .pm sem extensao */
void setPmName(void* ap, char* pmname);

/* bedqry: caminho completo do arquivo .qry de entrada */
void setBedQry(void* ap, char* bedqry);

/* qryarq: nome do arquivo .qry como passado em -q */
void setQryArq(void* ap, char* qryarq);

/* qryname: nome base do .qry sem extensao */
void setQryName(void* ap, char* qryname);

/* bsdgeosvg: caminho do SVG base gerado a partir do .geo */
void setBsdGeoSvg(void* ap, char* bsdgeosvg);

/* bsdgeoqrysvg: caminho do SVG de query gerado (geo + qry) */
void setBsdGeoQrySvg(void* ap, char* bsdgeoqrysvg);

/* bsdgeoqrytxt: caminho do TXT de saida da query */
void setBsdGeoQryTxt(void* ap, char* bsdgeoqrytxt);

/* --- Getters: retornam as strings armazenadas (nao devem ser liberadas pelo chamador) --- */

char* getBed(void* ap);
char* getBsd(void* ap);
char* getBedGeo(void* ap);
char* getGeoArq(void* ap);
char* getGeoName(void* ap);
char* getBedPm(void* ap);
char* getPmArq(void* ap);
char* getPmName(void* ap);
char* getBedQry(void* ap);
char* getQryArq(void* ap);
char* getQryName(void* ap);
char* getBsdGeoSvg(void* ap);
char* getBsdGeoQrySvg(void* ap);
char* getBsdGeoQryTxt(void* ap);

#endif
