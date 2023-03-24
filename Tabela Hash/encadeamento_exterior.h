//
// Created by Alvaro on 22/03/2023.
//

#ifndef HASH_TABLE_ENCADEAMENTO_EXTERIOR_H
#define HASH_TABLE_ENCADEAMENTO_EXTERIOR_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "funcionario.h"
#include "compartimento_hash.h"

// A função "hash" é responsável por calcular o endereço da tabela hash
int hash(int codigo, int tam);

/*
 A função "cria_hash" é responsável por criar o arquivo que irá armazenar a tabela hash.
 Essa função deve criar um arquivo binário com tamanho fixo e inicializar todos os ponteiros
 para as listas encadeadas com valor -1.
 */
void cria_hash(char *nome_arquivo_hash, int tam);

/*
 A função "busca" é responsável por buscar um registro de funcionário na tabela hash
 e na lista encadeada correspondente. Essa função deve utilizar a função "hash" para
 calcular o endereço na tabela hash, e em seguida, buscar o registro correspondente no
 arquivo da lista encadeada.
 */
int busca(int codig, FILE* arq_hash, char* nome_arquivo_dados, int tam);

int insere(TFunc *func, FILE *arq_hash, char *nome_arquivo_dados, int tam);

int exclui(int cod, FILE *arq_hash, char *nome_arquivo_dados, int tam);

void imprimir_hash(FILE *arq_hash, char* nome_arquivo_dados, int tam);

#endif //HASH_TABLE_ENCADEAMENTO_EXTERIOR_H
