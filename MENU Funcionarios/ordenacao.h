//
// Created by Alvaro on 18/02/2023.
//

#ifndef TRABALHO1_ORDENACAO_H
#define TRABALHO1_ORDENACAO_H

#include <stdio.h>
#include "lista.h"
#include "funcionario.h"


typedef struct vetor {
    TFunc *tfunc;
    FILE *file;
} TVet;

typedef struct Ordenacao {
    int comparacoes;
    double tempo;
} TOrden;

// Imprime ordenacao
void imprimeOrdenacao(TOrden *orden);

// Cria ordenacao. Lembrar de usar free(ordenacao)
TOrden *orden(int comparacoes, double tempo);

// Salva ordenacao no arquivo out, na posicao atual do cursor
void salva_orden(TOrden *orden, FILE *out);

// Le a ordenacao do arquivo in na posicao atual do cursor
// Retorna um ponteiro para a ordenacao lida do arquivo
TOrden *le_orden(FILE *in);

// Retorna tamanho da ordencao em bytes
int tamanho_ordenacao();

// Retorna o tamanho do arquivo
int tamanho_arquivo_ordenacao(FILE *arq);

// Imprime arquivo ordencao
void imprime_arquivo_ordenacao(FILE *arq);

// Insertion Sort disco: Maior capacidade de armazenamento, entretanto mais lento
void insertion_sort_disco(FILE *arq, int tam);

// Insertion Sort memória: Menor capacidade de armazenamento, entretanto mais rápido
void insertion_sort(FILE *arq, int tam);

// Sobrescreve um funcionario
void sobrescreve_funcionario(FILE *in, int posicao, TFunc *f1);

// Fisher-yates algoritmo de aleatorizacao
void shuffle(FILE *in);

void classificacao_interna(char *nome_arquivo_entrada, Lista *nome_arquivos_saida, int M);

void intercalacao_basico(char *nome_arquivo, int np, Lista *aux);

void selecao_com_substituicao(FILE *arq, Lista *nome_arquivos_saida, int M);

void intercalacao_otima(char *nome_arquivo_saida, int num_p, Lista *nome_particoes, int f);


#endif //TRABALHO1_ORDENACAO_H
