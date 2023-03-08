//
// Created by Alvaro on 05/03/2023.
//

#ifndef PARTE2_TRABALHO_LISTA_H
#define PARTE2_TRABALHO_LISTA_H

typedef struct Lista {
    char *nome;
    struct Lista *prox;
} Lista;

// Imprime lista de nomes
void imprime_nomes(Lista *nomes);

// Aloca copia de str
char *cria_str(char *str);

// Cria lista encadeada de nomes. Lembrar de chamar libera_nomes(nomes)
Lista *cria_nomes(char *nome, Lista *prox);

// Retorna quantidade de nomes
int conta_nomes(Lista *nomes);

// Adiciona nomes
void adiciona_nomes(Lista *nomes, char *nome);

// Libera lista
void libera_nomes(Lista *nomes);

#endif //PARTE2_TRABALHO_LISTA_H
