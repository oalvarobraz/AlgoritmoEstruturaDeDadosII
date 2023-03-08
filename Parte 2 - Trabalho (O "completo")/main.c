//
// Created by Alvaro on 05/03/2023.
//
#include <stdlib.h>
#include <stdio.h>
#include "arvore_b_mais.h"
#include "interface.h"

int menu(){
    int opt;
    printf("\nOpcoes:\n");
    printf("0. Sair\n");
    printf("1. Inserir\n");
    printf("2. Remover\n");
    printf("3. Inserir varios funcionarios aleatorios.\n");
    printf("4. Imprimir arvore.\n");
    printf("5. Buscar funcionario.\n");
    printf("7. Carregar funcionario apartir de um arquivo.\n");
    printf("8. Modificar funcionario.\n");
    scanf("%d", &opt);

    return opt;
}

//void tarefa(int opt, int d) {
//
//    if (opt == 0){
//
//        printf("Saindo...\n");
//
//    }else if(opt == 1){//Inserir
//        int cod;
//        char nome[50];
//        double salario;
//        printf("\nInformacoes da pizza:\n");
//        printf("Codigo:\n");
//        scanf("%d", &cod);
//        printf("Nome\n");
//        scanf(" %[^\n]", nome);
//        printf("Salario\n");
//        scanf("%lf", &salario);
//        if(insere(cod, nome,salario, "mdados.dat", "indice.dat", "funcionario_arvore.dat", d) != -1){
//            printf("Inserido com Sucesso\n");
//        }else printf("Codigo ja exitente\n");
//
//    }else if(opt == 2){//Remover
//
//        int cod;
//        printf("Codigo:\n");
//        scanf("%d", &cod);
//        exclui(cod, "mdados.dat", "indice.dat", "funcionario_arvore.dat", d);
//
//    }else if(opt == 3){ //Insere 5000 funcionarios aleatorios
//        int max = 0;
//        srand(time(NULL));
//        printf("Qauntos funcionarios voce deseja inserir?");
//        scanf("%d", &max);
//        int i = 0;
//        while (i<max){
//            insere(rand() % max + 1, "Fulano", 1300, "mdados.dat", "indice.dat", "funcionario_arvore.dat", d);
//            i++;
//        }
//
//    }else if(opt == 4){//Imprimir arvore
//
//        if(!imprimirArvore(d, "funcionario_arvore.dat","indice.dat", "mdados.dat")) printf("Sem Funcionarios cadastradas.\n");
//
//    }else if(opt == 5){//Buscar Funcionario
//
//        int cod;
//        printf("Codigo:\n");
//        scanf("%d", &cod);
//        if(!buscaFuncionario(cod, "mdados.dat", "indice.dat", "funcionario_arvore.dat", d))
//            printf("Funcionario nao existente.\n");
//
//    }else if(opt == 6){//Buscar todas as pizzas de uma categoria
//
//
//    }else if(opt == 7){//Carregar pizzas a partir de um arquivo
//
//        char nome_arquivo_entrada[20];
//
//        printf("Nome do arquivo de entrada(com .txt ou .dat).\n");
//        scanf(" %s", nome_arquivo_entrada);
//        carrega_dados(d, nome_arquivo_entrada, "mdados.dat", "indice.dat", "funcionario_arvore.dat");
//
//    }else if(opt == 8){//modificar pizza.
//
//        int cod;
//        char nome[50];
//        double salario;
//
//        printf("Codigo:\n");
//        scanf("%d", &cod);
//        if(buscaFuncionario(cod, "mdados.dat", "indice.dat", "funcionario_arvore.dat", d)) {
//            printf("Novo nome:\n");
//            scanf(" %[^\n]", nome);
//            printf("Novo salario:\n");
//            scanf(" %lf", &salario);
//            modifica(cod, nome, salario, "mdados.dat", "indice.dat", "funcionario_arvore.dat", d);
//        }else printf("Não existe essa pizza.\n");
//
//    }else{
//        printf("Tarefa invalida.\n");
//    }
//}


void tarefa(int opt, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d) {

    if (opt == 0){

        printf("Saindo...\n");

    }else if(opt == 1){//Inserir
        int cod;
        char nome[50];
        char categoria[20];
        float preco;
        printf("\nInformacoes da pizza:\n");
        printf("Codigo:\n");
        scanf("%d", &cod);
        printf("Nome\n");
        scanf(" %[^\n]", nome);
        printf("Preço\n");
        scanf("%f", &preco);
        if(insere(cod, nome, preco, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d) != -1){
            printf("Inserido com Sucesso\n");
        }else printf("Codigo ja exitente\n");

    }else if(opt == 2){//Remover

        int cod;
        printf("Codigo:\n");
        scanf("%d", &cod);
        exclui(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

    }else if(opt == 3){//Remove todas as pizzas de uma categoria
        system("cls");
        printf("\n|| INSERINDO VARIOS FUNCIONARIOS ALEATORIAMENTE||\n");
        int max = 0;
        printf("|| Quantos funcionarios voce deseja inserir: ");
        scanf("%d", &max);
        int i = 0;
        while (i<max){
            if(insere(i, "Fulano", 1300,nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d) != -1){
                printf("Inserido com Sucesso\n");
            }else printf("Codigo ja exitente\n");
            i++;
        }

    }else if(opt == 4){//Imprimir arvore

        if(!imprimirArvore(d, nome_arquivo_dados,nome_arquivo_indice, nome_arquivo_metadados)) printf("Sem pizzas cadastradas.\n");

    }else if(opt == 5){//Buscar pizza

        int cod;
        printf("Codigo:\n");
        scanf("%d", &cod);
        if(!buscaFuncionario(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d))
            printf("Pizza nao existente.\n");

    }else if(opt == 6){//Buscar todas as pizzas de uma categoria

    }else if(opt == 7){//Carregar pizzas a partir de um arquivo

        char nome_arquivo_entrada[20];

        printf("Nome do arquivo de entrada(com .txt ou .dat).\n");
        scanf(" %s", nome_arquivo_entrada);
        carrega_dados(d, nome_arquivo_entrada, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados);

    }else if(opt == 8) {//modificar pizza.

    }
}


//int main(){
//    int opt;
//    int d;
//    printf("Digite a ordem da arvore.\n");
//    scanf(" %d", &d);
//
//    int flag;
//    printf("\nDigite 1 caso deseje apagar o conteudo existente nesses arquivos e 0 se desejar manter.\n");
//    scanf(" %d", &flag);
//    if(flag){
//        fclose(fopen("metadados.dat", "wb"));//Apaga o conteudo do arquivo.
//        fclose(fopen("indice.dat", "wb"));
//        fclose(fopen("funcionarios.dat", "wb"));
//    }
//
//    do{
//
//        opt = menu();
//        tarefa(opt,"metadados.dat", "indice.dat", "funcionarios.dat", d);
//
//    }while(opt);
//
//    return 0;
//}

int main(){

    fclose(fopen("metadadosteste.dat", "wb"));//Apaga o conteudo do arquivo.
    fclose(fopen("indiceteste.dat", "wb"));
    fclose(fopen("arvoreteste.dat", "wb"));
    MENU("metadadosteste.dat", "indiceteste.dat", "arvoreteste.dat");

    return 0;
}

//metadados = "mdados.dat"
//indice = "indice.dat"
//arq = "funcionario_arvore.dat"