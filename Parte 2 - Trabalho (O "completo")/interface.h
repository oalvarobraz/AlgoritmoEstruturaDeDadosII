//
// Created by Alvaro on 06/03/2023.
//

#ifndef PARTE2_TRABALHO_INTERFACE_H
#define PARTE2_TRABALHO_INTERFACE_H

#include "arvore_b_mais.h"
#include "funcionario.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void MSG_MENU();

void MSG_BUSCA();

void retira_barraN(char* str);

//void lendo_informacoes_funcionario(int ordem);

void lendo_informacoes_funcionario(int ordem, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados);

void opcao_busca();

//void MENU(FILE *arq);

void MENU(char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados);

#endif //PARTE2_TRABALHO_INTERFACE_H
