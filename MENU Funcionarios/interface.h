#ifndef TRABALHOAEDS_INTERFACE_H
#define TRABALHOAEDS_INTERFACE_H

#include "funcionario.h"

void MSG_MENU(); // MENSAGEM PARA EXIBIR O MENU PRINCIPAL

// Retira \n do final da string
void retira_barraN(char *str);

// Le informacoes de um funcionario e salva dentro de um arquivo
void lendo_informacoes_funcionario(FILE *arq);

// Menu principla
void MENU(FILE *arq);

// Mensagem para busca
void MSG_BUSCA();

// Mensagem para ordenação
void MSG_ORDENA();

// Menu secundario para imprimir resultados da busca
void opcao_busca();

// Menu secundario para imprimir resultados da ordenação
void opcao_ordena();

// Função para imprimir um arquivo txt
void imprime_arquivo_txt(FILE *arq);

#endif //TRABALHOAEDS_INTERFACE_H
