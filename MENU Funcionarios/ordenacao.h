#ifndef TRABALHOAEDS_ORDENACAO_H
#define TRABALHOAEDS_ORDENACAO_H

#include <stdio.h>
#include "lista.h"
#include "funcionario.h"


typedef struct vetor {
    TFunc *tfunc;
    FILE *file;
} TVet;

// Insertion Sort disco: Maior capacidade de armazenamento, entretanto mais lento
void insertion_sort_disco(FILE *arq, int tam);

// Insertion Sort memória: Menor capacidade de armazenamento, entretanto mais rápido
void insertion_sort(FILE *arq, int tam);

// Sobrescreve um funcionario
void sobrescreve_funcionario(FILE *in, int posicao, TFunc *f1);

// Fisher-yates algoritmo de aleatorizacao
void shuffle(FILE *in);

int intercalacao_basico(char *nome_arquivo, int np, Lista *aux);

void selecao_com_substituicao(FILE *arq, Lista *nome_arquivos_saida, int M);

void intercalacao_otima(char *nome_arquivo_saida, int num_p, Lista *nome_particoes, int f);

#endif //TRABALHOAEDS_ORDENACAO_H
