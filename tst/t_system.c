#define _POSIX_C_SOURCE 200809L
#include "unity.h"
#include "system.h"
#include "paths.h"
#include "hashfile.h"
#include "quadra.h"
#include "habitante.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void setUp(void)    {}
void tearDown(void) {}

/* =========================================================
 * Testes de readParam
 * ========================================================= */

void test_readParam_caminhos_basicos(void) {
    char* argv[] = { "ted", "-e", "/tmp/in/", "-f", "c1.geo", "-o", "/tmp/out/" };
    int argc = 7;
    void* paths = createAllPaths();
    readParam(argc, argv, paths);
    TEST_ASSERT_EQUAL_STRING("c1.geo", getGeoArq(paths));
    TEST_ASSERT_EQUAL_STRING("c1",     getGeoName(paths));
    TEST_ASSERT_NOT_NULL(getBsd(paths));
    TEST_ASSERT_NOT_NULL(getBedGeo(paths));
    freeAllPaths(paths);
}

void test_readParam_sem_pm_sem_qry(void) {
    char* argv[] = { "ted", "-f", "mapa.geo", "-o", "/tmp/" };
    int argc = 5;
    void* paths = createAllPaths();
    readParam(argc, argv, paths);
    TEST_ASSERT_NULL(getPmArq(paths));
    TEST_ASSERT_NULL(getQryArq(paths));
    freeAllPaths(paths);
}

void test_readParam_com_pm_e_qry(void) {
    char* argv[] = { "ted", "-f", "c2.geo", "-o", "/tmp/",
                     "-pm", "povo.pm", "-q", "sub/q1.qry" };
    int argc = 9;
    void* paths = createAllPaths();
    readParam(argc, argv, paths);
    TEST_ASSERT_EQUAL_STRING("povo.pm", getPmArq(paths));
    TEST_ASSERT_EQUAL_STRING("povo",    getPmName(paths));
    TEST_ASSERT_EQUAL_STRING("q1",      getQryName(paths));
    freeAllPaths(paths);
}

/* =========================================================
 * Testes de readPrintGeo
 * ========================================================= */

void test_readPrintGeo_insere_quadra(void) {
    FILE* f = fopen("/tmp/t_sys.geo", "w");
    TEST_ASSERT_NOT_NULL(f);
    fprintf(f, "q b01 10.0 20.0 100.0 80.0\n");
    fclose(f);

    void* paths = createAllPaths();
    setBedGeo(paths, strdup("/tmp/t_sys.geo"));
    setBsdGeoSvg(paths, strdup("/tmp/t_sys.svg"));

    Hashfile hq = createHashfile("/tmp/t_sys_geo.hf", getQuadraSize(), 4);
    readPrintGeo(paths, hq);

    void* q = getHashfile(hq, "b01");
    TEST_ASSERT_NOT_NULL(q);
    free(q);

    closeHashfile(hq);
    freeAllPaths(paths);
}

void test_readPrintGeo_geo_inexistente_nao_trava(void) {
    void* paths = createAllPaths();
    setBedGeo(paths, strdup("/tmp/nao_existe.geo"));
    setBsdGeoSvg(paths, strdup("/tmp/nao_existe.svg"));

    Hashfile hq = createHashfile("/tmp/t_sys_geo2.hf", getQuadraSize(), 4);
    readPrintGeo(paths, hq);

    void* q = getHashfile(hq, "qualquer");
    TEST_ASSERT_NULL(q);

    closeHashfile(hq);
    freeAllPaths(paths);
}

/* =========================================================
 * Testes de readPm
 * ========================================================= */

void test_readPm_insere_habitante(void) {
    FILE* f = fopen("/tmp/t_sys.pm", "w");
    TEST_ASSERT_NOT_NULL(f);
    fprintf(f, "p 111.111.111-11 Joao Silva M 01/01/1990\n");
    fclose(f);

    void* paths = createAllPaths();
    setBedPm(paths, strdup("/tmp/t_sys.pm"));

    Hashfile hh = createHashfile("/tmp/t_sys_hab.hf", getHabitanteSize(), 4);
    readPm(paths, hh);

    void* h = getHashfile(hh, "111.111.111-11");
    TEST_ASSERT_NOT_NULL(h);
    free(h);

    closeHashfile(hh);
    freeAllPaths(paths);
}

void test_readPm_moradia_torna_morador(void) {
    FILE* f = fopen("/tmp/t_sys2.pm", "w");
    TEST_ASSERT_NOT_NULL(f);
    fprintf(f, "p 222.222.222-22 Maria Souza F 05/05/1985\n");
    fprintf(f, "m 222.222.222-22 b01 N 50.0 apto3\n");
    fclose(f);

    void* paths = createAllPaths();
    setBedPm(paths, strdup("/tmp/t_sys2.pm"));

    Hashfile hh = createHashfile("/tmp/t_sys_hab2.hf", getHabitanteSize(), 4);
    readPm(paths, hh);

    Habitante h = (Habitante)getHashfile(hh, "222.222.222-22");
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_TRUE(isMorador(h));
    TEST_ASSERT_EQUAL_STRING("b01", getHabitanteEndCep(h));
    free(h);

    closeHashfile(hh);
    freeAllPaths(paths);
}

/* =========================================================
 * Testes de readQry
 * ========================================================= */

#define QRY_GEO  "/tmp/tsys_qry.geo"
#define QRY_HF   "/tmp/tsys_qry_geo.hf"
#define QRY_HHF  "/tmp/tsys_qry_hab.hf"
#define QRY_FILE "/tmp/tsys_qry.qry"
#define QRY_SVG  "/tmp/tsys_qry.svg"
#define QRY_TXT  "/tmp/tsys_qry.txt"

/* Monta paths + dois hashfiles com um bloco pre-carregado */
static void qry_setup(void** out_p, Hashfile* out_hq, Hashfile* out_hh) {
    FILE* f = fopen(QRY_GEO, "w");
    fprintf(f, "q b01 10.0 20.0 100.0 80.0\n");
    fclose(f);

    void* p = createAllPaths();
    setBedGeo(p,       strdup(QRY_GEO));
    setBsdGeoSvg(p,    strdup(QRY_SVG));
    setBsdGeoQrySvg(p, strdup(QRY_SVG));
    setBsdGeoQryTxt(p, strdup(QRY_TXT));
    setBedQry(p,       strdup(QRY_FILE));

    Hashfile hq = createHashfile(QRY_HF,  getQuadraSize(),   4);
    readPrintGeo(p, hq);
    Hashfile hh = createHashfile(QRY_HHF, getHabitanteSize(), 4);

    *out_p  = p;
    *out_hq = hq;
    *out_hh = hh;
}

static void qry_teardown(void* p, Hashfile hq, Hashfile hh) {
    closeHashfile(hq);
    closeHashfile(hh);
    freeAllPaths(p);
    remove(QRY_GEO); remove(QRY_HF); remove(QRY_HHF);
    remove(QRY_FILE); remove(QRY_SVG); remove(QRY_TXT);
}

static void txt_read(char* buf, int sz) {
    FILE* f = fopen(QRY_TXT, "r");
    if (!f) { buf[0] = '\0'; return; }
    int n = (int)fread(buf, 1, sz - 1, f);
    buf[n] = '\0';
    fclose(f);
}

void test_readQry_qry_vazia_nao_trava(void) {
    void* p; Hashfile hq, hh;
    qry_setup(&p, &hq, &hh);

    FILE* f = fopen(QRY_FILE, "w"); fclose(f);
    readQry(p, hq, hh); /* nao deve crashar */

    qry_teardown(p, hq, hh);
}

void test_readQry_censo_produz_txt(void) {
    void* p; Hashfile hq, hh;
    qry_setup(&p, &hq, &hh);

    Habitante h = createHabitante("111.111.111-11", "Ana", "Silva", 'F', "01/01/1990");
    insertHashfile(hh, "111.111.111-11", h);
    freeHabitante(h);

    FILE* f = fopen(QRY_FILE, "w");
    fprintf(f, "censo\n");
    fclose(f);

    readQry(p, hq, hh);

    char buf[2048];
    txt_read(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Total de habitantes: 1"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Total de moradores: 0"));

    qry_teardown(p, hq, hh);
}

void test_readQry_nasc_insere_habitante(void) {
    void* p; Hashfile hq, hh;
    qry_setup(&p, &hq, &hh);

    void* antes = getHashfile(hh, "222.222.222-22");
    TEST_ASSERT_NULL(antes);

    FILE* f = fopen(QRY_FILE, "w");
    fprintf(f, "nasc 222.222.222-22 Carlos Souza M 15/06/1985\n");
    fclose(f);

    readQry(p, hq, hh);

    Habitante depois = (Habitante)getHashfile(hh, "222.222.222-22");
    TEST_ASSERT_NOT_NULL(depois);
    TEST_ASSERT_EQUAL_STRING("Carlos", getHabitanteNome(depois));
    free(depois);

    qry_teardown(p, hq, hh);
}

void test_readQry_rip_remove_habitante(void) {
    void* p; Hashfile hq, hh;
    qry_setup(&p, &hq, &hh);

    Habitante h = createHabitante("333.333.333-33", "Pedro", "Costa", 'M', "10/10/1995");
    insertHashfile(hh, "333.333.333-33", h);
    freeHabitante(h);

    FILE* f = fopen(QRY_FILE, "w");
    fprintf(f, "rip 333.333.333-33\n");
    fclose(f);

    readQry(p, hq, hh);

    void* depois = getHashfile(hh, "333.333.333-33");
    TEST_ASSERT_NULL(depois);

    qry_teardown(p, hq, hh);
}

void test_readQry_pq_bloco_inexistente_txt(void) {
    void* p; Hashfile hq, hh;
    qry_setup(&p, &hq, &hh);

    FILE* f = fopen(QRY_FILE, "w");
    fprintf(f, "pq bloco_nao_existe\n");
    fclose(f);

    readQry(p, hq, hh);

    char buf[1024];
    txt_read(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "nao encontrada"));

    qry_teardown(p, hq, hh);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_readParam_caminhos_basicos);
    RUN_TEST(test_readParam_sem_pm_sem_qry);
    RUN_TEST(test_readParam_com_pm_e_qry);
    RUN_TEST(test_readPrintGeo_insere_quadra);
    RUN_TEST(test_readPrintGeo_geo_inexistente_nao_trava);
    RUN_TEST(test_readPm_insere_habitante);
    RUN_TEST(test_readPm_moradia_torna_morador);
    RUN_TEST(test_readQry_qry_vazia_nao_trava);
    RUN_TEST(test_readQry_censo_produz_txt);
    RUN_TEST(test_readQry_nasc_insere_habitante);
    RUN_TEST(test_readQry_rip_remove_habitante);
    RUN_TEST(test_readQry_pq_bloco_inexistente_txt);
    return UNITY_END();
}
