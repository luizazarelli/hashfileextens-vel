#include "hashfile.h"

/*
 * Estrutura Baseada em Disco (Hash Dinamico Extensivel / Chaining por blocos)
 *
 * O arquivo .hf comeca sempre com um CABECALHO (HashHeader).
 * Logo em seguida, residem os N buckets principais.
 * Opcionalmente, buckets de excesso (expansão) sao adicionados ao final do arquivo.
 */

#define MAGIC_NUMBER 0x48415348 // "HASH"
#define MAX_KEY_LEN  64         // Chaves (CEP, CPF) truncadas em 64 chars no disco

typedef struct {
    unsigned int magic;       // Assinatura do arquivo
    int          capacity;    // Quantidade de buckets iniciais
    int          bucketSize;  // Capacidade maxima de registros (slots) por bucket
    size_t       sizeStruct;  // Tamanho em bytes do dado armazenado (void*)
    long         numRecords;  // Contagem estatica de quantos inseridos existem
} HashHeader;

typedef struct {
    char key[MAX_KEY_LEN];
    bool active;
    // O dado binario estara armazenado imediatamente apos esse cabecalho de slot
} SlotHeader;

// Essa e a estrutura de memoria que gerencia o arquivo aberto,
// e que sera retornada como void* (Hashfile) para encapsulamento.
typedef struct {
    FILE*      file;
    HashHeader header;
    char*      path;
} hashfile_info;

/*
 * Funcao utilitaria pra calcular offset de um bucket no arquivo.
 * O bucket X fica em: sizeof(HashHeader) + X * tamanho_do_bucket
 */
static long calcBucketOffset(hashfile_info* h, int bucketIdx) {
    long sizePerSlot = sizeof(SlotHeader) + h->header.sizeStruct;
    long sizeBucket  = sizeof(int) /* count */ + sizeof(long) /* nextOverflow */
                     + (h->header.bucketSize * sizePerSlot);

    return sizeof(HashHeader) + (bucketIdx * sizeBucket);
}

/* 
 * Hash basico djb2 (retorna int 0..capacity-1) 
 */
static unsigned int hashFn(const char* chave, int capacity) {
    unsigned long h = 5381;
    int c;
    while ((c = (unsigned char)*chave++))
        h = h * 33 + c;
    return (unsigned int)(h % (unsigned long)capacity);
}

Hashfile createHashfile(char* path, size_t sizeStruct, int bucketSize) {
    hashfile_info* h = calloc(1, sizeof(hashfile_info));
    assert(h != NULL);

    h->path = strdup(path);
    h->file = fopen(path, "w+b");
    assert(h->file != NULL);

    // Preenche header
    h->header.magic      = MAGIC_NUMBER;
    h->header.capacity   = 101; // Default primos
    h->header.bucketSize = bucketSize;
    h->header.sizeStruct = sizeStruct;
    h->header.numRecords = 0;

    fwrite(&h->header, sizeof(HashHeader), 1, h->file);

    // Gera os N buckets vazios
    int count = 0;
    long nextOverflow = -1; // -1 significa NULL

    long sizePerSlot = sizeof(SlotHeader) + sizeStruct;
    void* emptySlot  = calloc(1, sizePerSlot); // Zeroed

    for (int i = 0; i < h->header.capacity; i++) {
        fwrite(&count, sizeof(int), 1, h->file);
        fwrite(&nextOverflow, sizeof(long), 1, h->file);
        for (int s = 0; s < bucketSize; s++) {
            fwrite(emptySlot, sizePerSlot, 1, h->file);
        }
    }
    
    free(emptySlot);
    fflush(h->file);

    return h;
}

Hashfile openHashfile(char* path) {
    hashfile_info* h = calloc(1, sizeof(hashfile_info));
    assert(h != NULL);

    h->path = strdup(path);
    h->file = fopen(path, "r+b"); // Leitura/Escrita, requer binario ja existente

    if (!h->file) {
        free(h->path);
        free(h);
        return NULL;
    }

    fread(&h->header, sizeof(HashHeader), 1, h->file);
    if (h->header.magic != MAGIC_NUMBER) {
        printf("Erro: Arquivo %s nao e um Hashfile valido.\n", path);
        fclose(h->file);
        free(h->path);
        free(h);
        return NULL;
    }

    return h;
}

/*
 * Insere fisicamente varrendo os slots do bucket principal.
 * Se todos lotados, implementaremos expansao no disco.
 */
void insertHashfile(Hashfile hf, char* chave, void* data) {
    if (!hf || !chave || !data) return;
    hashfile_info* h = (hashfile_info*)hf;

    unsigned int bucketIdx = hashFn(chave, h->header.capacity);
    long currentOffset = calcBucketOffset(h, bucketIdx);

    long sizePerSlot = sizeof(SlotHeader) + h->header.sizeStruct;

    while (currentOffset != -1) {
        fseek(h->file, currentOffset, SEEK_SET);

        int count;
        long nextOverflow;
        fread(&count, sizeof(int), 1, h->file);
        fread(&nextOverflow, sizeof(long), 1, h->file);

        if (count < h->header.bucketSize) {
            // Tem espaco nete bucket. Acha o slot livre (active==false)
            for (int s = 0; s < h->header.bucketSize; s++) {
                long slotOffset = currentOffset + sizeof(int) + sizeof(long) + (s * sizePerSlot);
                fseek(h->file, slotOffset, SEEK_SET);

                SlotHeader slotH;
                fread(&slotH, sizeof(SlotHeader), 1, h->file);

                if (!slotH.active) {
                    // Achou slot livre!
                    // Sobrepoe header do slot:
                    slotH.active = true;
                    strncpy(slotH.key, chave, MAX_KEY_LEN - 1);
                    slotH.key[MAX_KEY_LEN - 1] = '\0';

                    fseek(h->file, slotOffset, SEEK_SET);
                    fwrite(&slotH, sizeof(SlotHeader), 1, h->file);
                    
                    // Sobrepoe os bits do struct do data imediatamente a frente:
                    fwrite(data, h->header.sizeStruct, 1, h->file);

                    // Incrementar counter
                    count++;
                    fseek(h->file, currentOffset, SEEK_SET);
                    fwrite(&count, sizeof(int), 1, h->file);

                    // Header counter global
                    h->header.numRecords++;
                    fseek(h->file, 0, SEEK_SET);
                    fwrite(&h->header, sizeof(HashHeader), 1, h->file);
                    
                    fflush(h->file);
                    return;
                }
            }
        }
        
        // Se chegou aqui, bucket atual esta LOTADO.
        if (nextOverflow == -1) {
            // Criar bucket de OVERFLOW no EOF
            fseek(h->file, 0, SEEK_END);
            long newBucketOffset = ftell(h->file);

            // Escreve o bucket vazio no EOF
            int zeroCount = 0;
            long endOverflow = -1;
            fwrite(&zeroCount, sizeof(int), 1, h->file);
            fwrite(&endOverflow, sizeof(long), 1, h->file);
            
            void* emptySlot  = calloc(1, sizePerSlot);
            for (int s = 0; s < h->header.bucketSize; s++) {
                fwrite(emptySlot, sizePerSlot, 1, h->file);
            }
            free(emptySlot);

            // Volta no offset atual e linka `nextOverflow` com o novo bloco
            fseek(h->file, currentOffset + sizeof(int), SEEK_SET);
            fwrite(&newBucketOffset, sizeof(long), 1, h->file);

            // A proxima iteracao do while vai cair no novo bloco vazio
            nextOverflow = newBucketOffset;
        }

        currentOffset = nextOverflow;
    }
}

void* getHashfile(Hashfile hf, char* chave) {
    if (!hf || !chave) return NULL;
    hashfile_info* h = (hashfile_info*)hf;
    
    unsigned int bucketIdx = hashFn(chave, h->header.capacity);
    long currentOffset = calcBucketOffset(h, bucketIdx);
    long sizePerSlot = sizeof(SlotHeader) + h->header.sizeStruct;

    while (currentOffset != -1) {
        fseek(h->file, currentOffset, SEEK_SET);
        int count;
        long nextOverflow;
        fread(&count, sizeof(int), 1, h->file);
        fread(&nextOverflow, sizeof(long), 1, h->file);

        if (count > 0) {
            for (int s = 0; s < h->header.bucketSize; s++) {
                long slotOffset = currentOffset + sizeof(int) + sizeof(long) + (s * sizePerSlot);
                fseek(h->file, slotOffset, SEEK_SET);
                
                SlotHeader slotH;
                fread(&slotH, sizeof(SlotHeader), 1, h->file);

                if (slotH.active && strcmp(slotH.key, chave) == 0) {
                    // Achou! Ler os proximos N bytes e retornar allocado.
                    void* result = malloc(h->header.sizeStruct);
                    assert(result != NULL);
                    fread(result, h->header.sizeStruct, 1, h->file);
                    return result;
                }
            }
        }
        currentOffset = nextOverflow;
    }

    return NULL; 
}

bool removeHashfile(Hashfile hf, char* chave) {
    if (!hf || !chave) return false;
    hashfile_info* h = (hashfile_info*)hf;

    unsigned int bucketIdx = hashFn(chave, h->header.capacity);
    long currentOffset = calcBucketOffset(h, bucketIdx);
    long sizePerSlot = sizeof(SlotHeader) + h->header.sizeStruct;

    while (currentOffset != -1) {
        fseek(h->file, currentOffset, SEEK_SET);
        int count;
        long nextOverflow;
        fread(&count, sizeof(int), 1, h->file);
        fread(&nextOverflow, sizeof(long), 1, h->file);

        if (count > 0) {
            for (int s = 0; s < h->header.bucketSize; s++) {
                long slotOffset = currentOffset + sizeof(int) + sizeof(long) + (s * sizePerSlot);
                fseek(h->file, slotOffset, SEEK_SET);
                
                SlotHeader slotH;
                fread(&slotH, sizeof(SlotHeader), 1, h->file);

                if (slotH.active && strcmp(slotH.key, chave) == 0) {
                    // Achou! Marca como tombstone (active = false)
                    slotH.active = false;
                    fseek(h->file, slotOffset, SEEK_SET);
                    fwrite(&slotH, sizeof(SlotHeader), 1, h->file);

                    // Decrementa contadores
                    count--;
                    fseek(h->file, currentOffset, SEEK_SET);
                    fwrite(&count, sizeof(int), 1, h->file);

                    h->header.numRecords--;
                    fseek(h->file, 0, SEEK_SET);
                    fwrite(&h->header, sizeof(HashHeader), 1, h->file);

                    fflush(h->file);
                    return true;
                }
            }
        }
        currentOffset = nextOverflow;
    }

    return false;
}

void iterateHashfile(Hashfile hf, void (*callback)(char* key, void* data, void* extra1, void* extra2), void* extra1, void* extra2) {
    if (!hf || !callback) return;
    hashfile_info* h = (hashfile_info*)hf;

    long sizePerSlot = sizeof(SlotHeader) + h->header.sizeStruct;

    for (int i = 0; i < h->header.capacity; i++) {
        long currentOffset = calcBucketOffset(h, i);

        while (currentOffset != -1) {
            fseek(h->file, currentOffset, SEEK_SET);
            int count;
            long nextOverflow;
            fread(&count, sizeof(int), 1, h->file);
            fread(&nextOverflow, sizeof(long), 1, h->file);

            if (count > 0) {
                for (int s = 0; s < h->header.bucketSize; s++) {
                    long slotOffset = currentOffset + sizeof(int) + sizeof(long) + (s * sizePerSlot);
                    fseek(h->file, slotOffset, SEEK_SET);
                    
                    SlotHeader slotH;
                    fread(&slotH, sizeof(SlotHeader), 1, h->file);

                    if (slotH.active) {
                        void* data = malloc(h->header.sizeStruct);
                        assert(data != NULL);
                        fread(data, h->header.sizeStruct, 1, h->file);
                        
                        callback(slotH.key, data, extra1, extra2);
                        free(data);
                    }
                }
            }
            currentOffset = nextOverflow;
        }
    }
}

void dumpHashfileTxt(Hashfile hf, char* txtPath) {
    if (!hf || !txtPath) return;
    hashfile_info* h = (hashfile_info*)hf;
    
    FILE* out = fopen(txtPath, "w");
    if (!out) return;

    fprintf(out, "=== DUMP HASHFILE DINAMICO ===\n");
    fprintf(out, "Capacidade Primaria (Buckets): %d\n", h->header.capacity);
    fprintf(out, "Capacidade do Bucket (Slots): %d\n", h->header.bucketSize);
    fprintf(out, "Tamanho util do Slot (bytes): %zu\n", h->header.sizeStruct);
    fprintf(out, "Total de Registros Ativos: %ld\n\n", h->header.numRecords);

    long sizePerSlot = sizeof(SlotHeader) + h->header.sizeStruct;

    for (int i = 0; i < h->header.capacity; i++) {
        long currentOffset = calcBucketOffset(h, i);
        int listLevel = 0;

        fprintf(out, "Bucket[%03d] ", i);

        while (currentOffset != -1) {
            if (listLevel > 0) fprintf(out, "  -> Overflow @0x%lx ", currentOffset);

            fseek(h->file, currentOffset, SEEK_SET);
            int count;
            long nextOverflow;
            fread(&count, sizeof(int), 1, h->file);
            fread(&nextOverflow, sizeof(long), 1, h->file);

            fprintf(out, "(Count: %d/%d): [", count, h->header.bucketSize);

            int printed = 0;
            for (int s = 0; s < h->header.bucketSize; s++) {
                long slotOffset = currentOffset + sizeof(int) + sizeof(long) + (s * sizePerSlot);
                fseek(h->file, slotOffset, SEEK_SET);
                
                SlotHeader slotH;
                fread(&slotH, sizeof(SlotHeader), 1, h->file);

                if (slotH.active) {
                    if (printed > 0) fprintf(out, ", ");
                    fprintf(out, "%s", slotH.key);
                    printed++;
                }
            }
            fprintf(out, "]\n");

            currentOffset = nextOverflow;
            listLevel++;
        }
    }

    fprintf(out, "==============================\n");
    fclose(out);
}

void closeHashfile(Hashfile hf) {
    if (!hf) return;
    hashfile_info* h = (hashfile_info*)hf;
    if (h->file) fclose(h->file);
    if (h->path) free(h->path);
    free(h);
}
