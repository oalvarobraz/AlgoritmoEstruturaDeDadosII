//
// Created by Alvaro on 05/03/2023.
//

#ifndef PARTE2_TRABALHO_ARVORE_B_MAIS_H
#define PARTE2_TRABALHO_ARVORE_B_MAIS_H

#include "listaFuncionario.h"
#include "metadados.h"
#include "no_folha.h"
#include "lista_nos_internos.h"


//Preenche o arquivo metadados se ele estiver vazio.
void checa_mdados(char *nome_arquivo_metadados, int d);

//Insere um no interno
void insereNoInterno(int cod, int d, char *nome_arquivo_indice, FILE *arq_Dados , TMDados *metadados, int pos, int flag);

//Insere um no folha
void insereNoFolha(TNoFolha *no, int cod, TFunc *p, int d, FILE *arq_Dados, TMDados *metadados, int pos, char *nome_arquivo_indice);

//retorna o elemento e a posição dele
int busca_binaria_arvore(void *r, int *pos, int chave, int esq, int dir, int folha);

//Verifica se o codigo já existe
int buscaFolha(int pos, char *nome_arquivo_dados, int cod, int d);

int busca(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d);

int insere(int cod, char *nome, double salario, char *nome_arquivo_metadados, char *nome_arquivo_indice,char *nome_arquivo_dados, int d);

//Remove uma funcionario do no folha
TNoFolha *retira_funcionario(TNoFolha *no, int pos);

TNoInterno *retira_interno(TNoInterno *no, int pos);

//Move as func
TNoFolha *shift(TNoFolha *no);

//Acho que nao sera necessaria.
TNoInterno *shift_interno(TNoInterno *pai);

TNoInterno * get_irmao_op_interno(int d,FILE *dados,TNoInterno *pai, TNoInterno *no, int pos, int *op, int *index);

//Retorna o irmão e a operacao q tem q ser feita
TNoFolha * get_irmao_op(int d,FILE *dados,TNoInterno *pai, TNoFolha *no, int pos, int *op, int *index);

void redistribui_dir_interno(TNoInterno *dir, TNoInterno *esq, TNoInterno *pai);

void redistribui_esq_interno(TNoInterno *dir, TNoInterno *esq, TNoInterno *pai);

void redistribui_dir(TNoFolha *dir, TNoFolha *esq, TNoInterno *pai);

//Redistribui o irmão da esquerda pro da direita
void redistribui_esq(TNoFolha *dir, TNoFolha *esq, TNoInterno *pai);

int q_filhos(TNoInterno *pai);

void concatena_interno(TNoInterno *w, TNoInterno *irmao, TNoInterno *pai, int d, int op, int index, FILE *dados, FILE *indice);

void concatena(TNoFolha *no, TNoFolha *irmao, TNoInterno *pai, int d, int op, int ind_pai ,int *index, FILE *dados);

void exclui_interno(int d, FILE *indice, TNoInterno *w, TNoInterno *pai, TNoInterno *irmao, int op, int index, char *nome_arquivo_metadados, FILE *dados);

int exclui(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d);

void carrega_dados(int d, char *nome_arquivo_entrada, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados);

//Imprime as func
int imprimirArvore(int d, char *nome_arquivo_dados, char *nome_arquivo_indice, char *nome_arquivo_metadados);

//Verifica se o codigo da funcionario existe
int buscaFuncionario(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d);

//Modifica a funcionario
void modifica(int cod, char *nome, double salario, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d);

#endif //PARTE2_TRABALHO_ARVORE_B_MAIS_H
