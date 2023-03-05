//
// Created by Alvaro on 05/03/2023.
//

#ifndef PARTE2_TRABALHO_DADOS_INTERNOS_H
#define PARTE2_TRABALHO_DADOS_INTERNOS_H


#include <stdio.h>

typedef struct DadosInternos {
    //ordem da arvore
    int d;
    // ponteiro para a raiz da arvore
    int pont_raiz;
    // flag que diz se a raiz eh uma folha (nesse caso ela esta no arquivo de dados)
    int raiz_folha;
    // ponteiro para o proximo noh interno livre
    int pont_prox_no_interno_livre;
    // ponteiro para o proximo noh folha livre
    int pont_prox_no_folha_livre;
} TDadosInternos;

// Imprime dados
void imprime_dados(TDadosInternos *dados);

// Cria metadados. Lembrar de usar free(dados)
TDadosInternos *dados(int d, int pont_raiz, int raiz_folha, int pont_prox_no_interno_livre, int pont_prox_no_folha_livre);

// Cria dados vazio. Lembrar de usar free(dados)
TDadosInternos *dados_vazio(int d);

// Salva dados no arquivo out, na posicao atual do cursor
void salva_dados(TDadosInternos *dados, FILE *out);

// Salva dados no arquivo nome_arquivo
void salva_arq_dados(char *nome_arquivo, TDadosInternos *dados);

// Le um dados do arquivo in na posicao atual do cursor
// Retorna um ponteiro para dados lido do arquivo
TDadosInternos *le_dados(FILE *in);

// Le dados do arquivo nome_arquivo
TDadosInternos *le_arq_dados(char *nome_arquivo);

// Compara dois dados
// Retorna 1 se os valores dos atributos de ambos forem iguais
// e 0 caso contrario
int cmp_dados(int d, TDadosInternos *m1, TDadosInternos *m2);

// Retorna tamanho do dados em bytes
int tamanho_dados();


#endif //PARTE2_TRABALHO_DADOS_INTERNOS_H
