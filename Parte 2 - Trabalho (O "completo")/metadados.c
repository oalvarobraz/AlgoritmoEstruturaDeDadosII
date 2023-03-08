//
// Created by Alvaro on 04/03/2023.
//
#include <string.h>
#include <stdlib.h>

#include "metadados.h"



TMDados *mdados(int d, int pont_raiz, int raiz_folha, int pont_prox_no_interno_livre, int pont_prox_no_folha_livre)
{
    TMDados *metadados = (TMDados *) malloc(sizeof(TMDados));
    // inicializa espaço de memória com ZEROS
    if (metadados) memset(metadados, 0, sizeof(TMDados));
    //copia valores para os campos
    metadados->d = d;
    metadados->pont_raiz = pont_raiz;
    metadados->raiz_folha = raiz_folha;
    metadados->pont_prox_no_interno_livre = pont_prox_no_interno_livre;
    metadados->pont_prox_no_folha_livre = pont_prox_no_folha_livre;
    return metadados;
}

void salva_mdados(TMDados *dados, FILE *out)
{
    fwrite(&dados->d, sizeof(int), 1, out);
    fwrite(&dados->pont_raiz, sizeof(int), 1, out);
    fwrite(&dados->raiz_folha, sizeof(int), 1, out);
    fwrite(&dados->pont_prox_no_interno_livre, sizeof(int), 1, out);
    fwrite(&dados->pont_prox_no_folha_livre, sizeof(int), 1, out);
}

void salva_arq_mdados(char *nome_arquivo, TMDados *dados)
{
    FILE *out = fopen(nome_arquivo, "wb");
    salva_mdados(dados, out);
    fclose(out);
}


TMDados *le_mdados(FILE *in)
{
    TMDados *metadados = (TMDados *) malloc(sizeof(TMDados));
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

TMDados *le_arq_mdados(char *nome_arquivo)
{
    FILE *in = fopen(nome_arquivo, "rb");
    if (in != NULL) {
        TMDados *resultado = le_mdados(in);
        fclose(in);
        return resultado;
    }
    else return NULL;
}


int tamanho_mdados()
{
    return sizeof(int) + //d
           sizeof(int) + // pont_raiz
           sizeof(int) + // raiz_folha
           sizeof(int) + // pont_prox_no_interno_livre
           sizeof(int); // pont_prox_no_folha_livre
}