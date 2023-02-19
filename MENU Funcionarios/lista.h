#ifndef TRABALHOAEDS_LISTA_H
#define TRABALHOAEDS_LISTA_H

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

#endif //TRABALHOAEDS_LISTA_H
