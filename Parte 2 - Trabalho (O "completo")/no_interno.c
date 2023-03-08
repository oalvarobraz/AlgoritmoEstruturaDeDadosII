//
// Created by Alvaro on 04/03/2023.
//
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "no_interno.h"


void imprime_no_interno(int d, TNoInterno *no)
{
    int i;
    printf("%d, %d, %d (", no->m, no->pont_pai, no->flag_aponto_folha);
    for (i = 0; i < 2 * d; i++) {
        printf("%d, ", no->pont[i]);
    }
    printf("%d), (", no->pont[i]);
    for (i = 0; i < 2 * d - 1; i++) {
        printf("%d, ", no->chaves[i]);
    }
    printf("%d)\n", no->chaves[i]);
}

TNoInterno *no_interno(int d, int m, int pont_pai, int aponta_folha)
{
    int i;
    TNoInterno *no = (TNoInterno *) malloc(sizeof(TNoInterno));
    //inicializa espaço de memória com ZEROS
    if (no) memset(no, 0, sizeof(TNoInterno));
    //copia valores para os campos
    no->m = m;
    no->pont_pai = pont_pai;
    no->flag_aponto_folha = aponta_folha;
    no->pont = (int *) malloc(sizeof(int) * (2 * d + 1));
    no->chaves = (int *) malloc(sizeof(int) * (2 * d));
    for (i = 0; i < 2 * d; i++) {
        no->pont[i] = -1;
        no->chaves[i] = -1;
    }
    no->pont[2 * d] = -1;
    return no;
}

TNoInterno *no_interno_vazio(int d)
{
    return no_interno(d, 0, -1, 0);
}

TNoInterno *cria_no_interno(int d, int m, int pont_pai, int aponta_folha, int size, ...)
{
    TNoInterno *n = no_interno(d, m, pont_pai, aponta_folha);
    int i;
    va_list ap;
    va_start(ap, size);
    for (i = 0; i < n->m + 1; i++) {
        n->pont[i] = va_arg(ap, int);
    }
    for (i = 0; i < n->m; i++) {
        n->chaves[i] = va_arg(ap, int);
    }
    va_end(ap);
    return n;
}

void salva_no_interno(int d, TNoInterno *no, FILE *out)
{
    int i;
    fwrite(&no->m, sizeof(int), 1, out);
    fwrite(&no->pont_pai, sizeof(int), 1, out);
    fwrite(&no->flag_aponto_folha, sizeof(int), 1, out);
    //garantidamente, sempre havera pelo menos 1 chave no noh
    //portanto, p0 sempre vai existir
    fwrite(&no->pont[0], sizeof(int), 1, out);

    for (i = 0; i < 2 * d; i++) {
        fwrite(&no->chaves[i], sizeof(int), 1, out);
        fwrite(&no->pont[i + 1], sizeof(int), 1, out);
    }
}

TNoInterno *le_no_interno(int d, FILE *in)
{
    int i;
    TNoInterno *no = (TNoInterno *) malloc(sizeof(TNoInterno));
    if (0 >= fread(&no->m, sizeof(int), 1, in)) {
        free(no);
        return NULL;
    }
    fread(&no->pont_pai, sizeof(int), 1, in);
    fread(&no->flag_aponto_folha, sizeof(int), 1, in);
    no->pont = (int *) malloc(sizeof(int) * (2 * d + 1));
    no->chaves = (int *) malloc(sizeof(int) * (2 * d));

    fread(&no->pont[0], sizeof(int), 1, in);
    for (i = 0; i < 2 * d; i++) {
        fread(&no->chaves[i], sizeof(int), 1, in);
        fread(&no->pont[i + 1], sizeof(int), 1, in);
    }
    return no;
}

int cmp_no_interno(int d, TNoInterno *n1, TNoInterno *n2)
{
    int i;
    if (n1 == NULL) {
        return (n2 == NULL);
    }
    if (n1->m != n2->m) {
        return 0;
    }
    if (n1->pont_pai != n2->pont_pai) {
        return 0;
    }
    if (n1->flag_aponto_folha != n2->flag_aponto_folha) {
        return 0;
    }
    for (i = 0; i < n1->m+1; i++) {
        if (n1->pont[i] != n2->pont[i]) {
            return 0;
        }
    }
    for (i = 0; i < n1->m; i++) {
        if (n1->chaves[i] != n2->chaves[i]) {
            return 0;
        }
    }
    return 1;
}

int tamanho_no_interno(int d){
    return sizeof(int)  // m
           + sizeof(int) // pont_pai
           + sizeof(int) // flag_aponto_folha
           + sizeof(int) * (2 * d + 1) // pont
           + sizeof(int) * (2 * d); // chaves
}

void libera_no_interno(TNoInterno *no)
{
    free(no->pont);
    free(no->chaves);
    free(no);
}