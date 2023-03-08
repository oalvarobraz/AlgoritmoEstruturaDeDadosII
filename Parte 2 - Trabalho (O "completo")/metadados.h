//
// Created by Alvaro on 05/03/2023.
//

#ifndef PARTE2_TRABALHO_METADADOS_H
#define PARTE2_TRABALHO_METADADOS_H


#include <stdio.h>

/*
 *  Um arquivo para armazenar os metadados
 *  -> Ponteiro para a raiz da árvore
 *  -> Flag indicando se a raiz é folha
 */

typedef struct metadados {
    //ordem da arvore
    int d;
    // ponteiro para a raiz
    int pont_raiz;
    // informa se a raiz é uma folha (flag)
    int raiz_folha;
    // ponteiro para o proximo noh interno livre
    int pont_prox_no_interno_livre;
    // ponteiro para o proximo noh folha livre
    int pont_prox_no_folha_livre;
} TMDados;


// Cria metadados. Lembrar de usar free(tmdados)
TMDados *mdados(int d, int pont_raiz, int raiz_folha, int pont_prox_no_interno_livre, int pont_prox_no_folha_livre);

// Salva mdados no arquivo out, na posicao atual do cursor
void salva_mdados(TMDados *dados, FILE *out);

// Salva mdados no arquivo nome_arquivo
void salva_arq_mdados(char *nome_arquivo, TMDados *dados);

// Le um mdados do arquivo in na posicao atual do cursor
// Retorna um ponteiro para mdados lido do arquivo
TMDados *le_mdados(FILE *in);

// Le mdados do arquivo nome_arquivo
TMDados *le_arq_mdados(char *nome_arquivo);

// Retorna tamanho do mdados em bytes
int tamanho_mdados();


#endif //PARTE2_TRABALHO_METADADOS_H
