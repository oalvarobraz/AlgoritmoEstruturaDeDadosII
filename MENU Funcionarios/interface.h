//
// Created by Alvaro on 18/02/2023.
//

#ifndef TRABALHO1_INTERFACE_H
#define TRABALHO1_INTERFACE_H

#include "funcionario.h"

void MSG_MENU(); // MENSAGEM PARA EXIBIR O MENU PRINCIPAL

void retira_barraN(char *str);

void lendo_informacoes_funcionario(FILE *arq);

void MENU(FILE *arq);

void MSG_BUSCA();

void MSG_ORDENA();

void opcao_busca();

void opcao_ordena();

#endif //TRABALHO1_INTERFACE_H
