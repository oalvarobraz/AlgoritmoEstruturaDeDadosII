#ifndef TRABALHOAEDS_COPIA_PARA_TESTE_BUSCA_H
#define TRABALHOAEDS_COPIA_PARA_TESTE_BUSCA_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "funcionario.h"


// Busca binária
TFunc *busca_binaria(int chave, FILE *in, int inicio, int fim, FILE *arqBusca);

// Busca sequencial
TFunc *busca_sequencial(FILE *in, int codigo, FILE *arq_busca);

#endif //TRABALHOAEDS_COPIA_PARA_TESTE_BUSCA_H
