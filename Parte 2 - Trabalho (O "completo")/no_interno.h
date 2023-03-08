//
// Created by Alvaro on 05/03/2023.
//

#ifndef PARTE2_TRABALHO_NO_INTERNO_H
#define PARTE2_TRABALHO_NO_INTERNO_H


#include <stdio.h>

typedef struct NoInterno {
    // m eh o numero total de chaves armazenados no noh
    int m;
    // ponteiro para o nó pai
    int pont_pai;
    // informa se o nó aponta para a folha ou não
    int flag_aponto_folha;
    // essa lista tera sempre m + 1 ponteiros
    int *pont;
    // essa lista tera sempre m chaves
    int *chaves;
} TNoInterno;

// Imprime nó interno
void imprime_no_interno(int d, TNoInterno *no);

// Cria nó interno. Lembrar de usar libera_no_interno(no)
TNoInterno *no_interno(int d, int m, int pont_pai, int aponta_folha);

// Cria nó interno vazio
TNoInterno *no_interno_vazio(int d);

// Cria nó interno com mdados
// tamanho deve ser 2m + 1
TNoInterno *cria_no_interno(int d, int m, int pont_pai, int aponta_folha, int size, ...);

// Grava uma pagina (nó) no arquivo em disco
void salva_no_interno(int d, TNoInterno *no, FILE *out);

// Le uma pagina (nó) do disco
TNoInterno *le_no_interno(int d, FILE *in);

// Compara dois nós internos
int cmp_no_interno(int d, TNoInterno *n1, TNoInterno *n2);

// Retorna tamanho do nó interno em bytes
int tamanho_no_interno(int d);

// Libera nó interno
void libera_no_interno(TNoInterno *no);

#endif //PARTE2_TRABALHO_NO_INTERNO_H
