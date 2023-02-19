//
// Created by Alvaro on 18/02/2023.
//
#include "ordenacao.h"
#include "windows.h"


// Imprime ordenacao
void imprimeOrdenacao(TOrden *orden){
    printf("\nComparacoes ");
    printf("%d", orden->comparacoes);
    printf("\nTempo ");
    printf("%f", orden->tempo);
    printf("\n\n********************************************************************************************");
}

// Cria ordenacao. Lembrar de usar free(ordenacao)
TOrden *orden(int comparacoes, double tempo){
    TOrden *ord = (TOrden *) malloc(sizeof(TOrden));
    if(ord) memset(ord, 0, sizeof(TOrden));
    ord->comparacoes = comparacoes;
    ord->tempo = tempo;
    return ord;
}

// Salva ordenacao no arquivo out, na posicao atual do cursor
void salva_orden(TOrden *orden, FILE *out){
    fwrite(&orden->comparacoes, sizeof(int), 1, out);
    fwrite(&orden->tempo, sizeof(double ), 1, out);
}

// Le a ordenacao do arquivo in na posicao atual do cursor
// Retorna um ponteiro para a ordenacao lida do arquivo
TOrden *le_orden(FILE *in){
    TOrden *ord = (TOrden *) malloc(sizeof(TOrden));
    fread(&ord->comparacoes, sizeof(int), 1, in);
    fread(&ord->tempo, sizeof(double ), 1, in);
    return ord;
}

// Retorna tamanho da ordencao em bytes
int tamanho_ordenacao(){
    return sizeof(int)  //comparacoes
           + sizeof(double ); //tempo
}

// Retorna o tamanho do arquivo
int tamanho_arquivo_ordenacao(FILE *arq){
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq)/tamanho_ordenacao());
    return tam;
}

// Imprime arquivo ordencao
void imprime_arquivo_ordenacao(FILE *arq){
    //le o arquivo e coloca no vetor
    rewind(arq); //posiciona cursor no inicio do arquivo
    TOrden *ord = le_orden(arq);
    while (!feof(arq)) {
        imprimeOrdenacao(ord);
        ord = le_orden(arq);
    }
}


// Insertion Sort disco: Maior capacidade de armazenamento, entretanto mais lento
/*
 * arq É o ponteiro para o arquivo
 * tam É a quantidade de registros que o arquivo contém
 */
void insertion_sort_disco(FILE *arq, int tam) {

    // Variáveis para calcular o tempo
    double time_spent = 0.0;
    clock_t begin = clock();

    // Variável para calcular a quantidade de comparações


    int i, comp;
    comp = 0;
    // faz o insertion sort
    for (int j = 2; j <= tam; j++) {
        // posiciona o arquivo no registro j
        fseek(arq, (j-1) * tamanho(), SEEK_SET);
        TFunc *fj = le(arq);
        printf("\n********* Funcionario atual: %d\n", fj->cod);
        i = j - 1;
        //posiciona o cursor no registro i
        fseek(arq, (i-1) * tamanho(), SEEK_SET);
        TFunc *fi = le(arq);
        printf("fi = %d\n", fi->cod);
        while ((i > 0) && (fi->cod > fj->cod)) {
            // posiciona o cursor no registro i+1
            fseek(arq, i * tamanho(), SEEK_SET);
            printf("Salvando funcionario %d na posicao %d\n", fi->cod, i+1);
            salva(fi, arq);
            i = i - 1;
            // le registro i
            fseek(arq, (i-1) * tamanho(), SEEK_SET);
            fi = le(arq);
            printf("fi = %d; i = %d\n", fi->cod, i);
            comp ++;
        }
        // posiciona cursor no registro i + 1
        fseek(arq, (i) * tamanho(), SEEK_SET);
        printf("*** Salvando funcionario %d na posicao %d\n", fj->cod, i+1);
        // salva registro j na posiÃ§Ã£o i
        salva(fj, arq);
    }
    Sleep(1);

    clock_t end = clock();
    time_spent += (double)(end - begin)/ CLOCKS_PER_SEC;

    FILE *out;
    out = fopen("insertion_disco.dat", "wb+");

    TOrden *od;
    od = orden(comp, time_spent);
    salva_orden(od, out);

    imprimeOrdenacao(od);

    //descarrega o buffer para ter certeza que dados foram gravados
    fflush(arq);
    fflush(out);

    free(od);

}

// Insertion Sort memória: Menor capacidade de armazenamento, entretanto mais rápido
void insertion_sort(FILE *arq, int tam) {

    // Variáveis para calcular o tempo
    double time_spent = 0.0;
    int i = 0;
    int comp = 0;
    clock_t begin = clock();

    // Variável para calcular a quantidade de comparações


    TFunc *v[tam - 1];
    //le o arquivo e coloca no vetor
    rewind(arq); //posiciona cursor no inicio do arquivo
    TFunc *f = le(arq);

    while (!feof(arq)) {
        v[i] = f;
        f = le(arq);
        i++;
    }
    //faz o insertion sort
    for (int j = 1; j < tam; j++) {
        TFunc *fun = v[j];
        i = j - 1;
        while ((i >= 0) && (v[i]->cod > fun->cod)) {
            v[i + 1] = v[i];
            i = i - 1;
            comp += 1;
        }
        v[i+1] = fun;
    }
    //grava vetor no arquivo, por cima do conteudo anterior
    rewind(arq);
    for (int k = 0; k < tam; k++) {
        salva(v[k], arq);
    }
    Sleep(1);

    clock_t end = clock();
    time_spent += (double)(end - begin)/ CLOCKS_PER_SEC;

    FILE *out;
    out = fopen("insertion_memoria.dat", "wb+");

    TOrden *od;
    od = orden(comp, time_spent);
    salva_orden(od, out);

    imprimeOrdenacao(od);

    //descarrega o buffer para ter certeza que dados foram gravados
    fflush(arq);
    fflush(out);
    free(od);
}

// Sobrescreve funcionario
void sobrescreve_funcionario(FILE *in, int posicao, TFunc *f1) {
    //pula primeiros n registros para posicionar no início do quarto registro
    fseek(in, tamanho() * posicao, SEEK_SET);
    salva(f1, in);
}

// Fisher-yates algoritmo de aleatorizacao
void shuffle(FILE *in) {
    // Valores diferentes todas as vezes
    srand (time(NULL));

    TFunc *f1 = NULL;
    TFunc *f2 = NULL;

    for (int i = tamanho_arquivo(in)-1; i > 0; i--) {
        int j = rand() % (i+1);

        fseek(in, tamanho() * i, SEEK_SET);
        f1 = le(in);
        fseek(in, tamanho() * j, SEEK_SET);
        f2 = le(in);

        sobrescreve_funcionario(in, i, f2);
        sobrescreve_funcionario(in, j, f1);

        free(f1);
        free(f2);
    }

}

void classificacao_interna(char *nome_arquivo_entrada, Lista *nome_arquivos_saida, int M) {

    // Variáveis para calcular o tempo
    double time_spent = 0.0;
    clock_t begin = clock();

    // Variável para calcular a quantidade de comparações
    int comp = 0;

    int fim = 0; //variável de controle para saber se arquivo de entrada terminou
    FILE *arq; //declara ponteiro para arquivo

    //abre arquivo para leitura
    if ((arq = fopen(nome_arquivo_entrada, "rb")) == NULL) {
        printf("Erro ao abrir arquivo de entrada\n");
    } else {

        //le primeiro cliente
        TFunc *func = le(arq);

        while (!(fim)) {
            //le o arquivo e coloca no vetor
            TFunc *v[M];

            int i = 0;
            while (!feof(arq) && i < M) {
                v[i] = func;
                func = le(arq);
                i++;
            }

            //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
            if (i != M) {
                M = i;
            }

            //faz ordenacao
            for (int j = 1; j < M; j++) {
                TFunc *c = v[j];
                i = j - 1;
                while ((i >= 0) && (v[i]->cod > c->cod)) {
                    v[i + 1] = v[i];
                    i = i - 1;
                    comp++;
                }
                v[i + 1] = c;
            }

            //cria arquivo de particao e faz gravacao
            char *nome_particao = nome_arquivos_saida->nome;
            nome_arquivos_saida = nome_arquivos_saida->prox;

            printf("\n%s\n", nome_particao);

            FILE *p;
            if ((p = fopen(nome_particao, "wb")) == NULL) {
                printf("Erro criar arquivo de saida\n");
            } else {
                for (int m = 0; m < M; m++) {
                    salva(v[m], p);
                    imprime(v[m]);
                }
                fclose(p);
            }
            if (feof(arq)) {

                fim = 1;
            }
        }

        clock_t end = clock();
        time_spent += (double)(end - begin)/ CLOCKS_PER_SEC;

        FILE *out;
        out = fopen("classificao_interna.dat", "wb");

        TOrden *od;
        od = orden(comp, time_spent);
        salva_orden(od, out);

        imprimeOrdenacao(od);

        fflush(out);
        fflush(arq);

        free(od);
    }
}

void selecao_com_substituicao(FILE *arq, Lista *nome_arquivos_saida, int M) {

    // Variáveis para calcular o tempo
    double time_spent = 0.0;
    clock_t begin = clock();

    // Variável para calcular a quantidade de comparações
    int comp = 0;

    int fim = 0; //variável de controle para saber se arquivo de entrada terminou
    int desc[M];
    TFunc *v[M];

    // Ler M registros do arquivo para a memória

    TFunc *func;
    rewind(arq);
    func = le(arq);


    int i = 0;
    while (!feof(arq) && i < M) {
        v[i] = func;
        func = le(arq);
        i++;
    }

    //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
    if (i != M) {
        M = i;
    }

    // ajusta a posição do cursor visto que na linha 237 eu leio de novo um funcionario do arquivo e isso faz eu pular um dos funcionarios, dessa maneira esse erro é corrigido
    fseek(arq, tamanho() * i, SEEK_SET);


    while (!(fim)) {

        //cria arquivo de particao
        // A quantidade de nomes na lista tem que ser grande
        char *nomeParticao = nome_arquivos_saida->nome;
        nome_arquivos_saida = nome_arquivos_saida->prox;

        FILE *p;
        if ((p = fopen(nomeParticao, "wb")) == NULL) {
            printf("Erro criar arquivo de saida\n");
            exit(1);
        }


        // Inicializando o vetor, que diz quem esta congelado, com todas as variáveis descongeladas
        for (int j = 0; j < M; ++j) {
            if (v[j] != NULL) {
                desc[j] = 0;
            } else {
                desc[j] = 1;
            }
        }

        int cont = 1;

        while (cont) {

            // 2. Selecionar, no array em memória, o registro r com menor chave
            int posicao_menor = 0;
            TFunc *menor;

            for (int k = 0; k < M; k++) {
                if (desc[k] == 0 && v[k] != NULL) {
                    menor = v[k];
                    //cod = menor->cod;
                    posicao_menor = k;
                    break;
                }
            }

            for (int j = 0; j < M; j++) {
                if (v[j] != NULL) {
                    if (menor->cod > v[j]->cod && desc[j] == 0) {
                        menor = v[j];
                        posicao_menor = j;
                    }
                    comp++;
                }
            }

            // 3. Gravar o registro r na partição de saída
            salva(menor, p);

            // 4. Substituir, no array em memória, o registro r pelo próximo registro do arquivo de entrada
            func = le(arq);


            if (func != NULL) {
                v[posicao_menor] = func;

                // 5. Caso a chave deste último seja menor do que a chave recém gravada, considerá-lo congelado e ignorá-lo no restante do processamento
                if (v[posicao_menor]->cod < menor->cod) {
                    desc[posicao_menor] = 1;
                }
            } else {
                v[posicao_menor] = NULL;
                desc[posicao_menor] = 1;
            }

            // 6. Caso existam em memória registros não congelados, voltar ao passo 2
            for (int j = 0; j < M; ++j) {
                if (desc[j] == 0) {
                    cont = 1;
                    break;
                } else {
                    cont = 0;
                }
            }

            if (cont == 0) {
                // 7. fechando a partição de saída, descongelando os registros congelados, abrindo nova partição de saída
                fflush(p);
                for (int j = 0; j < M; j++) {
                    if (desc[j] == 0) {
                        free(v[j]);
                        v[j] = NULL;
                    }
                }
            }

        }
        if (feof(arq)) {
            for(int u = 0; u < M; u++){
                if (v[u] != NULL){
                    fim = 0;
                    break;
                }else{
                    fim = 1;
                }
            }
        }

    }

    clock_t end = clock();
    time_spent += (double)(end - begin)/ CLOCKS_PER_SEC;

    FILE *out;
    out = fopen("selecao_com_substituicao.dat", "wb");

    TOrden *od;
    od = orden(comp, time_spent);
    salva_orden(od, out);

    imprimeOrdenacao(od);

    fflush(out);
    fflush(arq);


    // Liberando variáveis utilizadas
    free(func);
    free(od);
}

void intercalacao_basico(char *nome_arquivo_saida, int num_p, Lista *nome_particoes) {

    double time_spent = 0.0;
    clock_t begin = clock();
    int comp = 0;

    int fim = 0; //variavel que controla fim do procedimento
    FILE *out; //declara ponteiro para arquivo

    //abre arquivo de saida para escrita
    if ((out = fopen(nome_arquivo_saida, "wb")) == NULL) {
        printf("Erro ao abrir arquivo de sa?da\n");
    } else {
        //cria vetor de particoes
        TVet v[num_p];

        //abre arquivos das particoes, colocando variavel de arquivo no campo file do vetor
        //e primeiro cliente do arquivo no campo cli do vetor
        for (int i = 0; i < num_p; i++) {
            v[i].file = fopen(nome_particoes->nome, "rb");
            if (v[i].file != NULL) {
                TFunc *f = le(v[i].file);
                if (f == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posição do vetor
                    v[i].tfunc = funcionario(INT_MAX, "", 0.0);
                } else {
                    //conseguiu ler funcionario, coloca na posicao atual do vetor
                    v[i].tfunc = f;
                }
            } else {
                fim = 1;
            }
            nome_particoes = nome_particoes->prox;
        }

        while (!(fim)) { //conseguiu abrir todos os arquivos
            int menor = INT_MAX;
            int pos_menor;
            //encontra o cliente com menor chave no vetor
            for (int i = 0; i < num_p; i++) {
                if (v[i].tfunc->cod < menor) {
                    menor = v[i].tfunc->cod;
                    pos_menor = i;
                }
                comp++;
            }
            if (menor == INT_MAX) {
                fim = 1; //terminou processamento
            } else {
                //salva cliente no arquivo de saida
                salva(v[pos_menor].tfunc, out);
                //atualiza posicao pos_menor do vetor com proximo cliente do arquivo
                TFunc *f = le(v[pos_menor].file);
                if (f == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posicao do vetor
                    v[pos_menor].tfunc = funcionario(INT_MAX, "", 0.0);
                } else {
                    v[pos_menor].tfunc = f;
                }
            }
        }

        //fecha arquivos das particoes de entrada
        for (int i = 0; i < num_p; i++) {
            fclose(v[i].file);
        }

        Sleep(1);

        clock_t end = clock();
        time_spent += (double)(end - begin)/ CLOCKS_PER_SEC;

        FILE *arq;
        arq = fopen("intercalacao_basico.dat", "wb");

        TOrden *od;
        od = orden(comp, time_spent);
        salva_orden(od, arq);

        imprimeOrdenacao(od);

        //fecha arquivo de saida
        fclose(out);
        fclose(arq);

        free(od);
    }
}

void intercalacao_otima(char *nome_arquivo_saida, int num_p, Lista *nome_particoes, int f) {
    // Variáveis para calcular o tempo
    clock_t start, end;
    double time_spent = 0.0;
    start = clock();

    // Variavel para calcular a quantidade de comparações
    int comp = 0;

    // Variáveis auxiliares
    int numP = num_p;
    int p = num_p;
    char nome_arquivo[50];
    Lista *pLista = NULL;

    while (p > f - 1) {
        Lista *aux = cria_nomes(cria_str("anything"), NULL);
        for (int k = 0; k < f - 1; k++) {
            pLista = nome_particoes->prox;
            adiciona_nomes(aux, nome_particoes->nome);
            nome_particoes = pLista;
            free(nome_particoes->nome);
            free(nome_particoes);
            p--;
        }
        pLista = aux->prox;
        free(aux->nome);
        free(aux);
        aux = pLista;
        numP++;
        sprintf(nome_arquivo, "p%d.dat", numP);
        intercalacao_basico(nome_arquivo, f-1, aux);
        adiciona_nomes(nome_particoes, nome_arquivo);
        libera_nomes(aux);
    }
    // Ordenação final
    intercalacao_basico(nome_arquivo_saida, p, nome_particoes);

    // Tempo de execucao em arquivo
    end = clock();
    time_spent += (double) (end - start) / CLOCKS_PER_SEC;

    FILE *arq;
    arq = fopen("intercalacao_otima.dat", "wb");

    TOrden *od;
    od = orden(comp, time_spent);
    salva_orden(od, arq);

    imprimeOrdenacao(od);

    fflush(arq);

    free(od);
    // Apagar as particoes


}
