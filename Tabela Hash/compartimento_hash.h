//
// Created by Alvaro on 22/03/2023.
//

#ifndef HASH_TABLE_COMPARTIMENTO_HASH_H
#define HASH_TABLE_COMPARTIMENTO_HASH_H
#include <stdlib.h>
#include <stdio.h>

typedef struct CompartimentoHash {
    int prox;
} CompartimentoHash;

// Cria compartimento hash. Lembrar de usar free(compartimento_hash)
CompartimentoHash *compartimento_hash(int prox);

// Salva compartimento no arquivo out, na posicao atual do cursor
void salva_compartimento(CompartimentoHash *compartimento, FILE *out);

// Le um compartimento do arquivo in na posicao atual do cursor
// Retorna um ponteiro para compartimento lido do arquivo
CompartimentoHash *le_compartimento(FILE *in);


// Retorna tamanho do compartimento hash em bytes
int tamanho_compartimento();


#endif //HASH_TABLE_COMPARTIMENTO_HASH_H
