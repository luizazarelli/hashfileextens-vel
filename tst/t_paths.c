#define _POSIX_C_SOURCE 200809L
#include "unity.h"
#include "paths.h"
#include <string.h>
#include <stdlib.h>

void setUp(void) {}
void tearDown(void) {}

void test_create_not_null(void) {
    void* ap = createAllPaths();
    TEST_ASSERT_NOT_NULL(ap);
    freeAllPaths(ap);
}

void test_setters_and_getters(void) {
    void* ap = createAllPaths();
    char* bed = strdup("/entrada/");
    char* bsd = strdup("/saida/");
    setBed(ap, bed);
    setBsd(ap, bsd);
    TEST_ASSERT_EQUAL_STRING("/entrada/", getBed(ap));
    TEST_ASSERT_EQUAL_STRING("/saida/",   getBsd(ap));
    freeAllPaths(ap);
}

void test_prepareDir_adds_slash(void) {
    char* dir = strdup("/foo/bar");
    dir = prepareDir(dir);
    TEST_ASSERT_EQUAL_STRING("/foo/bar/", dir);
    free(dir);
}

void test_prepareDir_keeps_slash(void) {
    char* dir = strdup("/foo/bar/");
    dir = prepareDir(dir);
    TEST_ASSERT_EQUAL_STRING("/foo/bar/", dir);
    free(dir);
}

void test_prepareDoc_with_path_and_extension(void) {
    char asread[] = "/some/path/cidade.geo";
    char name[64];
    prepareDoc(asread, name);
    TEST_ASSERT_EQUAL_STRING("cidade", name);
}

void test_prepareDoc_no_path(void) {
    char asread[] = "arquivo.qry";
    char name[64];
    prepareDoc(asread, name);
    TEST_ASSERT_EQUAL_STRING("arquivo", name);
}

void test_all_path_setters(void) {
    void* ap = createAllPaths();
    char* bedgeo   = strdup("bed/geo/");
    char* geoarq   = strdup("c1.geo");
    char* geoname  = strdup("c1");
    char* bedpm    = strdup("bed/pm/");
    char* pmarq    = strdup("c1.pm");
    char* pmname   = strdup("c1pm");
    char* bedqry   = strdup("bed/qry/");
    char* qryarq   = strdup("q1.qry");
    char* qryname  = strdup("q1");
    char* svg1     = strdup("out/c1.svg");
    char* svg2     = strdup("out/c1-q1.svg");
    char* txt      = strdup("out/c1-q1.txt");

    setBedGeo(ap, bedgeo);   setGeoArq(ap, geoarq);   setGeoName(ap, geoname);
    setBedPm(ap, bedpm);     setPmArq(ap, pmarq);     setPmName(ap, pmname);
    setBedQry(ap, bedqry);   setQryArq(ap, qryarq);   setQryName(ap, qryname);
    setBsdGeoSvg(ap, svg1);  setBsdGeoQrySvg(ap, svg2); setBsdGeoQryTxt(ap, txt);

    TEST_ASSERT_EQUAL_STRING("bed/geo/", getBedGeo(ap));
    TEST_ASSERT_EQUAL_STRING("c1.geo",   getGeoArq(ap));
    TEST_ASSERT_EQUAL_STRING("c1",       getGeoName(ap));
    TEST_ASSERT_EQUAL_STRING("bed/pm/",  getBedPm(ap));
    TEST_ASSERT_EQUAL_STRING("c1.pm",    getPmArq(ap));
    TEST_ASSERT_EQUAL_STRING("c1pm",     getPmName(ap));
    TEST_ASSERT_EQUAL_STRING("bed/qry/", getBedQry(ap));
    TEST_ASSERT_EQUAL_STRING("q1.qry",   getQryArq(ap));
    TEST_ASSERT_EQUAL_STRING("q1",       getQryName(ap));
    TEST_ASSERT_EQUAL_STRING("out/c1.svg",     getBsdGeoSvg(ap));
    TEST_ASSERT_EQUAL_STRING("out/c1-q1.svg",  getBsdGeoQrySvg(ap));
    TEST_ASSERT_EQUAL_STRING("out/c1-q1.txt",  getBsdGeoQryTxt(ap));

    freeAllPaths(ap);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_not_null);
    RUN_TEST(test_setters_and_getters);
    RUN_TEST(test_prepareDir_adds_slash);
    RUN_TEST(test_prepareDir_keeps_slash);
    RUN_TEST(test_prepareDoc_with_path_and_extension);
    RUN_TEST(test_prepareDoc_no_path);
    RUN_TEST(test_all_path_setters);
    return UNITY_END();
}
