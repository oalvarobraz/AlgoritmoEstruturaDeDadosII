//
// Created by Alvaro on 18/02/2023.
//

#include "busca.h"
#include "windows.h"

void imprimeBusca(TBusca *busca){
    printf("\nFuncionario de codigo ");
    printf("%d", busca->cod);
    printf("\nNome: ");
    printf("%s", busca->nome);
    printf("\nSalario: ");
    printf("R$ %4.2f", busca->salario);
    printf("\nComparacoes ");
    printf("%d", busca->comparacoes);
    printf("\nTempo ");
    printf("%f", busca->tempo);
    printf("\n\n********************************************************************************************");
}

TBusca *busca(int cod, char *nome, double salario, int comparacoes, double tempo){
    TBusca *busca = (TBusca *) malloc(sizeof(TBusca));
    //inicializa espaço de memória com ZEROS
    if (busca) memset(busca, 0, sizeof(TBusca));
    //copia valores para os campos de busca
    busca->cod = cod;
    strcpy(busca->nome, nome);
    busca->salario = salario;
    busca->comparacoes = comparacoes;
    busca->tempo = tempo;
    return busca;
}

// Salva busca no arquivo out, na posicao atual do cursor
void salva_busca(TBusca *busca, FILE *out){
    fwrite(&busca->cod, sizeof(int), 1, out);
    //busca->nome ao invés de &busca->nome, pois string já é ponteiro
    fwrite(busca->nome, sizeof(char), sizeof(busca->nome), out);
    fwrite(&busca->salario, sizeof(double), 1, out);
    fwrite(&busca->comparacoes, sizeof(int), 1, out);
    fwrite(&busca->tempo, sizeof(double ), 1, out);
}

// Le a busca do arquivo in na posicao atual do cursor
// Retorna um ponteiro para a busca lida do arquivo
TBusca *le_busca(FILE *in) {
    TBusca *busca = (TBusca *) malloc(sizeof(TBusca));
    if(0 >= fread(&busca->cod, sizeof(int), 1, in)){
        free(busca);
        return NULL;
    }
    fread(busca->nome, sizeof(char), sizeof(busca->nome), in);
    fread(&busca->salario, sizeof(double), 1, in);
    fread(&busca->comparacoes, sizeof(int), 1, in);
    fread(&busca->tempo, sizeof(double ), 1, in);
    return busca;
}

// Retorna tamanho da busca em bytes
int tamanho_busca() {
    return sizeof(int)  //cod
           + sizeof(char) * 50 //nome
           + sizeof(double) //salario
           + sizeof(int) //comparacoes
           + sizeof(double); //tempo
}

// Imprime arquivo busca
void imprime_arquivo_busca(FILE *arq) {
    //le o arquivo e coloca no vetor
    rewind(arq); //posiciona cursor no inicio do arquivo
    TBusca *busc = le_busca(arq);
    while (!feof(arq)) {
        imprimeBusca(busc);
        busc = le_busca(arq);
    }
}


// Busca binária
TFunc *busca_binaria(int chave, FILE *in, int inicio, int fim, FILE *arqBusca) {

    // Variáveis para calcular o tempo
    double time_spent = 0.0;
    clock_t begin = clock();

    // Variável para calcular a quantidade de comparações
    int comp = 0;

    TFunc *func = NULL;
    int cod = -1;
    while (inicio <= fim && cod != chave) {

        comp++;

        int meio = trunc((inicio + fim) / 2);
        printf("Inicio: %d; Fim: %d; Meio: %d\n", inicio, fim, meio);
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

        Sleep(1);

        clock_t end = clock();
        time_spent += (double)(end - begin)/ CLOCKS_PER_SEC;


        TBusca *b;
        b = busca(func->cod,func->nome,func->salario, comp, time_spent);

        imprimeBusca(b);
        salva_busca(b,arqBusca);

        return func;
    } else {

        clock_t end = clock();

        return NULL;
    }
}

// Busca sequencial
TFunc *busca_sequencial(FILE *in, int codigo, FILE *arq_busca){

    // Variáveis para calcular o tempo
    double time_spent = 0.0;
    clock_t begin = clock();

    // Variável para calcular a quantidade de comparações
    // Nesse caso sera o próprio i do looping

    TFunc *f;

    for (int i = 0; i < tamanho_arquivo(in); i++)
    {

        fseek(in, tamanho() * i, SEEK_SET);
        f = le(in);
        if (f != NULL && f->cod == codigo)
        {

            clock_t end = clock();
            time_spent += (double)(end - begin)/ CLOCKS_PER_SEC;

            // Aqui eu devo salvar o funcionario encontrado e o tempo de execução do codigo
            TBusca *b;
            b = busca(f->cod, f->nome, f->salario, i, time_spent);

            imprimeBusca(b);

            salva_busca(b,arq_busca);

            return f;
        }
    }
    printf("Funcionario não encontrado");
    free(f);

    return NULL;
}