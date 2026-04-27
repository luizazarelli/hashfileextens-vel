#include "libs.h"
#include "hashfile.h"
#include "unity.h"

// Struct dummy genérica para simular dados do projeto (Quadra ou Habitante)
typedef struct {
    int id;
    double value;
} DummyData;

void setUp(void) {
    // Apagar arquivos residuais antes de cada teste
    remove("test_hash.hf");
    remove("test_hash.hfd");
}

void tearDown(void) {
    remove("test_hash.hf");
    remove("test_hash.hfd");
}

void test_Criar_E_Abrir_Hashfile(void) {
    Hashfile hf = createHashfile("test_hash.hf", sizeof(DummyData), 3);
    TEST_ASSERT_NOT_NULL(hf);
    closeHashfile(hf);

    // Reabrir o arquivo que acabou de ser criado
    Hashfile hf2 = openHashfile("test_hash.hf");
    TEST_ASSERT_NOT_NULL(hf2);
    closeHashfile(hf2);
}

void test_Inserir_E_Buscar_Basico(void) {
    Hashfile hf = createHashfile("test_hash.hf", sizeof(DummyData), 3);
    
    DummyData d1 = {10, 3.14};
    insertHashfile(hf, "CHAVE_1", &d1);

    DummyData* rd1 = (DummyData*)getHashfile(hf, "CHAVE_1");
    TEST_ASSERT_NOT_NULL(rd1);
    TEST_ASSERT_EQUAL_INT(10, rd1->id);
    
    free(rd1);
    closeHashfile(hf);
}

void test_Inserir_Ate_Causar_Overflow(void) {
    // Forçar bucket pequeno para facilitar overflow (ex: 2 slots/bucket)
    Hashfile hf = createHashfile("test_hash.hf", sizeof(DummyData), 2);

    // Dicionario fixo para cair no mesmo Hash (simulação ou apenas forçando varredura completa)
    // Para garantir colisões num bucket, teríamos que interceptar o djb2, ou apenas inserir várias chaves.
    // Como a capacidade é 101, vamos inserir 300 chaves diferentes; estatisticamente haverá overflow.
    for (int i = 0; i < 300; i++) {
        char key[20];
        snprintf(key, 20, "KEY_%d", i);
        DummyData d = {i, (double)i * 2.5};
        insertHashfile(hf, key, &d);
    }

    // Verificar se conseguimos recuperar
    DummyData* rd = (DummyData*)getHashfile(hf, "KEY_150");
    TEST_ASSERT_NOT_NULL(rd);
    TEST_ASSERT_EQUAL_INT(150, rd->id);
    free(rd);

    // Gerar dump do que aconteceu
    dumpHashfileTxt(hf, "test_hash.hfd");

    closeHashfile(hf);

    // Garantir que o dump foi criado
    FILE* f = fopen("test_hash.hfd", "r");
    TEST_ASSERT_NOT_NULL(f);
    fclose(f);
}

void test_Remocao_Logica(void) {
    Hashfile hf = createHashfile("test_hash.hf", sizeof(DummyData), 3);
    
    DummyData d1 = {42, 99.9};
    insertHashfile(hf, "CHAVE_REMOVER", &d1);
    
    // Antes da remoção acha o dado:
    DummyData* r1 = (DummyData*)getHashfile(hf, "CHAVE_REMOVER");
    TEST_ASSERT_NOT_NULL(r1);
    free(r1);

    // Remove
    bool success = removeHashfile(hf, "CHAVE_REMOVER");
    TEST_ASSERT_TRUE(success);

    // Após a remoção (Tombstone activado) deve retornar NULL
    DummyData* r2 = (DummyData*)getHashfile(hf, "CHAVE_REMOVER");
    TEST_ASSERT_NULL(r2);

    closeHashfile(hf);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_Criar_E_Abrir_Hashfile);
    RUN_TEST(test_Inserir_E_Buscar_Basico);
    RUN_TEST(test_Inserir_Ate_Causar_Overflow);
    RUN_TEST(test_Remocao_Logica);
    return UNITY_END();
}
