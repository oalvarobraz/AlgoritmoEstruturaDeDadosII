//
// Created by Alvaro on 06/03/2023.
//
#include "interface.h"

void MSG_MENU(){
    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>> OPCOES DE MENU <<<<<<<<<<<<<<<<<<<<<<<<");
    printf("\n\n|| (1) INSERIR 1 FUNCIONARIO");
    printf(" \n|| (2) INSERIR FUNCIONARIOS APARTIR DE UM ARQUIVO");
    printf(" \n|| (3) REMOVER 1 FUNCIONARIO");
    printf(" \n|| (4) INSERIR VARIOS FUNCIONARIOS");
    printf(" \n|| (5) REMOVER VARIOS FUNCIONARIOS");
    printf(" \n|| (6) IMPRIMIR ARVORE");
    printf(" \n|| (7) PESQUISAR FUNCIONARIO");
    printf(" \n|| (8) REALIZAR BUSCAS POR 50 FUNCIONARIOS ALEATORIOS");
    printf(" \n|| (0) SAIR");
}

void MSG_BUSCA(){
    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>> (6) IMPRIMINDO RESULTADOS DA BUSCA <<<<<<<<<<<<<<<<<<<<<<<<");
    printf("\n\n|| (1) RESULTADO DA BUSCA");
    printf(" \n|| (0) VOLTAR PARA O MENU PRINCIPAL");
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
                printf("\n|| LENDO O RESULTADO DA BUSCA ||\n");

                // Função para ler arquivo onde foram salvos os mdados do funcionario buscado, tempo de execução e quantidades de comparações

                printf("\n");
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

void retira_barraN(char* str){
    int i = 0;
    while(*(str+i) != '\0'){
        if(*(str+i) == '\n' ){
            *(str+i) = '\0';
        }
        i++;
    }
}

void lendo_informacoes_funcionario(int ordem, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados){

    int cod;
    char nome[50];
    double salario;

    printf("|| Informe o codigo do funcionario: ");
    scanf("%d", &cod);
    printf("|| Informe o nome: ");
    fflush(stdin);
    fgets(nome, 50, stdin);
    retira_barraN(nome);
    printf("|| Salario: ");
    scanf("%lf", &salario);

    // Salvando no arquivo
    if(insere(cod, nome, salario,nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, ordem) != -1){
        printf("Inserido com Sucesso\n");
    }else printf("Codigo ja exitente\n");
}

void MENU(char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados){
    int codigo;
    int ordem;
    printf("|| Digite a ordem da arvore: \n");
    scanf(" %d", &ordem);
    system("cls");
    int opcao;
    do{
        MSG_MENU();
        printf("\n\n|| Digite uma opcao: ");
        fflush(stdin);
        scanf("%d", &opcao);
        switch(opcao){
            case 0:
                system("cls");
                printf("\n<<<<<<<<< SAINDO DO PROGRAMA >>>>>>>>>>\n");
                system("PAUSE");

                break;
            case 1: // Adicionando 1 funcionario
                system("cls");
                printf("\n|| LENDO O FUNCIONARIO ||\n");
                lendo_informacoes_funcionario(ordem, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados);
                system("PAUSE");
                system("cls");
                break;
            case 2: // Inserindo funcionarios apartir de um arquivo
                system("cls");
                char nome_arquivo_entrada[20];

                printf("Nome do arquivo de entrada(com .txt ou .dat).\n");
                scanf(" %s", nome_arquivo_entrada);
                carrega_dados(ordem, nome_arquivo_entrada, nome_arquivo_metadados, nome_arquivo_indice,nome_arquivo_dados);
                system("PAUSE");
                system("cls");
                break;
            case 3: // Removendo 1 funcionario
                printf("\n|| REMOVENDO FUNCIONARIO ||\n");
                printf("|| Qual o codigo do funcionario: ");
                scanf("%d", &codigo);
                if(exclui(codigo, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, ordem) != -1){
                    printf("|| Funcionario de codigo %d removido com sucesso!\n", codigo);
                }else
                    printf("|| Funcionario de codigo %d nao esta na arvore!\n", codigo);
                system("PAUSE");
                system("cls");
                break;
            case 4: // Inserindo max funcionarios
                system("cls");
                printf("\n|| INSERINDO VARIOS FUNCIONARIOS ALEATORIAMENTE||\n");
                int max = 0;
                int cod_start = 0;
                printf("|| Quantos funcionarios voce deseja inserir: ");
                scanf("%d", &max);
                printf("|| Qual o valor do codigo incial: ");
                scanf("%d", &cod_start);
                int i = 0;
                while (i<max){
                    insere(cod_start, "Fulano", 1300, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, ordem);
                    i++;
                    cod_start++;
                }
                break;
            case 5:
                system("cls");
                printf("\n|| REMOVENDO VARIOS FUNCIONARIOS ALEATORIAMENTE||\n");
                max = 0;
                int cod = 0;
                printf("|| Quantos funcionarios voce deseja remover: ");
                scanf("%d", &max);
                i = 0;
                while (i<max){
                    printf("|| Informe o codigo do funcionario que voce deseja remover: ");
                    scanf("%d", &cod);
                    if(exclui(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, ordem) != -1){
                        printf("|| Funcionario de codigo %d removido com sucesso!\n", cod);
                    }else
                        printf("|| Funcionario de codigo %d nao esta na arvore!\n", cod);
                    i++;
                }

                break;
            case 6:
                system("cls");
                printf("\n|| IMPRIMINDO FUNCIONARIOS DENTRO DA ARVORE ||\n");
                if(!imprimirArvore(ordem, nome_arquivo_dados,nome_arquivo_indice, nome_arquivo_metadados)) printf("Sem Funcionarios cadastradas.\n");
                printf("\n");
                system("PAUSE");
                system("cls");

                break;
            case 7:
                system("cls");
                printf("\n|| PESQUISANDO FUNCIONARIO ||\n");
                printf("|| Qual o codigo do funcionario: ");
                scanf("%d", &codigo);
                if(!buscaFuncionario(codigo, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, ordem))
                    printf("\n-> Funcionario nao encontrado\n");

                printf("\n");
                system("PAUSE");
                system("cls");

                break;
            case 8:
                system("cls");
                srand(time(NULL));
                for (i = 0; i< 50; i++){
                    codigo = 1 +(rand() % 150);
                    printf("\n|| Codigo do funcionario: %d\n", codigo);
                    if(!buscaFuncionario(codigo, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, ordem))
                        printf("-> Funcionario nao encontrado\n");
                }

                printf("\n");
                system("PAUSE");
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