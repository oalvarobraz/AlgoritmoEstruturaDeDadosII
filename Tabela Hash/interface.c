//
// Created by Alvaro on 22/03/2023.
//

#include "interface.h"
#include <stdlib.h>

void imprime_arquivo_txt(FILE *arq){
    int i;
    char Linha[100];
    char *result;
    // Abre um arquivo TEXTO para LEITURA
    if (arq == NULL)  // Se houve erro na abertura
    {
        printf("|| Erro ao abrir o arquivo\n");
        return;
    }
    i = 1;
    while (!feof(arq))
    {
        // Lê uma linha (inclusive com o '\n')
        result = fgets(Linha, 100, arq);  // o 'fgets' lê até 99 caracteres ou até o '\n'
        if (result)  // Se foi possível ler
            printf("%s",Linha);
        i++;
    }
}

void MSG_BUSCA(){
    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>> (2) OPCOES BUSCA <<<<<<<<<<<<<<<<<<<<<<<<");
    printf("\n\n|| (1) BUSCAR FUNCIONARIO");
    printf("  \n|| (2) BUSCAR FUNCIONARIO ALEATORIO");
    printf("  \n|| (3) RESULTADO DA BUSCA");
    printf("  \n|| (0) VOLTAR PARA O MENU PRINCIPAL");
}

void MSG_REMOCAO(){
    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>> (3) OPCOES REMOCAO <<<<<<<<<<<<<<<<<<<<<<<<");
    printf("\n\n|| (1) REMOVER FUNCIONARIO");
    printf("  \n|| (2) REMOVER FUNCIONARIO ALEATORIO");
    printf("  \n|| (3) RESULTADO DA REMOCAO");
    printf("  \n|| (0) VOLTAR PARA O MENU PRINCIPAL");
}

void salva_busca(double tempo, TFunc *func, FILE *arq, int search) {

    if (search == -1) {
        fprintf(arq, "|| Codigo: %d\n", func->cod);
        fprintf(arq, "|| Tempo gasto: %f\n", tempo);
        fprintf(arq, "|| Foi encontrado: Nao\n");
    } else {
        fprintf(arq, "|| Nome: %s\n", func->nome);
        fprintf(arq, "|| Codigo: %d\n", func->cod);
        fprintf(arq, "|| Salario: R$ %.2f\n", func->salario);
        fprintf(arq, "|| Tempo gasto: %f\n", tempo);
        fprintf(arq, "|| Foi encontrado: Sim\n");
    }
}

void salva_remocao(double tempo, int codigo, FILE* arq, int search){
    if (search == -1) {
        fprintf(arq, "|| Codigo: %d\n", codigo);
        fprintf(arq, "|| Tempo gasto: %f\n", tempo);
        fprintf(arq, "|| Foi removido: Nao\n");
    }else {
        fprintf(arq, "|| Codigo: %d\n", codigo);
        fprintf(arq, "|| Tempo gasto: %f\n", tempo);
        fprintf(arq, "|| Foi removido: Sim\n");
    }
}

void salva_insercao(double tempo, FILE *arq) {
    fprintf(arq, "|| Tempo gasto: %f\n", tempo);
    fprintf(arq, "|| Arquivo: insercao.txt\n");
}

void opcao_busca(FILE* arq_hash, char* nome_arquivo_dados, int tam){

    int op_busca;
    FILE *arq_busca, *aux;
    int codigo;
    clock_t begin, end;
    double time_spent = 0.0;
    TFunc *faux;
    int pos;

    do{
        MSG_BUSCA();
        printf("\n\nDigite uma opcao: ");
        fflush(stdin);
        scanf("%d", &op_busca);
        switch (op_busca) {
            case 0:
                break;
            case 1:
                system("cls");
                printf("\n|| BUSCANDO FUNCIONARIO ||\n");
                printf("\n|| Informe o codigo do funcionario: ");
                scanf("%d", &codigo);
                begin = clock();
                time_spent = 0.0;
                pos = busca(codigo, arq_hash, nome_arquivo_dados, tam);
                end = clock();
                time_spent += (double )(end - begin)/CLOCKS_PER_SEC;
                arq_busca = fopen("busca.txt", "w");

                if(pos != -1){
                    aux = fopen(nome_arquivo_dados, "rb");
                    fseek(aux, tamanho() * pos, SEEK_SET);
                    faux = le(aux);
                    imprime(faux);
                    salva_busca(time_spent, faux, arq_busca, pos);
                    fclose(aux);
                } else{
                    printf("\n|| Funcionario nao se encontra na tabela\n");
                    faux = funcionario(codigo,"",0,-1);
                    salva_busca(time_spent,faux,arq_busca,pos);
                }
                free(faux);
                fclose(arq_busca);
                printf("\n");
                system("PAUSE");
                system("cls");
                op_busca = 0;
                break;
            case 2:
                system("cls");
                printf("\n|| BUSCANDO FUNCIONARIO ALEATORIO ||\n");
                codigo = rand()%300;
                printf("\n|| Codigo do funcionario: %d\n", codigo);

                begin = clock();
                time_spent = 0.0;
                pos = busca(codigo, arq_hash, nome_arquivo_dados, tam);
                end = clock();
                time_spent += (double )(end - begin)/CLOCKS_PER_SEC;
                if(pos != -1){
                    printf("\n|| Funcionario encotrado");
                } else
                    printf("\n|| Funcionario nao encontrado");
                aux = fopen(nome_arquivo_dados, "rb");
                fseek(aux, tamanho() * pos, SEEK_SET);
                faux = le(aux);
                arq_busca = fopen("busca.txt", "w");
                salva_busca(time_spent, faux, arq_busca, pos);
                free(faux);

                fclose(aux);
                fclose(arq_busca);
                printf("\n");
                system("PAUSE");
                system("cls");
                op_busca = 0;
                break;
            case 3:
                system("cls");
                printf("\n|| LENDO O RESULTADO DA BUSCA ||\n");
                arq_busca = fopen("busca.txt", "r");
                imprime_arquivo_txt(arq_busca);
                printf("\n");
                fclose(arq_busca);
                system("PAUSE");
                system("cls");
                op_busca = 0;
                break;
            default:
                system("cls");
                printf("\t|| Digite uma opcao valida!!!\n");
                system("PAUSE");
                system("cls");
        } //fim do bloco switch
    } while (op_busca != 0);

}

void opcao_remocao(FILE* arq_hash, char* nome_arquivo_dados, int tam){

    int op_remocao;
    FILE *remov;
    int codigo;
    int excluir;
    clock_t begin, end;
    double time_spent = 0.0;

    do{
        MSG_REMOCAO();
        printf("\n\nDigite uma opcao: ");
        fflush(stdin);
        scanf("%d", &op_remocao);
        switch (op_remocao) {
            case 0:
                break;
            case 1:
                system("cls");
                printf("\n|| REMOVENDO FUNCIONARIO ||\n");
                printf("\n|| Informe o codigo do funcionario que voce deseja remover: ");
                scanf("%d", &codigo);
                remov = fopen("remover.txt", "w");
                begin = clock();
                time_spent = 0.0;
                excluir = exclui(codigo, arq_hash, nome_arquivo_dados, tam);
                end = clock();
                time_spent += (double )(end - begin)/CLOCKS_PER_SEC;
                salva_remocao(time_spent, codigo, remov, excluir);
                if(excluir != -1){
                    printf("\n|| Funcionario excluido com sucesso");
                }else{
                    printf("\n|| Funcionario nao se encontra na tabela hash");
                }

                fclose(remov);
                printf("\n");
                system("PAUSE");
                system("cls");
                op_remocao = 0;
                break;
            case 2:
                system("cls");
                printf("\n|| REMOVENDO FUNCIONARIO ALEATORIO ||\n");
                codigo = rand()%300;
                printf("\n|| Codigo do funcionario: %d\n", codigo);


                remov = fopen("remover.txt", "w");
                begin = clock();
                time_spent = 0.0;

                excluir = exclui(codigo, arq_hash, nome_arquivo_dados, tam);

                end = clock();
                time_spent += (double )(end - begin)/CLOCKS_PER_SEC;

                salva_remocao(time_spent, codigo, remov, excluir);
                if(excluir != -1){
                    printf("\n|| Funcionario excluido com sucesso");
                }else{
                    printf("\n|| Funcionario nao se encontra na tabela hash");
                }
                fclose(remov);
                printf("\n");
                system("PAUSE");
                system("cls");
                op_remocao = 0;
                break;
            case 3:
                system("cls");
                printf("\n|| LENDO O RESULTADO DA REMOCAO ||\n");
                remov = fopen("remover.txt", "r");
                imprime_arquivo_txt(remov);
                printf("\n");
                fclose(remov);
                system("PAUSE");
                system("cls");
                op_remocao = 0;
                break;
            default:
                system("cls");
                printf("\t|| Digite uma opcao valida!!!\n");
                system("PAUSE");
                system("cls");
        } //fim do bloco switch
    } while (op_remocao != 0);

}

void MSG_MENU() {
    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>> OPCOES DE MENU <<<<<<<<<<<<<<<<<<<<<<<<");
    printf(" \n|| (1) IMPRIMIR ARQUIVO");
    printf(" \n|| (2) OPCOES BUSCA");
    printf(" \n|| (3) OPCOES REMOVER");
    printf(" \n|| (4) TEMPO INSERCAO");
    printf(" \n|| (0) SAIR");
}

void MENU(char* nome_arquivo_dados, int tam) {

    FILE *arq_hash = fopen("hash.dat", "wb+");
    FILE *insercao = fopen("insercao.txt", "w");
    cria_hash("hash.dat",tam);
    clock_t begin, end;
    double time_spent = 0.0;
    TFunc *f;
    begin = clock();
    for (int i = 0; i < 1040; ++i) {
        f = funcionario((i+1),"Fulano", 1300, -1);
        insere(f,arq_hash, "funcionarios.dat", tam);
    }
    end = clock();
    time_spent += (double )(end - begin)/CLOCKS_PER_SEC;
    salva_insercao(time_spent,insercao);
    free(f);
    fclose(insercao);

    int opcao;
    do {
        MSG_MENU();
        printf("\n\n|| Digite uma opcao: ");
        fflush(stdin);
        scanf("%d", &opcao);
        switch (opcao) {
            case 0:
                system("cls");
                printf("\n<<<<<<<<< SAINDO DO PROGRAMA >>>>>>>>>>\n");
                fclose(arq_hash);
                system("PAUSE");
                break;
            case 1:
                system("cls");
                printf("\n|| IMPRIMINDO FUNCIONARIOS DENTRO DO ARQUIVO ||\n");

                imprimir_hash(arq_hash, "funcionarios.dat", tam);

                printf("\n");
                system("PAUSE");
                system("cls");
                break;
            case 2:
                system("cls");

                opcao_busca(arq_hash, nome_arquivo_dados, tam);

                system("cls");
                break;
            case 3:
                system("cls");

                opcao_remocao(arq_hash, nome_arquivo_dados, tam);

                system("cls");
                break;
            case 4:
                system("cls");
                FILE *file = fopen("insercao.txt", "r");
                imprime_arquivo_txt(file);
                system("PAUSE");
                fclose(file);
                system("cls");
                break;
            default:
                system("cls");
                printf("|| Digite uma opcao valida!!!\n");
                system("PAUSE");
                system("cls");
        } // fim do bloco switch
    } while (opcao != 0);
}