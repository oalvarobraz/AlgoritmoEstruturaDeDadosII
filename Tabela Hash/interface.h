//
// Created by Alvaro on 22/03/2023.
//

#ifndef HASH_TABLE_INTERFACE_H
#define HASH_TABLE_INTERFACE_H

#include "funcionario.h"
#include "compartimento_hash.h"
#include "encadeamento_exterior.h"
#include <time.h>

void MSG_MENU(); // MENSAGEM PARA EXIBIR O MENU PRINCIPAL

void MSG_BUSCA();

void MSG_REMOCAO();

void salva_insercao(double tempo, FILE *arq);

void salva_busca(double tempo, TFunc *func, FILE *arq, int search);

void salva_remocao(double tempo, int codigo, FILE* arq, int search);

//void MENU(FILE *arq_hash, char* nome_arquivo_dados, int tam);

void MENU(char* nome_arquivo_dados, int tam);

void imprime_arquivo_txt(FILE *arq);
#endif //HASH_TABLE_INTERFACE_H
