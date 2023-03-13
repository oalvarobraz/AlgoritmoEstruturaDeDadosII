//
// Created by Alvaro on 11/03/2023.
//

#include "interface.h"

void MSG_MENU() {
    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>> OPCOES DE MENU <<<<<<<<<<<<<<<<<<<<<<<<");
    printf(" \n|| (1) IMPRIMIR ARQUIVO");
    printf(" \n|| (2) PESQUISAR FUNCIONARIO");
    printf(" \n|| (3) REALIZAR BUSCAS POR 50 FUNCIONARIOS ALEATORIOS");
    printf(" \n|| (4) REMOVER FUNCIONARIO");
    printf(" \n|| (5) REALIZAR A REMOCAO DE 50 FUNCIONARIOs ALEATORIOS");
    printf(" \n|| (0) SAIR");
}


void MENU(FILE *arq, bTree *tree, queue *q0) {
    int codigo;

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
                btDestroy(tree, arq);
                system("PAUSE");
                break;
            case 1:
                system("cls");
                printf("\n|| IMPRIMINDO FUNCIONARIOS DENTRO DO ARQUIVO ||\n");
                btPrintTree(tree, q0, arq);
                printf("\n");
                system("PAUSE");
                system("cls");
                break;
            case 2:
                system("cls");
                printf("\n|| PESQUISANDO FUNCIONARIO ||\n");
                printf("|| Qual o codigo do funcionario: ");
                scanf("%d", &codigo);
                element ke = {.codigo = codigo};
                int search = btSearch(tree->root, tree->order, ke, arq);
                if (search != -1) {
                    btPrintElement(*tree, ke, arq);
                } else {
                    printf("Funcionario nao encontrado.\n");
                }

                printf("\n");
                system("PAUSE");
                system("cls");

                break;
            case 3:
                system("cls");
                for (int i = 0; i < 50; i++) {
                    codigo = 1 + (rand() % 5500);
                    element ke2 = {.codigo = codigo};
                    printf("\n\n********************************************************************************************");
                    printf("\n|| Codigo do funcionario: %d\n\n", codigo);
                    int search2 = btSearch(tree->root, tree->order, ke2, arq);
                    if (search2 != -1) {
                        btPrintElement(*tree, ke2, arq);
                        printf("\n\n********************************************************************************************");
                    } else {
                        printf("Funcionario nao encontrado.\n");
                        printf("\n\n********************************************************************************************");
                    }
                }

                printf("\n");
                system("PAUSE");
                system("cls");

                break;
            case 4:
                system("cls");
                printf("\n|| REMOVENDO FUNCIONARIO ||\n");
                printf("|| Qual o codigo do funcionario: ");
                scanf("%d", &codigo);
                element ke_remove = {.codigo = codigo};
                int removed = btDelete(tree, ke_remove, arq);
                if (removed == 1) {
                    printf("|| Funcionario de codigo %d removido com sucesso!", codigo);
                } else
                    printf("|| Funcionario de codigo %d nao esta na arvore!", codigo);
                printf("\n");
                system("PAUSE");
                system("cls");
                break;
            case 5:
                system("cls");
                for (int i = 0; i < 50; i++) {
                    codigo = 1 + (rand() % 2000);
                    element ke2 = {.codigo = codigo};
                    if (codigo != 1023) {
                        printf("\n\n********************************************************************************************");
                        printf("\n|| Codigo do funcionario: %d\n\n", codigo);
                        int search2 = btSearch(tree->root, tree->order, ke2, arq);
                        if (search2 != -1) {
                            btPrintElement(*tree, ke2, arq);
                            printf("\n\n********************************************************************************************");
                        } else {
                            printf("Funcionario nao encontrado.\n");
                            printf("\n\n********************************************************************************************");
                        }
                    } else {
                        i--;
                    }
                }
                printf("\n");
                system("PAUSE");
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