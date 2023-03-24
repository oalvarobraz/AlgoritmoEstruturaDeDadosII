//
// Created by Alvaro on 22/03/2023.
//

#include "compartimento_hash.h"


CompartimentoHash *compartimento_hash(int prox)
{
    CompartimentoHash *compartimento = (CompartimentoHash *) malloc(sizeof(CompartimentoHash));
    compartimento->prox = prox;
    return compartimento;
}


void salva_compartimento(CompartimentoHash *compartimento, FILE *out)
{
    fwrite(&compartimento->prox, sizeof(int), 1, out);
}

CompartimentoHash *le_compartimento(FILE *in)
{
    CompartimentoHash *compartimento = (CompartimentoHash *) malloc(sizeof(CompartimentoHash));
    if (0 >= fread(&compartimento->prox, sizeof(int), 1, in)) {
        free(compartimento);
        return NULL;
    }
    return compartimento;
}


int tamanho_compartimento()
{
    return sizeof(int); // prox
}