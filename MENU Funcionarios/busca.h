//
// Created by Alvaro on 18/02/2023.
//

#ifndef TRABALHO1_BUSCA_H
#define TRABALHO1_BUSCA_H

/*
 Dentro deste arquivo estão as declarações das funções, além disso eu importei algumas bibliotecas
 para o funcionamento do meu código como a funcionario.h, para que eu pudesse utilizar a strcut Func
 e também a time.h, que foi utilizada para calcular o tempo de execução do algoritmo, eu podia ter
 calculado o tempo de execução dentro ou fora do algoritmo, entretanto eu preferi calcular dentro do
 algoritmo mesmo

 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "funcionario.h"

typedef struct Busca{
    int cod;
    char nome[50];
    double salario;
    int comparacoes;
    double tempo;
} TBusca;

// Imprime busca
void imprimeBusca(TBusca *busca);

// Cria busca. Lembrar de usar free(busca)
TBusca *busca(int cod, char *nome, double salario, int comparacoes, double tempo);

// Salva busca no arquivo out, na posicao atual do cursor
void salva_busca(TBusca *busca, FILE *out);

// Le a busca do arquivo in na posicao atual do cursor
// Retorna um ponteiro para a busca lida do arquivo
TBusca *le_busca(FILE *in);

// Retorna tamanho da busca em bytes
int tamanho_busca();

// Imprime arquivo busca
void imprime_arquivo_busca(FILE *arq);

// Busca binária
TFunc *busca_binaria(int chave, FILE *in, int inicio, int fim, FILE *arqBusca);

// Busca sequencial
TFunc *busca_sequencial(FILE *in, int codigo, FILE *arq_busca);

#endif //TRABALHO1_BUSCA_H
