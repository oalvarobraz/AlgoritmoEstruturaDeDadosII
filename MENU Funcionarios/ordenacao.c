#include "ordenacao.h"
#include "windows.h"


// Insertion Sort disco: Maior capacidade de armazenamento, entretanto mais lento
/*
 * arq É o ponteiro para o arquivo
 * tam É a quantidade de registros que o arquivo contém
 */
void insertion_sort_disco(FILE *arq, int tam) {

    // Variáveis para calcular o tempo
    double time_spent = 0.0;
    clock_t begin, end;
    begin = clock();

    // Variável para calcular a quantidade de comparações


    int i, comp;
    comp = 0;
    // faz o insertion sort
    for (int j = 2; j <= tam; j++) {
        // posiciona o arquivo no registro j
        fseek(arq, (j-1) * tamanho(), SEEK_SET);
        TFunc *fj = le(arq);
        i = j - 1;
        //posiciona o cursor no registro i
        fseek(arq, (i-1) * tamanho(), SEEK_SET);
        TFunc *fi = le(arq);
        printf("fi = %d\n", fi->cod);
        while ((i > 0) && (fi->cod > fj->cod)) {
            // posiciona o cursor no registro i+1
            fseek(arq, i * tamanho(), SEEK_SET);
            salva(fi, arq);
            i = i - 1;
            // le registro i
            fseek(arq, (i-1) * tamanho(), SEEK_SET);
            fi = le(arq);
            comp ++;
        }
        // posiciona cursor no registro i + 1
        fseek(arq, (i) * tamanho(), SEEK_SET);
        // salva registro j na posiÃ§Ã£o i
        salva(fj, arq);
    }
    end = clock();
    time_spent += (double)(end - begin)/ CLOCKS_PER_SEC;

    FILE *out;
    out = fopen("insertion_disco.txt", "w");

    fprintf(out, "\n|| Total de comparacoes: %d\n", comp);
    fprintf(out, "|| Tempo gasto: %f\n", time_spent);
    printf("|| Total de comparacoes: %d", comp);
    printf("\n|| Tempo gasto: %f", time_spent);

    //descarrega o buffer para ter certeza que dados foram gravados
    fflush(arq);
    fflush(out);
    fclose(out);

}

// Insertion Sort memória: Menor capacidade de armazenamento, entretanto mais rápido
void insertion_sort(FILE *arq, int tam) {

    // Variáveis para calcular o tempo
    double time_spent = 0.0;
    clock_t begin, end;
    begin = clock();

    // Variável para calcular a quantidade de comparações
    int comp = 0;

    int i = 0;
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

    end = clock();
    time_spent += (double)(end - begin)/ CLOCKS_PER_SEC;

    FILE *out;
    out = fopen("insertion_memoria.txt", "w");

    fprintf(out, "\n|| Total de comparacoes: %d\n", comp);
    fprintf(out, "|| Tempo gasto: %f\n", time_spent);
    printf("|| Total de comparacoes: %d", comp);
    printf("\n|| Tempo gasto: %f", time_spent);

    //descarrega o buffer para ter certeza que dados foram gravados
    fflush(arq);
    fflush(out);
    fclose(out);
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

void selecao_com_substituicao(FILE *arq, Lista *nome_arquivos_saida, int M) {

    // Variáveis para calcular o tempo
    double time_spent = 0.0;
    clock_t begin, end;
    begin = clock();

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

    end = clock();
    time_spent += (double)(end - begin)/ CLOCKS_PER_SEC;

    FILE *out;
    out = fopen("selecao_com_substituicao.txt", "w");

    fprintf(out, "\n|| Total de comparacoes: %d\n", comp);
    fprintf(out, "|| Tempo gasto: %f\n", time_spent);

    fflush(out);
    fflush(arq);
    fclose(out);

    // Liberando variáveis utilizadas
    free(func);
}

int intercalacao_basico(char *nome_arquivo_saida, int num_p, Lista *nome_particoes) {

    double time_spent = 0.0;
    clock_t begin, end;
    begin = clock();
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

        end = clock();
        time_spent += (double)(end - begin)/ CLOCKS_PER_SEC;

        FILE *arq;
        arq = fopen("intercalacao_basico.txt", "w");

        fprintf(arq, "\n|| Total de comparacoes: %d\n", comp);
        fprintf(arq, "|| Tempo gasto: %f\n", time_spent);

        //fecha arquivo de saida
        fflush(arq);
        fclose(out);
        fclose(arq);

        return comp;
    }
}

void intercalacao_otima(char *nome_arquivo_saida, int num_p, Lista *nome_particoes, int f) {
    int numP = num_p;
    int i = num_p;
    char nome_arquivo[50];

    int comp = 0;

    // Variáveis para calcular o tempo
    clock_t begin, end;
    double time_spent = 0.0;
    begin = clock();

    // Lista auxiliar
    Lista *pLista = NULL;
    // retirar o primeiro nome da pLista
    while (i > f - 1) {
        Lista *aux = cria_nomes(cria_str("anything"), NULL);
        for (int j = 0; j < f - 1; j++) {
            pLista = nome_particoes->prox;
            adiciona_nomes(aux, nome_particoes->nome);
            free(nome_particoes->nome);
            free(nome_particoes);
            nome_particoes = pLista;
            i--;
        }
        pLista = aux->prox;
        free(aux->nome);
        free(aux);
        aux = pLista;
        numP++;

        sprintf(nome_arquivo, "p%d.dat", numP);
        comp += intercalacao_basico(nome_arquivo, f-1, aux);
        adiciona_nomes(nome_particoes, nome_arquivo);
        libera_nomes(aux);
    }
    // Ordenação final
    comp += intercalacao_basico(nome_arquivo_saida, i, nome_particoes);

    // Tempo de execucao em arquivo
    end = clock();
    time_spent += (double) (end - begin) / CLOCKS_PER_SEC;

    FILE *out = fopen("intercalacao_otima.txt", "w");

    fprintf(out, "\n|| Total de comparacoes: %d\n", comp);
    fprintf(out, "|| Tempo gasto: %f\n", time_spent);
    printf("|| Total de comparacoes: %d", comp);
    printf("\n|| Tempo gasto: %f", time_spent);

    // Apagar as particoes
    fclose(out);
}
