//
// Created by Alvaro on 05/03/2023.
//

#ifndef PARTE2_TRABALHO_LISTAFUNCIONARIO_H
#define PARTE2_TRABALHO_LISTAFUNCIONARIO_H


#include "funcionario.h"

typedef struct ListaFunc {
    // Ponteiro de ponteiro lista
    TFunc **lista;
    int qtd;
} TListaFunc;

// Imprime funcionarios
void imprime_funcs(TListaFunc *l);

// Cria lista de funcionarios. Lembrar de usar libera_funcionarios(TListaFunc)
TListaFunc *cria_funcionarios(int qtd, ...);

// Salva lista de funcionarios no arquivo nome_arquivo
void salva_funcs(char *nome_arquivo, TListaFunc *l);

// Le lista de funcionarios do arquivo nome_arquivo
TListaFunc *le_funcs(char *nome_arquivo);

// Desaloca lista de funcionarios
void libera_funcs(TListaFunc *l);

#endif //PARTE2_TRABALHO_LISTAFUNCIONARIO_H
