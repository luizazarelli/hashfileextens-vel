#include "libs.h"

#ifndef SVG_H
#define SVG_H

/*
 * Modulo SVG
 *
 * Fornece macros de formato para elementos SVG basicos e funcoes para
 * abrir e fechar um arquivo SVG com cabecalho e rodape corretos.
 */

/* RECT_SVG: formato printf para um <rect> SVG.
 * Parametros: id (int), x, y, width, height (double), stroke (str), fill (str). */
#define RECT_SVG \
    "<rect id=\"%d\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" " \
    "stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" fill-opacity=\"75%%\"/>\n"

/* CIRC_SVG: formato printf para um <circle> SVG.
 * Parametros: id (int), cx, cy, r (double), stroke (str), fill (str). */
#define CIRC_SVG \
    "<circle id=\"%d\" cx=\"%lf\" cy=\"%lf\" r=\"%lf\" " \
    "stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" fill-opacity=\"75%%\"/>\n"

/* LINE_SVG: formato printf para um <line> SVG.
 * Parametros: id (int), x1, y1, x2, y2 (double), stroke (str). */
#define LINE_SVG \
    "<line id=\"%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" " \
    "stroke=\"%s\" stroke-width=\"1\"/>\n"

/* TEXT_SVG: formato printf para um <text> SVG com conteudo em CDATA.
 * Parametros: id (int), x, y (double), stroke (str), fill (str), text-anchor (str), texto (str). */
#define TEXT_SVG \
    "<text id=\"%d\" x=\"%lf\" y=\"%lf\" stroke=\"%s\" stroke-width=\"1\" " \
    "fill=\"%s\" text-anchor=\"%s\"><![CDATA[%s]]></text>\n"

/*
 * openSvg: cria um arquivo SVG e escreve o cabecalho XML/SVG.
 * entrada: path — caminho do arquivo a criar.
 * saida: ponteiro para o FILE aberto, ou NULL se nao conseguir criar.
 */
FILE* openSvg(char* path);

/*
 * closeSvg: escreve o rodape </svg> e fecha o arquivo.
 * entrada: svg — arquivo retornado por openSvg() (aceita NULL sem efeito).
 * saida: nenhuma.
 */
void closeSvg(FILE* svg);

#endif
