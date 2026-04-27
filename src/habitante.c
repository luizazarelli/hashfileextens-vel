#include "habitante.h"

typedef struct {
    char cep[32];
    char face;
    double num;
    char compl[32];
} Endereco;

typedef struct {
    char cpf[16];
    char nome[32];
    char sobrenome[32];
    char sexo;
    char nasc[16];
    bool is_morador;
    Endereco endereco;
} habitante_info;


Habitante createHabitante(char* cpf, char* nome, char* sobrenome, char sexo, char* nasc) {
    habitante_info* h = calloc(1, sizeof(habitante_info));
    assert(h != NULL);

    if (cpf) strncpy(h->cpf, cpf, 15);
    if (nome) strncpy(h->nome, nome, 31);
    if (sobrenome) strncpy(h->sobrenome, sobrenome, 31);
    if (nasc) strncpy(h->nasc, nasc, 15);
    h->sexo = sexo;
    h->is_morador = false;

    return h;
}

void freeHabitante(Habitante hab) {
    if (!hab) return;
    free(hab);
}

void setHabitanteEndereco(Habitante hab, char* cep, char face, double num, char* compl) {
    if (!hab) return;
    habitante_info* h = (habitante_info*)hab;

    h->is_morador = true;
    if (cep) strncpy(h->endereco.cep, cep, 31);
    h->endereco.face = face;
    h->endereco.num = num;
    if (compl) strncpy(h->endereco.compl, compl, 31); else h->endereco.compl[0] = '\0';
}

void removerEnderecoHabitante(Habitante hab) {
    if (!hab) return;
    habitante_info* h = (habitante_info*)hab;
    h->is_morador = false;
}

bool isMorador(Habitante hab) {
    if (!hab) return false;
    habitante_info* h = (habitante_info*)hab;
    return h->is_morador;
}

char* getHabitanteCpf(Habitante h)       { return ((habitante_info*)h)->cpf; }
char* getHabitanteNome(Habitante h)      { return ((habitante_info*)h)->nome; }
char* getHabitanteSobrenome(Habitante h) { return ((habitante_info*)h)->sobrenome; }
char  getHabitanteSexo(Habitante h)      { return ((habitante_info*)h)->sexo; }
char* getHabitanteNasc(Habitante h)      { return ((habitante_info*)h)->nasc; }

char* getHabitanteEndCep(Habitante h)   { return ((habitante_info*)h)->endereco.cep; }
char  getHabitanteEndFace(Habitante h)  { return ((habitante_info*)h)->endereco.face; }
double getHabitanteEndNum(Habitante h)  { return ((habitante_info*)h)->endereco.num; }
char* getHabitanteEndCompl(Habitante h) { return ((habitante_info*)h)->endereco.compl; }

size_t getHabitanteSize() {
    return sizeof(habitante_info);
}
