#include <stdlib.h>
#include <string.h>
#include "lista.h"


char *cria_str(char *str)
{
    char *s = (char *) calloc(strlen(str) + 1, sizeof(char));
    strcpy(s, str);
    return s;
}

Lista *cria_nomes(char *nome, Lista *prox)
{
    Lista *nomes = (Lista *) malloc(sizeof(Lista));
    nomes->nome = nome;
    nomes->prox = prox;
    return nomes;
}

int conta_nomes(Lista *nomes)
{
    int qtd = 0;
    while(nomes) {
        qtd += 1;
        nomes = nomes->prox;
    }
    return qtd;
}

void adiciona_nomes(Lista *nomes, char *nome) {
    while(0 < conta_nomes(nomes)-1) {
        nomes = nomes->prox;
    }
    nomes->prox = cria_nomes(cria_str(nome), NULL);
}

void libera_nomes(Lista *nomes)
{
    Lista *temp = NULL;
    while(nomes) {
        temp = nomes->prox;
        free(nomes->nome);
        free(nomes);
        nomes = temp;
    }
}
