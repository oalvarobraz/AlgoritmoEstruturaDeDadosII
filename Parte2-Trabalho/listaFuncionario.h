//
// Created by Alvaro on 05/03/2023.
//

#ifndef PARTE2_TRABALHO_LISTAFUNCIONARIO_H
#define PARTE2_TRABALHO_LISTAFUNCIONARIO_H


#include "funcionario.h"

typedef struct ListaFunc {
    TFunc **lista;
    int qtd;
} TListaFunc;

// Imprime funcionarios
void imprime_funcs(TListaFunc *l);

// Cria lista de funcionarios. Lembrar de usar libera_funcionarios(TListaFunc)
TListaFunc *cria_funcionarios(int qtd, ...);

// Salva lista de funcionarios no arquivo nome_arquivo
void salva_funcs(char *nome_arquivo, TListaFunc *l);

// Le lista de pizzas do arquivo nome_arquivo
TListaFunc *le_funcs(char *nome_arquivo);

// Compara duas listas de pizzas
// Retorna 1 se as pizzas das duas listas forem iguais
// e 0 caso contrario
int cmp_funcs(TListaFunc *l1, TListaFunc *l2);

// Desaloca lista de pizzas
void libera_funcs(TListaFunc *l);

#endif //PARTE2_TRABALHO_LISTAFUNCIONARIO_H
