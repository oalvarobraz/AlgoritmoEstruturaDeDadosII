#include "interface.h"
#include "ordenacao.h"
#include "lista.h"
#include "busca.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void MSG_MENU(){
    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>> OPCOES DE MENU <<<<<<<<<<<<<<<<<<<<<<<<");
    printf("\n\n|| (1) INSERIR 1 FUNCIONARIO");
    printf(" \n|| (2) INSERIR 5000 FUNCIONARIOS");
    printf(" \n|| (3) IMPRIMIR ARQUIVO");
    printf(" \n|| (4) PESQUISAR FUNCIONARIO (BUSCA BINARIA)");
    printf(" \n|| (5) PESQUISAR FUNCIONARIO (BUSCA SEQUENCIAL)");
    printf(" \n|| (6) REALIZAR BUSCAS POR 50 FUNCIONARIOS ALEATORIOS (BUSCA BINARIA)");
    printf(" \n|| (7) REALIZAR BUSCAS POR 50 FUNCIONARIOS ALEATORIOS (BUSCA SEQUENCIAL)");
    printf(" \n|| (8) ORDENAR POR INSERION SORT DISCO");
    printf(" \n|| (9) ORDENAR POR INSERION SORT MEMORIA");
    printf(" \n|| (10) EMBARALHAR ARQUIVO");
    printf(" \n|| (11) SELECAO COM SUBSTITUICAO");
    printf(" \n|| (12) INTERCALACAO OTIMA (necessita antes ter realizado a selecao com substituicao)");
    printf(" \n|| (13) SELECAO COM SUBSTITUICAO + INTERCALACAO OTIMA");
    printf(" \n|| (14) IMPRIMINDO RESULTADOS DA BUSCA");
    printf(" \n|| (15) IMPRIMINDO RESULTADOS DA ORDENACAO");
    printf(" \n|| (0) SAIR");
}

void retira_barraN(char* str){
    int i = 0;
    while(*(str+i) != '\0'){
        if(*(str+i) == '\n' ){
            *(str+i) = '\0';
        }
        i++;
    }
}

void lendo_informacoes_funcionario(FILE *arq){

    TFunc *func;

    // Coletando as informações do funcionario
    func = funcionario(0, "Fulano", 0);
    printf("|| Informe o codigo do funcionario: ");
    scanf("%d", &func->cod);
    printf("|| Informe o nome: ");
    fflush(stdin);
    fgets(func->nome, 50, stdin);
    retira_barraN(func->nome);
    printf("|| Informe o salario: ");
    scanf("%lf", &func->salario);

    // Salvando no arquivo
    salva(func, arq);
    free(func);
}

void MSG_BUSCA(){
    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>> (15) IMPRIMINDO RESULTADOS DA BUSCA <<<<<<<<<<<<<<<<<<<<<<<<");
    printf("\n\n|| (1) RESULTADO DA BUSCA BINARIA");
    printf(" \n|| (2) RESULTADO DA BUSCA SEQUENCIAL");
    printf(" \n|| (0) VOLTAR PARA O MENU PRINCIPAL");
}

void MSG_ORDENA(){
    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>> (16) IMPRIMINDO RESULTADOS DA ORDENACAO <<<<<<<<<<<<<<<<<<<<<<<<");
    printf("\n\n|| (1) RESULTADO DA INSERTION SORT DISCO");
    printf(" \n|| (2) RESULTADO DA INSERTION SORT MEMORIA");
    printf(" \n|| (3) RESULTADO DA INTERCALACAO OTIMA");
    printf(" \n|| (0) VOLTAR PARA O MENU PRINCIPAL");
}

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

void opcao_busca(){

    int op_busca;
    FILE *busca;

    do{
        MSG_BUSCA();
        printf("\n\nDigite uma opcao: ");
        fflush(stdin);
        scanf("%d", &op_busca);
        switch (op_busca) {
            case 0:
                system("cls");
                printf("\n|| VOLTANDO PARA O MENU PRINCIPAL ||\n");
                fclose(busca);
                break;
            case 1:
                system("cls");
                printf("\n|| LENDO O RESULTADO DA BUSCA BINARIA ||\n");
                busca = fopen("busca_binaria.txt", "r");
                imprime_arquivo_txt(busca);
                printf("\n");
                fclose(busca);
                system("PAUSE");
                system("cls");
                break;
            case 2:
                system("cls");
                printf("\n|| LENDO O RESULTADO DA BUSCA SEQUENCIAL ||\n");
                busca = fopen("busca_sequencial.txt", "r");
                imprime_arquivo_txt(busca);
                printf("\n");
                fclose(busca);
                system("PAUSE");
                system("cls");
                break;
            default:
                system("cls");
                printf("\t|| Digite uma opcao valida!!!\n");
                system("PAUSE");
                system("cls");
        } //fim do bloco switch
    } while (op_busca != 0);

}

void opcao_ordena(){

    int op_ordena;
    FILE *ordena;
    do{
        MSG_ORDENA();
        printf("\n\nDigite uma opcao: ");
        fflush(stdin);
        scanf("%d", &op_ordena);
        switch (op_ordena) {
            case 0:
                system("cls");
                printf("\n|| VOLTANDO PARA O MENU PRINCIPAL ||\n");
                fclose(ordena);
                break;
            case 1:
                system("cls");
                printf("\n|| LENDO O RESULTADO DA INSERTION SORT DISCO ||\n");
                ordena = fopen("insertion_disco.txt", "r");
                imprime_arquivo_txt(ordena);
                printf("\n");
                system("PAUSE");
                fclose(ordena);
                system("cls");
                break;
            case 2:
                system("cls");
                printf("\n|| LENDO O RESULTADO DA INSERTION SORT MEMORIA ||\n");
                ordena = fopen("insertion_memoria.txt", "r");
                imprime_arquivo_txt(ordena);
                printf("\n");
                system("PAUSE");
                fclose(ordena);
                system("cls");
                break;
            case 3:
                system("cls");
                printf("\n|| LENDO O RESULTADO DA INTERCALACAO OTIMA ||\n");
                ordena = fopen("intercalacao_otima.txt", "r");
                imprime_arquivo_txt(ordena);
                printf("\n");
                system("PAUSE");
                fclose(ordena);
                system("cls");
                break;
            default:
                system("cls");
                printf("\t|| Digite uma opcao valida!!!\n");
                system("PAUSE");
                system("cls");
        } //fim do bloco switch
    }while(op_ordena != 0);
}

void MENU(FILE *arq){
    TFunc *faux = NULL;
    int codigo;

    FILE *arq_busca_bin, *arq_busca_seq;

    Lista *lst = NULL;
    lst = cria_nomes(cria_str("p1.dat"), cria_nomes(cria_str("p2.dat"), cria_nomes(cria_str("p3.dat"), cria_nomes(
            cria_str("p4.dat"), cria_nomes(cria_str("p5.dat"), cria_nomes(cria_str("p6.dat"), cria_nomes(cria_str("p7.dat"), cria_nomes(cria_str("p8.dat"), NULL))))))));

    int opcao;
    do{
        MSG_MENU();
        printf("\n\nDigite uma opcao: ");
        fflush(stdin);
        scanf("%d", &opcao);
        switch(opcao){
            case 0:
                system("cls");
                printf("\n<<<<<<<<< SAINDO DO PROGRAMA >>>>>>>>>>\n");
                if(faux != NULL){
                    free(faux);
                }
                libera_nomes(lst);
                system("PAUSE");

                break;
            case 1:
                system("cls");
                printf("\n|| LENDO O FUNCIONARIO ||\n");
                lendo_informacoes_funcionario(arq);

                break;
            case 2:
                system("cls");
                printf("\n|| INSERINDO 5000 FUNCIONARIOS ALEATORIAMENTE||\n");
                inserir_5000funcionarios(arq);
                shuffle(arq);

                break;
            case 3:
                system("cls");
                printf("\n|| IMPRIMINDO FUNCIONARIOS DENTRO DO ARQUIVO ||\n");
                imprime_arquivo(arq);
                printf("\n");
                system("PAUSE");
                system("cls");

                break;
            case 4:
                system("cls");
                printf("\n|| PESQUISANDO FUNCIONARIO ||\n");
                printf("|| Qual o codigo do funcionario: ");
                arq_busca_bin = fopen("busca_binaria.txt", "w");
                scanf("%d", &codigo);
                faux = busca_binaria(codigo, arq, 0, tamanho_arquivo(arq), arq_busca_bin);

                if(faux != NULL){
                    printf("\n-> Funcionario encontrado\n");
                    imprime(faux);
                }else{
                    printf("\n-> Funcionario nao encontrado\n");
                }

                printf("\n");
                fclose(arq_busca_bin);
                system("PAUSE");
                system("cls");

                break;
            case 5:
                system("cls");
                printf("\n|| PESQUISANDO FUNCIONARIO ||\n");
                printf("|| Qual o codigo do funcionario: ");
                scanf("%d", &codigo);
                arq_busca_seq = fopen("busca_sequencial.txt", "w");
                faux = busca_sequencial(arq, codigo, arq_busca_seq);

                if(faux != NULL){
                    printf("\n-> Funcionario encontrado\n");
                    imprime(faux);
                }else{
                    printf("\n-> Funcionario nao encontrado\n");
                }

                printf("\n");
                fclose(arq_busca_seq);
                system("PAUSE");
                system("cls");

                break;
            case 6:
                system("cls");
                arq_busca_bin = fopen("busca_binaria.txt", "w");
                for (int i = 0; i< 50; i++){
                    codigo = 1 +(rand() % 5500);
                    printf("\n|| Codigo do funcionario: %d\n\n", codigo);
                    faux = busca_binaria(codigo, arq, 0, tamanho_arquivo(arq), arq_busca_bin);

                    if(faux != NULL){
                        printf("\n-> Funcionario encontrado\n");
                        imprime(faux);
                    }else{
                        printf("\n-> Funcionario nao encontrado\n");
                    }
                }

                printf("\n");
                fclose(arq_busca_bin);
                system("PAUSE");
                system("cls");

                break;
            case 7:
                system("cls");
                arq_busca_seq = fopen("busca_sequencial.txt", "a+");
                for (int i = 0; i< 50; i++){
                    codigo = 1 +(rand() % 5000);
                    printf("\n|| Codigo do funcionario: %d\n\n", codigo);
                    faux = busca_sequencial(arq, codigo, arq_busca_seq);

                    if(faux != NULL){
                        printf("\n-> Funcionario encontrado\n");
                        imprime(faux);
                    }else{
                        printf("\n-> Funcionario nao encontrado\n");
                    }
                }

                printf("\n");
                fclose(arq_busca_seq);
                system("PAUSE");
                system("cls");

                break;

            case 8:
                system("cls");
                printf("\n|| INSERION SORT DISCO ||\n");
                insertion_sort_disco(arq,  tamanho_arquivo(arq));
                printf("\n|| ARQUIVO ORDENADO ||\n");


                break;
            case 9:
                system("cls");
                printf("\n|| INSERION SORT MEMORIA ||\n");
                insertion_sort(arq,  tamanho_arquivo(arq));
                printf("\n|| ARQUIVO ORDENADO ||\n");
                break;
            case 10:
                system("cls");
                printf("\n|| EMBARALHANDO ARQUIVO ||\n");
                shuffle(arq);
                printf("|| ARQUIVO EMBARALHADO ||\n");
                break;
            case 11:
                system("cls");
                printf("\n|| SELECAO COM SUBSTITUICAO ||\n");
                selecao_com_substituicao(arq, lst, 200);
                printf("\n");
                system("PAUSE");
                system("cls");
                break;
            case 12:
                system("cls");
                printf("\n|| INTERCALACAO OTIMA: ");
                intercalacao_otima("funcionario.dat", conta_nomes(lst), lst, 5);
                printf("\n|| ARQUIVO ORDENADO ||\n");
                system("PAUSE");
                system("cls");
                break;
            case 13:
                system("cls");
                printf("\n|| SELECAO COM SUBBSTITUICAO ||\n");
                selecao_com_substituicao(arq,lst,625);
                intercalacao_otima("funcionario.dat", conta_nomes(lst), lst, 5);
                printf("\n|| ARQUIVO ORDENADO ||\n");
                system("PAUSE");
                system("cls");
                break;
            case 14:
                system("cls");
                opcao_busca();
                printf("\n");
                system("cls");
                break;
            case 15:
                system("cls");
                opcao_ordena();
                printf("\n");
                system("cls");
                break;
            default:
                system("cls");
                printf("\t|| Digite uma opcao valida!!!\n");
                system("PAUSE");
                system("cls");
        } // fim do bloco switch
    }while(opcao != 0);
}
