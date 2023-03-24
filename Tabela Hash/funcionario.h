//
// Created by Alvaro on 22/03/2023.
//

#ifndef HASH_TABLE_FUNCIONARIO_H
#define HASH_TABLE_FUNCIONARIO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


typedef struct Funcionario {
    int cod;
    char nome[50];
    double salario;
    int prox;
} TFunc;

// Imprime funcionario
void imprime(TFunc *func);

// Cria funcionario. Lembrar de usar free(funcionario)
TFunc *funcionario(int cod, char *nome, double salario, int prox);

// Salva funcionario no arquivo out, na posicao atual do cursor
void salva(TFunc *func, FILE *out);

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TFunc *le(FILE *in);

// Retorna tamanho do funcionario em bytes
int tamanho();

// Imprime arquivo
void imprime_arquivo(FILE *arq);

// Retorna o tamanho do arquivo
int tamanho_arquivo(FILE *arq);

#endif //HASH_TABLE_FUNCIONARIO_H
