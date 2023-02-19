#include "busca.h"
#include "windows.h"

// Busca binária
TFunc *busca_binaria(int chave, FILE *in, int inicio, int fim, FILE *arqBusca) {

    // Variáveis para calcular o tempo
    clock_t begin, end;
    double time_spent = 0.0;
    begin = clock();

    // Variável para calcular a quantidade de comparações
    int comp = 0;

    TFunc *func = NULL;
    int cod = -1;
    while (inicio <= fim && cod != chave) {

        comp++;

        int meio = trunc((inicio + fim) / 2);
        fseek(in, (meio -1) * tamanho(), SEEK_SET);
        func = le(in);
        cod = func->cod;
        if (func) {
            if (cod > chave) {
                fim = meio - 1;
            } else {
                inicio = meio + 1;
            }
        }
    }
    if (cod == chave) {

        end = clock();
        time_spent += (double)(end - begin)/ CLOCKS_PER_SEC;

        fprintf(arqBusca, "\n|| Nome: %s\n", func->nome);
        fprintf(arqBusca, "|| Codigo: %d\n", func->cod);
        fprintf(arqBusca, "|| Salario: %f\n", func->salario);
        fprintf(arqBusca, "|| Total de comparacoes: %d\n", comp);
        fprintf(arqBusca, "|| Tempo gasto: %f\n", time_spent);
        printf("|| Total de comparacoes: %d", comp);
        printf("\n|| Tempo gasto: %f", time_spent);

        return func;
    } else {
        return NULL;
    }
}

// Busca sequencial
TFunc *busca_sequencial(FILE *in, int codigo, FILE *arq_busca){

    // Variáveis para calcular o tempo
    double time_spent = 0.0;
    clock_t begin, end;
    begin = clock();

    // Variável para calcular a quantidade de comparações
    // Nesse caso sera o próprio i do looping

    TFunc *f;

    for (int i = 0; i < tamanho_arquivo(in); i++)
    {

        fseek(in, tamanho() * i, SEEK_SET);
        f = le(in);
        if (f != NULL && f->cod == codigo)
        {

            end = clock();
            time_spent += (double)(end - begin)/ CLOCKS_PER_SEC;

            fprintf(arq_busca, "\n|| Nome: %s\n", f->nome);
            fprintf(arq_busca, "|| Codigo: %d\n", f->cod);
            fprintf(arq_busca, "|| Salario: %f\n", f->salario);
            fprintf(arq_busca, "|| Total de comparacoes: %d\n", i);
            fprintf(arq_busca, "|| Tempo gasto: %f\n", time_spent);
            printf("|| Total de comparacoes: %d", i);
            printf("\n|| Tempo gasto: %f", time_spent);

            return f;
        }
    }
    printf("Funcionario não encontrado");
    free(f);

    return NULL;
}
