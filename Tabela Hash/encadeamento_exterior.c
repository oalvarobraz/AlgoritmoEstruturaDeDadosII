//
// Created by Alvaro on 22/03/2023.
//

#include "encadeamento_exterior.h"

int hash(int codigo, int tam){
    return codigo % tam;
}

void cria_hash(char *nome_arquivo_hash, int tam){
    FILE *arq;
    CompartimentoHash *Compartimento = (CompartimentoHash*) malloc(sizeof(CompartimentoHash));
    if((arq= fopen(nome_arquivo_hash, "wb")) == NULL){
        printf("|| ERRO");
    } else{
        for(int i = 0; i < tam; i++){
            Compartimento = compartimento_hash(-1);// Criando compartimentos apontando para NULL
            salva_compartimento(Compartimento, arq);
        }
    }
    fclose(arq);
}


int busca(int codig, FILE* arq_hash, char* nome_arquivo_dados, int tam){
    // Calculando o endereço aplicando a função h(x)
    int h = hash(codig,tam);
    int posicao_hash;

    // Buscando o resitro na lista associada ao endereço hash
    posicao_hash = h * sizeof(int);
    if(posicao_hash == 0){
        rewind(arq_hash);
    }else
        fseek(arq_hash, posicao_hash, SEEK_SET);


    // Percorrendo a lista encadeada associada ao endereço
    CompartimentoHash *compartimentoHash = le_compartimento(arq_hash);
    // Caso o compartimento estiver vazio, a chave obviamente não estará la
    if (compartimentoHash->prox == -1){
        return -1;
    } else {
        // Caso o compartimento aponte para uma lista encadeada
        FILE *arq_dados = fopen("funcionarios.dat","rb");
        if(compartimentoHash->prox == 0) {
            rewind(arq_dados);
        } else {
            fseek(arq_dados, tamanho() * compartimentoHash->prox, SEEK_SET);
        }
        // Percorrendo a lista encadeada
        int pos = compartimentoHash->prox;
        while (pos != -1) {
            if (pos == 0) {
                rewind(arq_dados);
            } else {
                fseek(arq_dados, tamanho() * pos, SEEK_SET);
            }
            // Lendo o funcionario
            TFunc *f = le(arq_dados);

            // Comparando o codigo desse funcionario com o codigo buscado
            if (f->cod == codig) {
                fclose(arq_dados);
                return pos;
            }else
                pos = f->prox;
        }
        return -1;
    }

}

int insere(TFunc *func, FILE *arq_hash, char *nome_arquivo_dados, int tam) {
    // Calculando o endereço com a função hash
    int h, posicao;
    h = hash(func->cod, tam);
    posicao = tamanho_compartimento() * (h);

    if(posicao == 0) {
        rewind(arq_hash);
    } else {
        fseek(arq_hash, posicao, SEEK_SET);
    }
    // Lendo o compartimento
    CompartimentoHash *Compartimento;
    Compartimento = le_compartimento(arq_hash);

    if(Compartimento->prox != -1) {
        // Caso I: Compartimento ja tem registro salvo, caso de colisão
        FILE *arq_dado;

        if ((arq_dado = fopen(nome_arquivo_dados, "r+b")) == NULL) {
            printf("|| ERRO: Ocorreu um erro ao tentar abrir o arquivo");
            exit(1);
        }

        if(Compartimento->prox == 0) {
            rewind(arq_dado);
        } else {
            fseek(arq_dado, tamanho() * Compartimento->prox, SEEK_SET);
        }

        posicao = Compartimento->prox;

        while (1) {
            TFunc *f = le(arq_dado);
            if (f->prox == -1) {
                fseek(arq_dado, 0, SEEK_END);
                // Salvando funcionario no arquivo
                salva(func, arq_dado);
                // Ajustando o prox do funcionario
                f->prox = tamanho_arquivo(arq_dado) - 1;
                if (posicao == 0) {
                    rewind(arq_dado);
                } else {
                    fseek(arq_dado, tamanho() * posicao, SEEK_SET);
                }
                salva(f, arq_dado);
                free(f);
                fclose(arq_dado);
                return 1;
            } else {
                fseek(arq_dado, tamanho() * f->prox, SEEK_SET);
                posicao = f->prox;
            }
        }
    } else {
        // Caso II: Compartimento vazio (salvo direto o funcionario e ajusto o prox do compartimento)
        FILE *arq_dado;
        if ((arq_dado = fopen(nome_arquivo_dados, "a+b")) == NULL) {
            printf("|| ERRO: Ocorreu um erro ao tentar abrir o arquivo");
            exit(1);
        }
        // Salvando o funcionario
        salva(func, arq_dado);

        // Atualizando o prox do compartimento
        Compartimento->prox = tamanho_arquivo(arq_dado) - 1;
        fseek(arq_hash, tamanho_compartimento() * (h), SEEK_SET);
        salva_compartimento(Compartimento, arq_hash);
        fclose(arq_dado);
        free(Compartimento);
        return 1;
    }

}


int exclui(int cod, FILE *arq_hash, char *nome_arquivo_dados, int tam) {
    // Calculando o endereço aplicando a função h(x)
    int h = hash(cod, tam);
    int posicao_hash;

    // Ajustando o ponteiro para o endereço obtido com a hash
    posicao_hash = h * tamanho_compartimento();
    if(posicao_hash == 0) {
        rewind(arq_hash);
    } else {
        fseek(arq_hash, posicao_hash, SEEK_SET);
    }
    CompartimentoHash *compartimentoHash;
    compartimentoHash = le_compartimento(arq_hash);

    if(compartimentoHash->prox == -1) {
        return -1;
    } else {
        FILE *arq_dado;
        if ((arq_dado = fopen(nome_arquivo_dados, "r+b")) == NULL) {
            printf("|| ERRO: Ocorreu um erro ao tentar abrir o arquivo");
            exit(1);
        }

        if(compartimentoHash->prox == 0) {
            rewind(arq_dado);
        } else {
            fseek(arq_dado, tamanho() * compartimentoHash->prox, SEEK_SET);
        }

        int flag = 1;
        int pos = compartimentoHash->prox;

        while (1) {
            // Lendo o funcionario
            TFunc *f = le(arq_dado);
            // Caso o funcionario for o primeiro na lista
            if (f->cod == cod && flag == 1) {
                if(h == 0) {
                    rewind(arq_hash);
                } else {
                    fseek(arq_hash, tamanho_compartimento() * (h), SEEK_SET);
                }
                // Fazendo com que o compartimento receba o funcionario proximo
                compartimentoHash->prox = f->prox;
                salva_compartimento(compartimentoHash, arq_hash);
                free(compartimentoHash);
                free(f);
                fclose(arq_dado);
                return 1;
            }
            // Caso em que o funcionario esta no meio da lista
            if (f->cod == cod) {
                TFunc *f_aux = funcionario(f->cod, f->nome, f->salario, f->prox);

                if (pos == 0) {
                    rewind(arq_dado);
                } else {
                    fseek(arq_dado, tamanho() * pos, SEEK_SET);
                }
                // Lendo o funcionario
                f = le(arq_dado);
                // Fazendo uma ponte
                f->prox = f_aux->prox;
                if (pos == 0) {
                    rewind(arq_dado);
                } else {
                    fseek(arq_dado, tamanho() * pos, SEEK_SET);
                }
                salva(f, arq_dado);
                free(f);
                free(f_aux);
                fclose(arq_dado);
                return 1;
            } else {
                fseek(arq_dado, tamanho() * f->prox, SEEK_SET);
                // Vericar se pode passar o prox de funcionario
                TFunc *f_aux = le(arq_dado);
                if (f_aux != NULL && f_aux->cod != cod) {
                    pos = f->prox;
                }
                free(f_aux);
                fseek(arq_dado, tamanho() * f->prox, SEEK_SET);
            }
            flag = 0;
            // Caso em o compartimento esta vazio
            if (f->prox == -1 && f->cod != cod) {
                fclose(arq_dado);
                free(f);
                return -1;
            }
        }
    }
}

void imprimir_hash(FILE *arq_hash, char* nome_arquivo_dados, int tam){

    for (int i = 0; i < tam; i++){

        if (i == 0)
            rewind(arq_hash);
        else
            fseek(arq_hash, tamanho_compartimento() * i, SEEK_SET);

        CompartimentoHash *compartimentoHash;
        // Lendo o compartimento
        compartimentoHash = le_compartimento(arq_hash);
        printf("\n\n-> Compartimento %d", i+1);

        // Caso I: Compartimento vazio
        if(compartimentoHash->prox == -1) {
            printf("\n Vazio");
            free(compartimentoHash);
        } else {
            // Caso II: Compartimento cheio
            FILE *arq_dado;

            if ((arq_dado = fopen(nome_arquivo_dados, "r+b")) == NULL) {
                printf("\n|| ERRO: Ocorreu um erro ao tentar abrir o arquivo");
                exit(1);
            }

            if(compartimentoHash->prox == 0) {
                rewind(arq_dado);
            } else {
                fseek(arq_dado, tamanho() * compartimentoHash->prox, SEEK_SET);
            }

            while (1) {
                TFunc *f = le(arq_dado);
                printf("\n");
                imprime(f);
                if (f->prox == -1) {
                    break;
                } else {
                    fseek(arq_dado, tamanho() * f->prox, SEEK_SET);
                }
            }
        }
    }
}