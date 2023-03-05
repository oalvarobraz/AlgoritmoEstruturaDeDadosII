//
// Created by Alvaro on 04/03/2023.
//
#include <string.h>
#include <stdlib.h>

#include "dados_internos.h"
#include "no_folha.h"


void imprime_dados(TDadosInternos *dados)
{
    printf("%d, %d, %d, %d, %d\n", dados->d, dados->pont_raiz, dados->raiz_folha,
           dados->pont_prox_no_interno_livre, dados->pont_prox_no_folha_livre);
}

TDadosInternos *dados(int d, int pont_raiz, int raiz_folha, int pont_prox_no_interno_livre, int pont_prox_no_folha_livre)
{
    TDadosInternos *metadados = (TDadosInternos *) malloc(sizeof(TDadosInternos));
    if (metadados) memset(metadados, 0, sizeof(TDadosInternos));
    metadados->d = d;
    metadados->pont_raiz = pont_raiz;
    metadados->raiz_folha = raiz_folha;
    metadados->pont_prox_no_interno_livre = pont_prox_no_interno_livre;
    metadados->pont_prox_no_folha_livre = pont_prox_no_folha_livre;
    return metadados;
}

TDadosInternos *dados_vazio(int d)
{
    return dados(d, 0, 1, 0, tamanho_no_folha(d));
}

void salva_dados(TDadosInternos *metadados, FILE *out)
{
    fwrite(&metadados->d, sizeof(int), 1, out);
    fwrite(&metadados->pont_raiz, sizeof(int), 1, out);
    fwrite(&metadados->raiz_folha, sizeof(int), 1, out);
    fwrite(&metadados->pont_prox_no_interno_livre, sizeof(int), 1, out);
    fwrite(&metadados->pont_prox_no_folha_livre, sizeof(int), 1, out);
}

void salva_arq_dados(char *nome_arquivo, TDadosInternos *metadados)
{
    FILE *out = fopen(nome_arquivo, "wb");
    salva_dados(metadados, out);
    fclose(out);
}


TDadosInternos *le_dados(FILE *in)
{
    TDadosInternos *metadados = (TDadosInternos *) malloc(sizeof(TDadosInternos));
    if (0 >= fread(&metadados->d, sizeof(int), 1, in)) {
        free(metadados);
        return NULL;
    }
    fread(&metadados->pont_raiz, sizeof(int), 1, in);
    fread(&metadados->raiz_folha, sizeof(int), 1, in);
    fread(&metadados->pont_prox_no_interno_livre, sizeof(int), 1, in);
    fread(&metadados->pont_prox_no_folha_livre, sizeof(int), 1, in);
    return metadados;
}

TDadosInternos *le_arq_dados(char *nome_arquivo)
{
    FILE *in = fopen(nome_arquivo, "rb");
    if (in != NULL) {
        TDadosInternos *resultado = le_dados(in);
        fclose(in);
        return resultado;
    }
    else return NULL;
}


int cmp_dados(int d, TDadosInternos *m1, TDadosInternos *m2)
{
    if (m1 == NULL) {
        return (m2 == NULL);
    }
    if (m2 == NULL) {
        return 0;
    }
    if (m1->pont_raiz != m2->pont_raiz) {
        return 0;
    }
    if (m1->raiz_folha != m2->raiz_folha) {
        return 0;
    }
    if (m1->pont_prox_no_interno_livre != m2->pont_prox_no_interno_livre) {
        return 0;
    }
    if (m1->pont_prox_no_folha_livre != m2->pont_prox_no_folha_livre) {
        return 0;
    }
    return 1;
}

int tamanho_dados()
{
    return sizeof(int) + //d
           sizeof(int) + // pont_raiz
           sizeof(int) + // raiz_folha
           sizeof(int) + // pont_prox_no_interno_livre
           sizeof(int); // pont_prox_no_folha_livre
}