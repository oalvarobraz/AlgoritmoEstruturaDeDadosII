//
// Created by Alvaro on 04/03/2023.
//
#define RIGHT 1
#define  LEFT 2
#include <limits.h>
#include <stdlib.h>
#include "arvore_b_mais.h"

//Preenche o arquivo metadados se ele estiver vazio.
void checa_mdados(char *nome_arquivo_metadados, int d){

    TMDados *metadados1 = le_arq_mdados(nome_arquivo_metadados);
    if(!metadados1){//Coloca valores no metadados
        metadados1 = mdados(d, 0, 1, 0, 0);
        salva_arq_mdados(nome_arquivo_metadados, metadados1);
        free(metadados1);
    }

}

//Insere um no interno
void insereNoInterno(int cod, int d, char *nome_arquivo_indice, FILE *arq_Dados , TMDados *metadados, int pos, int flag){


    FILE *arq_Indices = fopen(nome_arquivo_indice, "rb+");
    fseek(arq_Indices, pos, SEEK_SET);

    TNoInterno *noPai = le_no_interno(d, arq_Indices);

    if(noPai->m < 2*d) {//Caso caiba

        TNoInterno *novo = no_interno_vazio(d);
        novo->pont_pai = noPai->pont_pai;
        novo->flag_aponto_folha = noPai->flag_aponto_folha;

        //Guarda todos de forma ordenada no nó
        int j = 0;
        novo->pont[j] = noPai->pont[j];
        while (j < noPai->m && cod > noPai->chaves[j]) {
            novo->chaves[j] = noPai->chaves[j];
            j++;
            novo->pont[j] = noPai->pont[j];
        }

        novo->chaves[j] = cod;
        novo->m = noPai->m + 1;

        if(novo->flag_aponto_folha) novo->pont[j + 1] = metadados->pont_prox_no_folha_livre;
        else novo->pont[j + 1] = metadados->pont_prox_no_interno_livre;

        while (j + 1 < noPai->m + 1) {
            novo->chaves[j + 1] = noPai->chaves[j];
            j++;
            novo->pont[j + 1] = noPai->pont[j];
        }

        fseek(arq_Indices, pos, SEEK_SET);
        salva_no_interno(d, novo, arq_Indices);
        libera_no_interno(novo);

        fclose(arq_Indices);

    }else{//Caso precise dividir

        int guardaChaves[(2 * d) + 1];
        int guardaP[(2 * d) + 2];
        int j = 0;
        //Guarda todas em um vetor
        guardaP[j] = noPai->pont[j];
        while (j < noPai->m && cod > noPai->chaves[j]) {
            guardaChaves[j] = noPai->chaves[j];
            j++;
            guardaP[j] = noPai->pont[j];
        }

        guardaChaves[j] = cod;
        if(noPai->flag_aponto_folha) guardaP[j + 1] = metadados->pont_prox_no_folha_livre;
        else guardaP[j + 1] = metadados->pont_prox_no_interno_livre;

        while (j + 1 < noPai->m + 1) {
            guardaChaves[j + 1] = noPai->chaves[j];
            j++;
            guardaP[j + 1] = noPai->pont[j];
        }



        //Acha o meio na hora de mandar pra cima.
        int meio;
        if (j % 2 == 0) meio = j / 2;
        else meio = j / 2 + 1;

        TNoInterno *novo = no_interno_vazio(d);
        novo->pont_pai = noPai->pont_pai;
        novo->flag_aponto_folha = noPai->flag_aponto_folha;
        TNoInterno *novo2 = no_interno_vazio(d);
        novo2->pont_pai = noPai->pont_pai;
        novo2->flag_aponto_folha = noPai->flag_aponto_folha;

        //Divide o vetor em 2 novos no
        int k1 = 0;
        int k2 = 0;
        novo->pont[k1] = noPai->pont[k1];
        while (k1 < meio) {
            novo->chaves[k1] = guardaChaves[k1];
            k1++;
            novo->pont[k1] = guardaP[k1];
            novo->m++;
        }

        novo2->pont[k2] = guardaP[meio + 1];
        TNoFolha *mudaP;
        TNoInterno *mudaPI;
        while (k2 < meio) {

            if(novo2->flag_aponto_folha) { //Para alterar o pai do no
                fseek(arq_Dados, novo2->pont[k2], SEEK_SET);
                mudaP = le_no_folha(d, arq_Dados);
                mudaP->pont_pai = metadados->pont_prox_no_interno_livre;
                fseek(arq_Dados, novo2->pont[k2], SEEK_SET);
                salva_no_folha(d, mudaP, arq_Dados);
            }else if(flag == 1){
                fseek(arq_Indices, novo2->pont[k2], SEEK_SET);
                mudaPI = le_no_interno(d, arq_Indices);
                mudaPI->pont_pai = metadados->pont_prox_no_interno_livre + flag * tamanho_no_interno(d);
                fseek(arq_Indices, novo2->pont[k2], SEEK_SET);
                salva_no_interno(d, mudaPI, arq_Indices);
            }

            novo2->chaves[k2] = guardaChaves[k2 + meio + 1];
            k2++;
            novo2->pont[k2] = guardaP[k2 + meio + 1];
            novo2->m++;
        }

        if(novo2->flag_aponto_folha) { //Para alterar o pai
            fseek(arq_Dados, novo2->pont[k2], SEEK_SET);
            mudaP = le_no_folha(d, arq_Dados);
            mudaP->pont_pai = metadados->pont_prox_no_interno_livre;
            fseek(arq_Dados, novo2->pont[k2], SEEK_SET);
            salva_no_folha(d, mudaP, arq_Dados);
            libera_no_folha(d, mudaP);
        }else if(flag == 1){
            fseek(arq_Indices, novo2->pont[k2], SEEK_SET);
            mudaPI = le_no_interno(d, arq_Indices);
            mudaPI->pont_pai = metadados->pont_prox_no_interno_livre + flag * tamanho_no_interno(d);
            fseek(arq_Indices, novo2->pont[k2], SEEK_SET);
            salva_no_interno(d, mudaPI, arq_Indices);
            libera_no_interno(mudaPI);
        }
        //-------------------------------------------------------------------//

        //Se tem pai
        if(noPai->pont_pai!=-1){

            fseek(arq_Indices, pos, SEEK_SET);
            salva_no_interno(d, novo, arq_Indices);
            libera_no_interno(novo);

            fseek(arq_Indices, metadados->pont_prox_no_interno_livre, SEEK_SET);
            salva_no_interno(d, novo2, arq_Indices);
            libera_no_interno(novo2);

            fclose(arq_Indices);

            insereNoInterno(guardaChaves[meio], d, nome_arquivo_indice, arq_Dados, metadados, noPai->pont_pai, flag+1);

            arq_Indices = fopen(nome_arquivo_indice, "rb+");
            fseek(arq_Indices, 0, SEEK_END);
            metadados->pont_prox_no_interno_livre = ftell(arq_Indices); //Passa a apontar para o final do arquivo.
            fclose(arq_Indices);

        }

        else {
            if(flag!=0){
                metadados->pont_prox_no_interno_livre += flag * tamanho_no_interno(d);
            }

            metadados->pont_raiz = metadados->pont_prox_no_interno_livre + tamanho_no_interno(d);
            novo->pont_pai = metadados->pont_raiz;
            novo2->pont_pai = metadados->pont_raiz;
            TNoInterno *novoPai = no_interno_vazio(d);
            novoPai->pont[0] = pos;
            novoPai->pont[1] = metadados->pont_prox_no_interno_livre;
            novoPai->chaves[0] = guardaChaves[meio];
            novoPai->m = 1;
            novoPai->flag_aponto_folha = 0;

            fseek(arq_Indices, pos, SEEK_SET);
            salva_no_interno(d, novo, arq_Indices);
            libera_no_interno(novo);

            fseek(arq_Indices, metadados->pont_prox_no_interno_livre, SEEK_SET);
            salva_no_interno(d, novo2, arq_Indices);
            libera_no_interno(novo2);

            salva_no_interno(d, novoPai, arq_Indices);
            libera_no_interno(novoPai);
            fseek(arq_Indices, 0, SEEK_END);
            metadados->pont_prox_no_interno_livre = ftell(arq_Indices);//Passa a apontar para o final do arquivo.

            fclose(arq_Indices);
        }

    }
}

//Insere um no folha
void insereNoFolha(TNoFolha *no, int cod, TFunc *p, int d, FILE *arq_Dados, TMDados *metadados, int pos, char *nome_arquivo_indice){

    if(no->m < 2*d) {//Caso caiba

        TNoFolha *novo = no_folha_vazio(d);
        novo->pont_pai = no->pont_pai;
        novo->pont_prox = no->pont_prox;

        //Guarda todos clientes de forma ordenada no nó
        int j = 0;
        while (j < no->m && cod > no->func[j]->cod) {
            novo->func[j] = no->func[j];
            j++;
        }

        novo->func[j] = p;
        novo->m = no->m + 1;

        while (j + 1 < no->m + 1) {
            novo->func[j + 1] = no->func[j];
            j++;
        }
        //------------------------------------------------------//
        fseek(arq_Dados, pos, SEEK_SET);

        salva_no_folha(d, novo, arq_Dados);
        libera_no_folha(d, novo);

    } else { //Caso o no esteja cheio

        TFunc *guardaPizza[(2 * d) + 1];
        int j = 0;
        //Guarda todos os clientes em um vetor
        while (j < no->m && cod > no->func[j]->cod) {
            guardaPizza[j] = no->func[j];
            j++;
        }

        guardaPizza[j] = p;

        while (j + 1 < no->m + 1) {
            guardaPizza[j + 1] = no->func[j];
            j++;
        }
        //-------------------------------------------------------------------//

        int meio;
        if (j % 2 == 0) meio = j / 2;
        else meio = j / 2 + 1;

        TNoFolha *novo = no_folha_vazio(d);
        novo->pont_pai = no->pont_pai;
        novo->pont_prox = metadados->pont_prox_no_folha_livre;//Aponta para o local onde o novo2 estara

        TNoFolha *novo2 = no_folha_vazio(d);
        novo2->pont_pai = no->pont_pai;
        novo2->pont_prox = no->pont_prox;

        //Separa nos no
        int k1;
        int k2 = 0;
        for (k1 = 0; k1 < no->m + 1; k1++) {
            if (k1 < meio) {
                novo->func[k1] = guardaPizza[k1];
                novo->m++;
            } else if (k1 >= meio) {
                novo2->func[k2] = guardaPizza[k1];
                novo2->m++;
                k2++;
            }
        }
        //---------------------------------------------------//
        if (no->pont_pai != -1) { //Caso tenha um pai

            fseek(arq_Dados, pos, SEEK_SET);
            salva_no_folha(d, novo, arq_Dados);
            libera_no_folha(d, novo);

            fseek(arq_Dados, metadados->pont_prox_no_folha_livre, SEEK_SET);
            salva_no_folha(d, novo2, arq_Dados);
            int sobe = novo2->func[0]->cod;
            libera_no_folha(d, novo2);

            insereNoInterno(sobe, d, nome_arquivo_indice, arq_Dados, metadados, no->pont_pai, 0);

            fseek(arq_Dados, 0, SEEK_END);
            metadados->pont_prox_no_folha_livre = ftell(arq_Dados);

            //Cria um pai
        }else {

            metadados->pont_raiz = metadados->pont_prox_no_interno_livre;
            novo->pont_pai = metadados->pont_raiz;
            novo2->pont_pai = metadados->pont_raiz;
            metadados->raiz_folha = 0;
            TNoInterno *novoPai = no_interno_vazio(d);
            novoPai->pont[0] = pos;
            novoPai->pont[1] = metadados->pont_prox_no_folha_livre;
            novoPai->chaves[0] = guardaPizza[meio]->cod;
            novoPai->m = 1;
            novoPai->flag_aponto_folha = 1;

            FILE *arq_Indices = fopen(nome_arquivo_indice, "rb+");
            fseek(arq_Indices, metadados->pont_prox_no_interno_livre, SEEK_SET);
            salva_no_interno(d, novoPai, arq_Indices);
            metadados->pont_prox_no_interno_livre = ftell(arq_Indices);//Passa a apontar para o final do arquivo.
            fclose(arq_Indices);
            libera_no_interno(novoPai);

            fseek(arq_Dados, pos, SEEK_SET);
            salva_no_folha(d, novo, arq_Dados);
            libera_no_folha(d, novo);

            fseek(arq_Dados, metadados->pont_prox_no_folha_livre, SEEK_SET);
            salva_no_folha(d, novo2, arq_Dados);
            libera_no_folha(d, novo2);

            fseek(arq_Dados, 0, SEEK_END);
            metadados->pont_prox_no_folha_livre = ftell(arq_Dados);//Passa a apontar para o final do arquivo.

        }

    }
}


//retorna o elemento e a posição dele
int busca_binaria_arvore(void *r, int *pos, int chave, int esq, int dir, int folha){

    if(!r)return INT_MIN;
    int meio = (esq + dir)/2;
    *pos = meio;
    if(folha){
        if(dir >= esq){
            TNoFolha *raiz = r;
            if(raiz->func[meio]->cod < chave){
                return busca_binaria_arvore(raiz, pos, chave, meio + 1, dir, folha);
            }
            else if(raiz->func[meio]->cod == chave){
                *pos = meio;
                return chave;
            }
            return busca_binaria_arvore(raiz, pos, chave, esq, meio - 1, folha);
        }
    }else{
        if(dir >= esq){

            TNoInterno *raiz = r;

            if(raiz->chaves[meio] < chave){
                return busca_binaria_arvore(raiz, pos, chave, meio + 1, dir, folha);
            }else if(raiz->chaves[meio] == chave && raiz->flag_aponto_folha){
                //printf("oi porra desgraça\n");
                *pos = meio;
                return chave;
            }return busca_binaria_arvore(raiz, pos, chave, esq, meio - 1, folha);
        }
    }return -1;
}

//Verifica se o codigo já existe
int buscaFolha(int pos, char *nome_arquivo_dados, int cod, int d){

    FILE *arq_Dados = fopen(nome_arquivo_dados, "rb");
    fseek(arq_Dados, pos, SEEK_SET);
    TNoFolha *noFolha = le_no_folha(d, arq_Dados);
    int x = 0;
    if(noFolha){
        int p = busca_binaria_arvore(noFolha, &x, cod, 0, noFolha->m - 1, 1);
        if(p == -1) return 0;
        else return 1;
    }
    return 0;
}

int busca(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d) {
    FILE *arq_Indice = fopen(nome_arquivo_indice, "rb");

    TMDados *metadados = le_arq_mdados(nome_arquivo_metadados);
    if (metadados->raiz_folha) {
        if (metadados->pont_raiz != -1) return metadados->pont_raiz;
        else {
            return metadados->pont_prox_no_folha_livre;
        }
    } else {
        fseek(arq_Indice, metadados->pont_raiz, SEEK_SET);

        TNoInterno *noInterno = le_no_interno(d, arq_Indice);

        int i;
        while (noInterno) {

            //Busca binaria//
            int inf = 0;
            int sup = noInterno->m - 1;
            int meio = 0;
            while (inf <= sup) {
                meio = (inf + sup) / 2;
                if (cod < noInterno->chaves[meio])
                    sup = meio - 1;
                else
                    inf = meio + 1;
            }
            //-------------------//

            if (meio == noInterno->m) meio--;
            if (cod >= noInterno->chaves[meio]) {
                if (noInterno->flag_aponto_folha) {
                    return noInterno->pont[meio + 1];
                } else {
                    fseek(arq_Indice, noInterno->pont[meio + 1], SEEK_SET);
                    noInterno = le_no_interno(d, arq_Indice);
                }
            } else if (cod < noInterno->chaves[meio]) {
                if (noInterno->flag_aponto_folha) {
                    return noInterno->pont[meio];
                } else {
                    fseek(arq_Indice, noInterno->pont[meio], SEEK_SET);
                    noInterno = le_no_interno(d, arq_Indice);
                }

            }

        }
    }
    return 0;
}

int insere(int cod, char *nome, double salario, char *nome_arquivo_metadados, char *nome_arquivo_indice,char *nome_arquivo_dados, int d)
{

    checa_mdados(nome_arquivo_metadados, d);

    int pos = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

    if(!buscaFolha(pos, nome_arquivo_dados, cod, d)){

        TMDados *metadados = le_arq_mdados(nome_arquivo_metadados);

        FILE *arq_Dados = fopen(nome_arquivo_dados, "rb+");

        fseek(arq_Dados, pos, SEEK_SET);

        TFunc *p = funcionario(cod, nome, salario);

        TNoFolha *noFolha = le_no_folha(d, arq_Dados);

        if(noFolha) {
            insereNoFolha(noFolha, cod, p, d, arq_Dados, metadados, pos, nome_arquivo_indice);
        }else{
            TNoFolha *novoFolha = cria_no_folha(d, -1, -1, 1, p);
            fseek(arq_Dados, pos, SEEK_SET);
            salva_no_folha(d, novoFolha, arq_Dados);
            libera_no_folha(d, novoFolha);
            fseek(arq_Dados, 0 ,SEEK_END);
            metadados->pont_prox_no_folha_livre = ftell(arq_Dados);
            metadados->pont_raiz = pos;
            metadados->raiz_folha = 1;
        }

        salva_arq_mdados(nome_arquivo_metadados, metadados);

        fclose(arq_Dados);

        pos = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

        return pos;

    }else{

        return -1;
    }

}

//int insere(int cod, char *nome, double salario, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
//{
//
//    //ABRE OS ARQUIVOS NECESSÁRIOS
//    FILE * arq_indice = fopen(nome_arquivo_indice, "rb+");
//    FILE * arq_dados = fopen(nome_arquivo_dados, "rb+");
//    FILE * arq_metadados = fopen(nome_arquivo_metadados, "rb+");
//
//    //BUSCA O CÓDIGO A TENTAR SER INSERIDO E RETORNA O PONTEIRO PARA O NÓ ONDE ESTÁ OU DEVERIA ESTAR
//    int var_busca = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);
//    //COLOCA O CURSOR NA POSIÇÃO RETORNADA
//    fseek(arq_dados, var_busca, SEEK_SET);
//
//    //VERIFICA SE O COD JÁ PERTENCE AO NO FOLHA;
//    TNoFolha * noFolha;
//    if (var_busca != -1){
//        noFolha = le_no_folha(d, arq_dados);
//        for (int i = 0; i < noFolha->m; i++){
//            if (noFolha->func[i]->cod == cod)
//                return -1;
//        }
//    }
//
//    //CASO NÃO TENHA ENCONTRADO, COLOCA O CURSOR NO MESMO LOCAL E LÊ O NÓ NOVAMENTE
//    fseek(arq_dados, var_busca, SEEK_SET);
//    noFolha = le_no_folha(d, arq_dados);
//
//    //CRIA A PIZZA A SER INSERIDA POSTERIORMENTE
//    TFunc *p = funcionario(cod, nome, salario);
//
//
//    //VERIFICA SE AINDA HÁ ESPAÇO PARA INSERÇÃO NO NÓ FOLHA
//    if(noFolha->m < (2 * d)){
//
//        //INSERE PIZZA NO FINAL DO NÓ E INCREMENTA M EM UMA UNIDADE
//        noFolha->func[noFolha->m] = p;
//        noFolha->m++;
//
//        //REORDENA O VETOR DE PIZZAS POR CÓDIGO DA PIZZA
//        for (int i = 1; i < noFolha->m; i++) {
//
//            for (int j = 0; j < noFolha->m - i; j++) {
//                if (noFolha->func[j]->cod > noFolha->func[j + 1]->cod) {
//                    TFunc *aux = noFolha->func[j];
//                    noFolha->func[j] = noFolha->func[j + 1];
//                    noFolha->func[j + 1] = aux;
//                }
//            }
//        }
//
//        //SETA O PONTEIRO PARA O QUE FOI RETORNADO DA BUSCA
//        fseek(arq_dados, var_busca, SEEK_SET);
//
//        //SALVA O ARQUIVO DE DADOS E FECHA ARQUIVOS
//        salva_no_folha(d, noFolha, arq_dados);
//        fclose(arq_dados);
//        fclose(arq_indice);
//        fclose(arq_metadados);
//
//        return var_busca;
//
//    }
//        //CASO NÃO HAJA ESPAÇO PARA INSERÇÃO NO NÓ FOLHA
//    else{
//
//        //DA O SEEK NO ARQUIVO DE INDICE ATÉ O CORRESPONDENTE
//        fseek(arq_indice, noFolha->pont_pai, SEEK_SET);
//
//        TFunc *aux = funcionario(cod, nome, salario);
//
//        //CRIAR NOVO NÓ FOLHA VAZIO
//        TNoFolha * novoNo = no_folha_vazio(d);
//
//        //PERCORRE A NO FOLHA E VERIFICA ONDE DEVE SER INSERIDO A NOVA PIZZA
//        int controle = 0;
//        for (int i = 0; i < noFolha->m; i++) {
//
//            if (cod < noFolha->func[i]->cod)
//                break;
//
//            controle++;
//        }
//
//        //POSIÇÕES MAIORES QUE D VÃO PARA O NOVO NÓ
//        int trocou = 0;
//        if (controle >= d) {
//            int aux_index = 0;
//            int aux_tam = noFolha->m;
//            for (int i = d; i < aux_tam; i++) {
//                novoNo->func[aux_index] = funcionario(noFolha->func[i]->cod, noFolha->func[i]->nome, noFolha->func[i]->salario);
//                noFolha->func[i] = NULL;
//                noFolha->m--;
//                novoNo->m++;
//                aux_index++;
//            }
//            //ADICIONA NOVA PIZZA NO FINAL DO NOVO NÓ
//            novoNo->func[novoNo->m] = p;
//            novoNo->m++;
//        }
//            //INSERE NO NÓ FOLHA E MOVE OUTROS PARA NOVO NÓ
//        else {
//            int aux_index = 0;
//            int aux_tam = noFolha->m;
//            for (int i = 0; i < aux_tam; i++) {
//
//                if (i >= d - 1) {
//                    novoNo->func[aux_index] = funcionario(noFolha->func[i]->cod, noFolha->func[i]->nome, noFolha->func[i]->salario);
//                    noFolha->func[i] = NULL;
//                    noFolha->m--;
//                    novoNo->m++;
//                    aux_index++;
//                }
//            }
//            noFolha->func[noFolha->m] = p;
//            noFolha->m++;
//
//        }
//        //ORDENA NO FOLHA
//        for (int i = 1; i < noFolha->m; i++) {
//            for (int j = 0; j < noFolha->m - i; j++) {
//                if (noFolha->func[j]->cod > noFolha->func[j + 1]->cod) {
//                    TFunc *aux = noFolha->func[j];
//                    noFolha->func[j] = noFolha->func[j + 1];
//                    noFolha->func[j + 1] = aux;
//                }
//            }
//        }
//
//        //ORDENA NOVO NO FOLHA
//        for (int i = 1; i < novoNo->m; i++) {
//            for (int j = 0; j < novoNo->m - i; j++) {
//                if (novoNo->func[j]->cod > novoNo->func[j + 1]->cod) {
//                    TFunc *aux = novoNo->func[j];
//                    novoNo->func[j] = novoNo->func[j + 1];
//                    novoNo->func[j + 1] = aux;
//                }
//            }
//        }
//
//        //VERIFICA SE O CÓDIGO ESTÁ NO NOVO NÓ (PARA CONTROLE DE RETORNO)
//        for (int i = 0; i < novoNo->m; i++){
//            if(novoNo->func[i]->cod == cod)
//                trocou = 1;
//
//        }
//        //ABRE O ARQUIVO DE METADADOS PARA ATUALIZAR AS REFERENCIAS
//        TMDados *metadados = le_arq_mdados(nome_arquivo_metadados);
//
//        //VARIÁVEL PARA CONTROLAR O QUE RETORNAR
//        int var_retorno;
//
//        //CASO NÃO TENHA TROCADO DE NÓ FOLHA
//        if(trocou == 0){
//            var_retorno = var_busca;
//        }
//            //CASO TENHA TROCADO DE NÓ FOLHA
//        else{
//            var_retorno = metadados->pont_prox_no_folha_livre;
//        }
//
//        //VERIFICA SE É RAIZ
//        if(metadados->raiz_folha == 0){
//
//            //COLOCA O CURSOR NO PAI DO NÓ FOLHA E LÊ O NÓ INTERNO
//            fseek(arq_indice, noFolha->pont_pai, SEEK_SET);
//            TNoInterno * noInterno = le_no_interno(d, arq_indice);
//
//            //PEGA A CHAVE QUE SUBIRÁ PARA O PAI
//            int chave = novoNo->func[0]->cod;
//            int aux_chave;
//
//            //VARIÁVEL DE CONTROLE
//            int control = 0;
//
//            //AUXILIARES PARA ARMAZENAR PONTEIROS TEMPORÁRIOS
//            int aux_pont, aux_pont2;
//
//            //ACERTA AS CHAVES E PONTEIROS DO NÓ INTERNO
//            //O ESPAÇO PARA O NOVO PONTEIRO FICA NO LUGAR ONDE O -2 FOI INSERIDO
//            for(int i = 0; i < noInterno->m; i++){
//
//                //VERIFICA SE A CHAVE É MENOR QUE NO INDICE ATUAL DO NÓ INTERNO
//                if(noInterno->chaves[i] > chave){
//
//                    //CASO NÃO TENHA ALTERADO AINDA, ALTERA E TROCA VARIÁVEL DE CONTROLE
//                    if(control == 0){
//                        aux_pont = noInterno->pont[i + 1];
//                        noInterno->pont[i + 1] = -2;
//                        control = 1;
//                    }
//                    else{
//                        aux_pont2 = noInterno->pont[i + 1];
//                        noInterno->pont[i + 1] = aux_pont;
//                        aux_pont = aux_pont2;
//                    }
//                    aux_chave = noInterno->chaves[i];
//                    noInterno->chaves[i] = chave;
//                    chave = aux_chave;
//                }
//            }
//
//            //ATRIBUI A CHAVE NO FINAL DO NÓ INTERNO E INCREMENTA O VALOR DE M
//            noInterno->chaves[noInterno->m] = chave;
//            noInterno->m++;
//
//            //CASO VARIÁVEL DE CONTROLE SEJA IGUAL ESTADO INICIAL, COLOCA -2 PARA DEPOIS ATUALIZAR O PONTEIRO NO LOCAL
//            if(control == 0){
//                noInterno->pont[noInterno->m] = -2;
//            }
//                //CASO CONTRÁRIO, ATRIBUI PONTEIRO NO FINAL DO NÓ INTERNO
//            else{
//                noInterno->pont[noInterno->m] = aux_pont;
//            }
//
//            //PEGA O ENDEREÇO DO PRÓXIMO NÓ FOLHA LIVRE, PARA SALVAR NOVO NÓ FOLHA
//            int pont_novo_no = metadados->pont_prox_no_folha_livre;
//
//            //ATRIBUI ALGUNS ITENS AO NOVO NÓ FOLHA
//            novoNo->pont_pai = noFolha->pont_pai;
//            novoNo->pont_prox = noFolha->pont_prox;
//            noFolha->pont_prox = pont_novo_no;
//
//            //SALVA ARQUIVO DE DADOS (NO FOLHA)
//            fseek(arq_dados, var_busca, SEEK_SET);
//            salva_no_folha(d, noFolha, arq_dados);
//
//            //SALVA ARQUIVO DE DADOS (NOVO NÓ FOLHA)
//            fseek(arq_dados, pont_novo_no, SEEK_SET);
//            salva_no_folha(d, novoNo, arq_dados);
//
//            //VERIFICA SE VARIÁVEL DE CONTROLE É IGUAL ESTADO INICIAL, SE FOR, ADICIONA NOVO PONTEIRO NO FINAL DO NÓ INTERNO
//            if(control == 0){
//                noInterno->pont[noInterno->m] = pont_novo_no;
//            }
//                //CASO CONTRÁRIO, SIGNIFICA QUE FOI ALTERADO ACIMA E EM ALGUM PONTEIRO O -2 ESTÁ PRESENTE
//                //PERCORRE NÓ INTERNO ATÉ ENCONTRAR O -2 E ENTÃO ALTERA O PONTEIRO DELE
//            else{
//                for(int i = 0; i < noInterno->m; i++){
//
//                    if(noInterno->pont[i] == -2){
//                        noInterno->pont[i] = pont_novo_no;
//                        break;
//                    }
//                }
//            }
//
//            //ATUALIZAR ARQUIVO METADADOS E SALVA
//            metadados->pont_prox_no_folha_livre = metadados->pont_prox_no_folha_livre + tamanho_no_folha(d);
//            salva_mdados(metadados, arq_metadados);
//
//            //VARIÁVEL PARA CONTROLE DO LOOP
//            int loop_control = 0;
//
//            //VERIFICA SE O NÓ INTERNO ESTÁ CHEIO
//            if(noInterno->m > (2 * d)){
//                int pont_noInterno = noFolha->pont_pai;
//                int chave_aux;
//
//                //INICIALIZA NOVO NÓ E POSSÍVEL NÓ A SER UTILIZADO
//                TNoInterno * novoNoInterno;
//                TNoInterno * pai_noInterno;
//
//                while(loop_control == 0){
//
//                    //PEGA A CHAVE DO MEIO DO NÓ INTERNO
//                    chave_aux = noInterno->chaves[d];
//
//                    //CRIA UM NOVO NO VAZIO
//                    novoNoInterno = no_interno_vazio(d);
//
//                    //VERIFICA SE NÓ INTERNO NÃO TEM PAI, CASO NÃO TENHA, CRIA UM
//                    if(noInterno->pont_pai == -1){
//                        pai_noInterno = no_interno_vazio(d);
//                        loop_control = 1;
//                    }
//                    else{
//                        //CASO CONTRÁRIO, POSICIONA O CURSOR E LÊ O PAI
//                        fseek(arq_indice, noInterno->pont_pai, SEEK_SET);
//                        pai_noInterno = le_no_interno(d, arq_indice);
//                    }
//
//                    //TRANSFERE OS PONTEIROS PARA O NOVO NÓ INTERNO E ATUALIZA O NÓ INTERNO
//                    //PERCORRE ATÉ M + 1 POIS SÃO M + 1 PONTEIROS
//                    for(int i = d + 1; i < noInterno->m + 1; i++){
//
//                        novoNoInterno->pont[i - d - 1] = noInterno->pont[i];
//                        noInterno->pont[i] = -1;
//
//                        if(noInterno->flag_aponto_folha == 0 && i < noInterno->m)
//                            novoNoInterno->chaves[i - d - 1] = noInterno->chaves[i];
//
//                        noInterno->chaves[i - 1] = -1;
//                        novoNoInterno->m = novoNoInterno->m + 1;
//                    }
//                    noInterno->m = d;
//
//                    //CONSERTA M DO NÓ INTERNO E O APONTA FOLHA
//                    novoNoInterno->m--;
//                    novoNoInterno->flag_aponto_folha = noInterno->flag_aponto_folha;
//
//                    //ATUALIZA AS CHAVES DO NOVO NÓ INTERNO
//                    for(int i = 0; i < novoNoInterno->m; i++){
//
//                        if(novoNoInterno->flag_aponto_folha == 1){
//
//                            fseek(arq_dados, novoNoInterno->pont[i + 1], SEEK_SET);
//                            TNoFolha * aux_folha = le_no_folha(d, arq_dados);
//                            novoNoInterno->chaves[i] = aux_folha->func[0]->cod;
//
//                        }
//                    }
//
//                    //ATUALIZA OS PONTEIROS PARA O PAI DOS NÓS FOLHAS E/OU INTERNOS
//                    for(int i = 0; i < novoNoInterno->m + 1; i++){
//
//                        if(novoNoInterno->flag_aponto_folha == 1){
//                            fseek(arq_dados, novoNoInterno->pont[i], SEEK_SET);
//                            TNoFolha * aux_folha = le_no_folha(d, arq_dados);
//                            aux_folha->pont_pai = metadados->pont_prox_no_interno_livre;
//                            fseek(arq_dados, novoNoInterno->pont[i], SEEK_SET);
//                            salva_no_folha(d, aux_folha, arq_dados);
//
//                        }
//                        else{
//                            fseek(arq_indice, novoNoInterno->pont[i], SEEK_SET);
//                            TNoInterno * aux_interno = le_no_interno(d, arq_indice);
//                            aux_interno->pont_pai = metadados->pont_prox_no_interno_livre;
//                            fseek(arq_indice, novoNoInterno->pont[i], SEEK_SET);
//                            salva_no_interno(d, aux_interno, arq_indice);
//
//                        }
//                    }
//                    //VERIFICA SE O PAI É UM NOVO NÓ INTERNO
//                    if(loop_control == 1){
//
//                        //CASO SEJA, ADICIONA OS PONTEIROS, A CHAVE E INCREMENTA M
//                        pai_noInterno->pont[0] = pont_noInterno;
//                        pai_noInterno->pont[1] = metadados->pont_prox_no_interno_livre;
//                        pai_noInterno->chaves[0] = chave_aux;
//                        pai_noInterno->m = 1;
//
//                        //ATUALIZA O PONTEIRO DO PAI DO NOVO NÓ INTERNO E O SALVA
//                        novoNoInterno->pont_pai = metadados->pont_prox_no_interno_livre + tamanho_no_interno(d);
//                        fseek(arq_indice, metadados->pont_prox_no_interno_livre, SEEK_SET);
//                        salva_no_interno(d, novoNoInterno, arq_indice);
//
//                        //ATUALIZA O PONTEIRO DO PAI DO NÓ INTERNO E O SALVA
//                        noInterno->pont_pai = metadados->pont_prox_no_interno_livre + tamanho_no_interno(d);
//                        fseek(arq_indice, pont_noInterno, SEEK_SET);
//                        salva_no_interno(d, noInterno, arq_indice);
//
//                        //SALVA PAI DO NÓ INTERNO
//                        fseek(arq_indice, metadados->pont_prox_no_interno_livre + tamanho_no_interno(d), SEEK_SET);
//                        salva_no_interno(d, pai_noInterno, arq_indice);
//
//                        //ATUALIZA METADADOS E SALVA
//                        metadados->pont_raiz = metadados->pont_prox_no_interno_livre + tamanho_no_interno(d);
//                        metadados->pont_prox_no_interno_livre = metadados->pont_prox_no_interno_livre + (2*tamanho_no_interno(d));
//                        salva_arq_mdados(nome_arquivo_metadados, metadados);
//                    }
//                        //CASO O PAI NÃO SEJA UM NOVO NÓ INTERNO
//                    else{
//                        int aux_pos = -2;
//
//                        //PROCURA INDICE A SER ADICIONADO A CHAVE
//                        for(int i = 0; i < pai_noInterno->m; i++){
//                            if(pai_noInterno->chaves[i] > noInterno->chaves[0]){
//                                aux_pos = i;
//                                break;
//                            }
//                        }
//
//                        //VERIFICA SE ENCONTROU O INDICE
//                        if(aux_pos != -2){
//
//                            for(int i = pai_noInterno->m - 1; i > aux_pos; i++){
//                                pai_noInterno->chaves[i + 1] = pai_noInterno->chaves[i];
//                                pai_noInterno->pont[i + 2] = pai_noInterno->pont[i + 1];
//                            }
//
//                            pai_noInterno->pont[aux_pos + 1] = metadados->pont_prox_no_interno_livre;
//                            pai_noInterno->chaves[aux_pos] = chave_aux;
//
//                        }
//                            //CASO CONTRÁRIO, ADICIONA NO FINAL E INCREMENTA M
//                        else{
//                            pai_noInterno->chaves[pai_noInterno->m] = chave_aux;
//                            pai_noInterno->pont[pai_noInterno->m + 1] = metadados->pont_prox_no_interno_livre;
//                            pai_noInterno->m++;
//                        }
//
//                        //AJUSTA PAI DO NOVO NÓ INTERNO E SALVA-O
//                        novoNoInterno->pont_pai = noInterno->pont_pai;
//                        fseek(arq_indice, metadados->pont_prox_no_interno_livre, SEEK_SET);
//                        salva_no_interno(d, novoNoInterno, arq_indice);
//
//                        //SALVA NÓ INTERNO
//                        fseek(arq_indice, pont_noInterno, SEEK_SET);
//                        salva_no_interno(d, noInterno, arq_indice);
//
//                        //SALVA O PAI DE NÓ INTERNO
//                        fseek(arq_indice, noInterno->pont_pai, SEEK_SET);
//                        salva_no_interno(d, pai_noInterno, arq_indice);
//
//                        //ATUALIZA METADADOS
//                        metadados->pont_prox_no_interno_livre = metadados->pont_prox_no_interno_livre + tamanho_no_interno(d);
//
//                        //VERIFICA SE O PAI DO NÓ INTERNO ESTÁ OK
//                        if(pai_noInterno->m <= (2 * d)){
//                            loop_control = 1;
//                        }
//                            //CASO NÃO ESTEJA, VOLTA PRO LOOP
//                        else{
//                            pont_noInterno = noInterno->pont_pai;
//                            noInterno = pai_noInterno;
//                        }
//                    }
//                }
//            }
//            else{
//                //SALVAR ARQUIVO DE INDICE
//                fseek(arq_indice, noFolha->pont_pai, SEEK_SET);
//                salva_no_interno(d, noInterno, arq_indice);
//            }
//
//            //FECHA ARQUIVOS
//            fclose(arq_indice);
//            fclose(arq_dados);
//
//            salva_arq_mdados(nome_arquivo_metadados, metadados);
//
//            return var_retorno;
//        }
//            //CASO NÃO SEJA RAIZ
//        else{
//
//            //LÊ NÓ INTERNO E AJUSTA PONTEIROS E CHAVES
//            TNoInterno * noInterno = no_interno_vazio(d);
//            noInterno->pont[0] = var_busca;
//            noInterno->pont[1] = metadados->pont_prox_no_folha_livre;
//            noInterno->chaves[0] = novoNo->func[0]->cod;
//            noInterno->m = 1;
//            noInterno->flag_aponto_folha = 1;
//
//            //SALVA NÓ INTERNO
//            fseek(arq_indice, 0, SEEK_SET);
//            salva_no_interno(d, noInterno, arq_indice);
//            free(noInterno);
//
//            //CONSERTA PONTEIROS DO NÓ FOLHA
//            noFolha->pont_prox = metadados->pont_prox_no_folha_livre;
//            noFolha->pont_pai = 0;
//
//            //SALVAR NÓ FOLHA
//            fseek(arq_dados, var_busca, SEEK_SET);
//            salva_no_folha(d, noFolha, arq_dados);
//
//            //CONSERTA PONTEIROS DO NOVO NÓ E SALVA
//            novoNo->pont_pai = 0;
//
//            fseek(arq_dados, metadados->pont_prox_no_folha_livre, SEEK_SET);
//            salva_no_folha(d, novoNo, arq_dados);
//
//            //FECHA ARQUIVOS
//            fclose(arq_dados);
//            fclose(arq_indice);
//
//            //AJUSTA OS METADADOS
//            metadados->pont_raiz = 0;
//            metadados->raiz_folha = 0;
//            metadados->pont_prox_no_interno_livre = tamanho_no_interno(d);
//            metadados->pont_prox_no_folha_livre = metadados->pont_prox_no_folha_livre + tamanho_no_folha(d);
//
//            salva_mdados(metadados, arq_metadados);
//
//            fclose(arq_metadados);
//
//            return var_retorno;
//        }
//    }
//}

//Remove uma funcionario do no folha
TNoFolha *retira_funcionario(TNoFolha *no, int pos){
    int i;
    for(i = pos; i < no->m - 1; i++){
        if(no->func[i+1]){
            no->func[i] = no->func[i+1];
        }
    }
    no->func[no->m-1] = NULL;
    no->m--;
    return no;
}

TNoInterno *retira_interno(TNoInterno *no, int pos){
    int i;
    for(i = pos; i < no->m - 1; i++){
        if(no->chaves[i+1]){
            no->chaves[i] = no->chaves[i+1];
        }
    }
    no->chaves[no->m-1] = -1;
    no->m--;
    return no;
}

//Move as func
TNoFolha *shift(TNoFolha *no){
    int i;
    for(i = no->m; i>0; i--){
        no->func[i] = no->func[i-1];
    }
    return no;
}

//Acho que nao sera necessaria.
TNoInterno *shift_interno(TNoInterno *pai){//faz um shift
    //tenho q deslocar tanto os indices quanto
    //{1,5,9}
    //{n,(2,3,4), (6,7,8), (10,11,23)}
    if(pai){
        int i;
        for(i = 0; i < pai->m; i++){
            if(i >= pai->m-1){//pra nao dar segfault
                pai->pont[i] = pai->pont[i + 1];
            }
            else{
                pai->chaves[i] = pai->chaves[i+1];
                pai->pont[i] = pai->pont[i + 1];
            }
        }
        //anulo a ultima posição q tinha valor
        int j = pai->m;
        while(j > pai->m-2){
            if(pai->pont[j] != -1){
                pai->pont[j] = -1;
                pai->chaves[j-1] = -1;
                break;
            }
            j++;
        }
        pai->m--;
        return pai;
    }return NULL;
}

TNoInterno * get_irmao_op_interno(int d,FILE *dados,TNoInterno *pai, TNoInterno *no, int pos, int *op, int *index){//retorna a posicao do irmao e a operação que será feita com ele
    // op = -1 = redist com o irmao da direita. op = 0 = redist com o filho da esquerda. op = 1 = concatenação com o filho da direita.
    int i = 0;
    int p = 0;//indice dos filhos

    if(pai){
        for(i = 0; i <= pai->m; i++){
            if(pai->pont[i] == pos){
                *index = p = i;
                break;
            }
        }if(p == 0){
            fseek(dados, pai->pont[p + 1], SEEK_SET);
            TNoInterno *dir = le_no_interno(d, dados);
            if(dir){
                if(no->m + dir->m < 2*d){
                    *op = 1;
                }
                else{
                    *op = -1;
                }
            }
            return dir;
        }else{
            //carrego o no da direita e da esquerda
            TNoInterno *dir;
            TNoInterno *esq;
            fseek(dados, pai->pont[p - 1], SEEK_SET);
            esq = le_no_interno(2, dados);
            fseek(dados, pai->pont[p + 1], SEEK_SET);
            dir = le_no_interno(2, dados);
            //testo qual é o melhor e qual operacao
            if(dir){
                if(dir->m + no->m == 2*d){//redistribuição com o da direita
                    *op = -1;
                    return dir;
                }else if(!esq){//concatenação
                    *op = 1;
                    return dir;
                }
            }else if(esq){
                if(esq->m + no->m == 2*d){//redistribuição com o da esquerda
                    *op = 0;
                    return esq;
                }else{
                    *op = 2;
                    return esq;
                }
            }
        }
    }
    return NULL;
}

//Retorna o irmão e a operacao q tem q ser feita
TNoFolha * get_irmao_op(int d,FILE *dados,TNoInterno *pai, TNoFolha *no, int pos, int *op, int *index){//retorna a posicao do irmao e a operação que será feita com ele
    // op = -1 = redist com o irmao da direita. op = 0 = redist com o filho da esquerda. op = 1 = concatenação com o filho da direita.
    int i = 0;
    int p = 0;//indice dos filhos

    if(pai){
        for(i = 0; i <= pai->m; i++){
            if(pai->pont[i] == pos){
                *index = p = i;
                break;
            }
        }
        if(p == 0){
            fseek(dados, pai->pont[p + 1], SEEK_SET);
            TNoFolha *dir = le_no_folha(d, dados);
            if(dir){
                if(no->m + dir->m < 2*d){
                    *op = 1;
                }
                else{
                    *op = -1;
                }
            }
            return dir;
        }else{
            //carrego o no da direita e da esquerda
            TNoFolha *dir;
            TNoFolha *esq;
            fseek(dados, pai->pont[p - 1], SEEK_SET);
            esq = le_no_folha(2, dados);
            fseek(dados, pai->pont[p + 1], SEEK_SET);
            dir = le_no_folha(2, dados);
            //testo qual é o melhor e qual operacao
            if(dir){
                if(dir->m + no->m == 2*d){//redistribuição com o da direita
                    *op = -1;
                    return dir;
                }else if(!esq){//concatenação
                    *op = 1;
                    return dir;
                }
            }else if(esq){
                if(esq->m + no->m == 2*d){//redistribuição com o da esquerda
                    *op = 0;
                    return esq;
                }else{
                    *op = 2;
                    return esq;
                }
            }
        }
    }return NULL;
}

void redistribui_dir_interno(TNoInterno *dir, TNoInterno *esq, TNoInterno *pai){
    int index = 0;
    busca_binaria_arvore(pai, &index, esq->chaves[0], 0, pai->m - 1, 0);
    dir = shift_interno(dir);
    dir->chaves[0] = pai->chaves[index];
    pai->chaves[index] = esq->chaves[esq->m-1];
    dir->pont[0] = esq->pont[esq->m];
    esq->chaves[esq->m-1] = -1;
    esq->pont[esq->m] = -1;
    esq->m--;
    dir->m++;
}

void redistribui_esq_interno(TNoInterno *dir, TNoInterno *esq, TNoInterno *pai){
    int index = 0;
    busca_binaria_arvore(pai, &index, esq->chaves[0], 0, pai->m - 1, 0);
    esq->chaves[esq->m] = pai->chaves[index];
    pai->chaves[index] = dir->chaves[0];
    esq->pont[esq->m + 1] = dir->pont[0];

    int i = 0;
    for(;i<dir->m - 1;i++){
        dir->chaves[i] = dir->chaves[i+1];
        dir->pont[i] = dir->pont[i + 1];
    }
    dir->pont[dir->m] = -1;
    dir->chaves[dir->m-1] = -1;
    esq->m++;
    dir->m--;
}
//Redistribui o irmão da direita com o da esquerda
void redistribui_dir(TNoFolha *dir, TNoFolha *esq, TNoInterno *pai){
    esq->func[esq->m] = dir->func[0];//passei do irmao da direita pro da esquerda;
    int cod = esq->func[esq->m]->cod;
    dir = retira_funcionario(dir, 0);//retirei o valor da direita e mantive salvo em cod
    int pos = 0;
    int ind = 0;
    ind = busca_binaria_arvore(pai, &pos, cod, 0, pai->m - 1, 0);//achei a posição do pai que tem os filhos
    pai->chaves[pos] = dir->func[0]->cod;//atualizei o indice
    esq->m++;
}


//Redistribui o irmão da esquerda pro da direita
void redistribui_esq(TNoFolha *dir, TNoFolha *esq, TNoInterno *pai){
    int cod = esq->func[esq->m-1]->cod;
    dir = shift(dir);
    dir->func[0] = esq->func[esq->m-1];
    int pos = 0;
    busca_binaria_arvore(pai, &pos, cod, 0, pai->m - 1, 0);//achei a posição do pai que tem os filhos
    pai->chaves[pos] = cod;//atualizei o indice
}


int q_filhos(TNoInterno *pai){
    if(pai){
        int i;
        int q = 1;
        for(i = 0; i < pai->m; i++){
            if(pai->pont[i] != -1)q++;
        }
        return q;
    }
    return 0;
}



void concatena_interno(TNoInterno *w, TNoInterno *irmao, TNoInterno *pai, int d, int op, int index, FILE *dados, FILE *indice){
    int p_irmao = 0;
    if(op == RIGHT) {
        busca_binaria_arvore(pai, &p_irmao, irmao->chaves[0], 0, irmao->m - 1, 0);
        index = p_irmao - 1;

        w->chaves[w->m] = pai->chaves[index];
        w->pont[w->m + 1] = irmao->pont[0];
        w->m++;
        if(w->flag_aponto_folha) {//Altera o ponteiro do no pai nos noh folha
            fseek(dados, irmao->pont[0], SEEK_SET);
            TNoFolha *filho = le_no_folha(d, dados);
            filho->pont_pai = pai->pont[index];
            fseek(dados, irmao->pont[0], SEEK_SET);
            salva_no_folha(d, filho, dados);//salva noh com o valor do pai alterado
            for(int i = index + 1; i<= irmao->m ;i++){
                fseek(dados, irmao->pont[i], SEEK_SET);
                filho = le_no_folha(d, dados);
                filho->pont_pai = pai->pont[index];
                fseek(dados, irmao->pont[i], SEEK_SET);
                salva_no_folha(d, filho, dados);
            }
        }else{//Altera o ponteiro pro pai
            fseek(indice, irmao->pont[0], SEEK_SET);
            TNoInterno *filho = le_no_interno(d, indice);
            filho->pont_pai = pai->pont[index];
            fseek(dados, irmao->pont[0], SEEK_SET);
            salva_no_interno(d, filho, indice);//salva noh com o valor do pai alterado
            for(int i = index + 1; i<= irmao->m ;i++){
                fseek(dados, irmao->pont[i], SEEK_SET);
                filho = le_no_interno(d, indice);
                filho->pont_pai = pai->pont[index];
                fseek(dados, irmao->pont[i], SEEK_SET);
                salva_no_interno(d, filho, indice);
            }

        }
        for (int i = 0; i < irmao->m; i++){
            w->chaves[w->m] = irmao->chaves[i];
            w->pont[w->m + 1] = irmao->pont[i + 1];
            w->m++;
        }
        for (int i = index; i < pai->m - 1; i++) {

            pai->chaves[i] = pai->chaves[i + 1];
            if(pai->chaves[i] != -1) //
                pai->pont[i + 1] = pai->pont[i + 2];
        }
        //Garante que a ultima posição será -1
        pai->chaves[index] = -1;
        pai->pont[pai->m] = -1;
        pai->m--;

    }else if(op == LEFT){
        busca_binaria_arvore(pai, &p_irmao, irmao->chaves[0], 0, pai->m, 0);
        index = p_irmao;

        irmao = shift_interno(irmao);
        irmao->chaves[0] = pai->chaves[index];
        irmao->pont[0] = w->pont[w->m];
        irmao->m++;
        if(irmao->flag_aponto_folha) {
            fseek(dados, w->pont[w->m], SEEK_SET);
            TNoFolha *filho = le_no_folha(d, dados);
            filho->pont_pai = pai->pont[index];
            salva_no_folha(d, filho, dados);
            for(int i = index + 1; i< irmao->m ;i++){
                fseek(dados, w->pont[i], SEEK_SET);
                filho = le_no_folha(d, dados);
                filho->pont_pai = pai->pont[index];
                salva_no_folha(d, filho, dados);
            }

        }else{

        }

        for (int i =0; i < w->m; i++) {

            irmao->chaves[irmao->m] = w->chaves[i];
            irmao->pont[irmao->m + 1] = w->pont[i + 1];
            irmao->m++;
        }

        for (int i = index; i < pai->m; i++) {
            pai->chaves[i] = pai->chaves[i + 1];
            if(pai->chaves[i] != -1)
                pai->pont[i + 1] = pai->pont[i + 2];
        }

        //Garante que a ultima posição será -1
        if(pai->m == 2*d) pai->chaves[pai->m - 1] = -1;
        pai->pont[pai->m] = -1;
        pai->m--;

    }

}

void concatena(TNoFolha *no, TNoFolha *irmao, TNoInterno *pai, int d, int op, int ind_pai ,int *index, FILE *dados){
    if(op == RIGHT) {
        for (int i =0; i < irmao->m; i++){
            no->func[no->m] = irmao->func[i];
            no->m++;
        }

        //*index = 0; //Para descobrir qual index do pai vai ser apagado
        //for(int i = 1; pai->pont[i] != no->pont_prox; i++ ) (*index)++;
        busca_binaria_arvore(pai, index, irmao->func[0]->cod, 0, irmao->m - 1, 0);
        no->pont_prox = irmao->pont_prox;
        for (int i = *index; i < pai->m; i++) {

            pai->chaves[i] = pai->chaves[i + 1];
            if(pai->chaves[i] != -1) // Se a nova chave for diferente de -1 ele leva o ponteiro do irmao
                pai->pont[i + 1] = irmao->pont_prox;
            fseek(dados, irmao->pont_prox, SEEK_SET);
            irmao = le_no_folha(d, dados);
        }
        //Garante que a ultima posição será -1
        if(pai->m == 2*d) pai->chaves[pai->m - 1] = -1;
        pai->pont[pai->m] = -1;
        pai->m--;

    }else if(op == LEFT){
        for (int i =0; i < no->m; i++) {

            irmao->func[irmao->m] = no->func[i];
            irmao->m++;
        }

        //*index = 0; //Para descobrir qual index do pai vai ser apagado
        //for(int i = 1; pai->pont[i] != irmao->pont_prox; i++ ) (*index)++;
        busca_binaria_arvore(pai, index, irmao->func[0]->cod, 0, irmao->m - 1, 0);

        irmao->pont_prox = no->pont_prox; //Aponta para onde o noh excluido apontava

        for (int i = *index; i < pai->m; i++) {
            pai->chaves[i] = pai->chaves[i + 1];
            if(pai->chaves[i] != -1)// Se a nova chave for diferente de -1 ele leva o ponteiro do irmao
                pai->pont[i + 1] = no->pont_prox;
            fseek(dados, no->pont_prox, SEEK_SET);
            no = le_no_folha(d, dados);
        }

        //Garante que a ultima posição será -1
        if(pai->m == 2*d) pai->chaves[pai->m - 1] = -1;
        pai->pont[pai->m] = -1;
        pai->m--;

    }
}

void exclui_interno(int d, FILE *indice, TNoInterno *w, TNoInterno *pai, TNoInterno *irmao, int op, int index,
                    char *nome_arquivo_metadados, FILE *dados){

    if(op == -1){//redistribui com o da direita
        redistribui_dir_interno(irmao, w, pai);

        fseek(indice, pai->pont[index], SEEK_SET);//vou gravar o noh pont
        salva_no_interno(d, w, indice);
        fseek(indice, pai->pont[index + 1], SEEK_SET);
        salva_no_interno(d, irmao, indice);
        fseek(indice, w->pont_pai, SEEK_SET);
        salva_no_interno(d, w, indice);

    }
    else if(op == 0){//redistribui com o da esquerda
        redistribui_esq_interno(irmao, w, pai);

        fseek(indice, pai->pont[index], SEEK_SET);//vou gravar o noh pont
        salva_no_interno(d, w, indice);
        fseek(indice, pai->pont[index + 1], SEEK_SET);
        salva_no_interno(d, irmao, indice);
        fseek(indice, w->pont_pai, SEEK_SET);
        salva_no_interno(d, w, indice);


    }else{//concatena

        concatena_interno(w, irmao, pai, d, op, index, dados, indice);

        if(pai->pont_pai == -1 && q_filhos(pai) == 1){//vai ter q apagar esse pai

            TMDados *dados_aux = le_arq_mdados(nome_arquivo_metadados);

            irmao->pont_pai = -1;
            w->pont_pai = -1;
            dados_aux->pont_raiz = pai->pont[0];//vai ser sempre essa posição (graças a deus)
            salva_arq_mdados(nome_arquivo_metadados, dados_aux);
            fseek(indice, pai->pont[0], SEEK_SET);
            if(op == RIGHT)
                salva_no_interno(d, w, indice);
            else if (op == LEFT)
                salva_no_interno(d, irmao, indice);

        }else if(pai->m < d && pai->pont_pai != -1){//vai propagar. pode ser redistribuição de noh interno ou
            int i;//Acha o indice
            fseek(indice, pai->pont_pai, SEEK_SET);
            TNoInterno *p = le_no_interno(d, indice);
            TNoInterno *irmao_interno = get_irmao_op_interno(d, indice, p, pai, w->pont_pai, &op, &i);
            exclui_interno(d, indice, pai, p, irmao_interno, op, i, nome_arquivo_metadados, dados);
            fseek(indice, pai->pont[index], SEEK_SET);

        }else{
            //salvo na posicao q o pai já estava
            fseek(indice, irmao->pont_pai, SEEK_SET);
            salva_no_interno(d, pai, indice);
            fseek(indice, pai->pont[index], SEEK_SET);

            if(op == RIGHT)
                salva_no_interno(d, w, indice);
            else if (op == LEFT)
                salva_no_interno(d, irmao, indice);

        }


    }
}

int exclui(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
    int pos = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);
    TMDados *dados_aux = le_arq_mdados(nome_arquivo_metadados);
    if(buscaFolha(pos, nome_arquivo_dados, cod, d)){
        FILE * dados = fopen(nome_arquivo_dados, "rb+");

        if(!dados)return -1;
        FILE *indice = fopen(nome_arquivo_indice, "rb+");
        fseek(dados, pos, SEEK_SET);
        TNoFolha * no = le_no_folha(d, dados);

        int p = 0;
        busca_binaria_arvore(no, &p, cod, 0, no->m, 1);

        if(no->m == 1){

            no = retira_funcionario(no, p);

            fseek(dados, pos, SEEK_SET);
            salva_no_folha(d, no, dados);

            libera_no_folha(d, no);

            dados_aux->pont_raiz = -1;
            fseek(dados, 0, SEEK_END);
            dados_aux->pont_prox_no_folha_livre = ftell(dados);
            fseek(indice, 0, SEEK_END);
            dados_aux->pont_prox_no_interno_livre = ftell(indice);
            salva_arq_mdados(nome_arquivo_metadados, dados_aux);
            fclose(indice);
            fclose(dados);

        }else if(no->m > d || no->pont_pai == -1){//nao tem concatenacao nem redistribuicao

            no = retira_funcionario(no, p);

            fseek(dados, pos, SEEK_SET);
            salva_no_folha(d, no, dados);

            libera_no_folha(d, no);
            fclose(dados);

        }else{//caso de haver redistribuição ou concatenação

            fseek(indice, no->pont_pai, SEEK_SET);
            TNoInterno *w = le_no_interno(d, indice);
            if(!w)return -1;
            int op = -2;
            int index = 0;
            no = retira_funcionario(no, p);
            TNoFolha *irmao = get_irmao_op(d, dados, w, no, pos, &op, &index);

            if(op == -1){
                redistribui_dir(irmao, no, w);

                fseek(dados, w->pont[index], SEEK_SET);//vou gravar o noh pont
                salva_no_folha(d, no, dados);
                fseek(dados, w->pont[index + 1], SEEK_SET);
                salva_no_folha(d, irmao, dados);
                fseek(indice, no->pont_pai, SEEK_SET);
                salva_no_interno(d, w, indice);
                fclose(indice);
                fclose(dados);
            }
            else if(op == 0){
                redistribui_esq(irmao, no, w);

                fseek(dados, w->pont[index], SEEK_SET);//vou gravar o noh pont
                salva_no_folha(d, no, dados);
                fseek(dados, w->pont[index + 1], SEEK_SET);
                salva_no_folha(d, irmao, dados);
                fseek(indice, no->pont_pai, SEEK_SET);
                salva_no_interno(d, w, indice);
                fclose(indice);

                fclose(dados);

            }else{//concatena

                int guardaP;
                concatena(no, irmao, w, d, op, no->pont_pai, &guardaP, dados);

                if(w->pont_pai == -1 && q_filhos(w) == 1){//vai ter q apagar esse pai

                    irmao->pont_pai = -1;
                    dados_aux->pont_raiz = w->pont[0];//vai ser sempre essa posição (graças a deus)
                    dados_aux->raiz_folha = 1;
                    salva_arq_mdados(nome_arquivo_metadados, dados_aux);
                    pos = w->pont[0];
                    fseek(dados, pos, SEEK_SET);
                    salva_no_folha(d, irmao, dados);
                    fclose(dados);
                    libera_no_interno(w);
                    libera_no_folha(d, no);

                }else if(w->m < d && w->pont_pai != -1){//vai propagar. pode ser redistribuição de noh interno ou
                    fseek(indice, w->pont_pai, SEEK_SET);
                    TNoInterno *pai = le_no_interno(d, indice);
                    TNoInterno *irmao_interno = get_irmao_op_interno(d, indice, pai, w, no->pont_pai, &op, &index);
                    exclui_interno(d, indice, w, pai, irmao_interno, op, index, nome_arquivo_metadados, dados);

                    fseek(dados, w->pont[guardaP], SEEK_SET);

                    if(op == RIGHT)
                        salva_no_folha(d, no, dados);
                    else if (op == LEFT)
                        salva_no_folha(d, irmao, dados);

                    fclose(indice);
                    fclose(dados);

                }else{//salvo na posicao q o pai já estava

                    fseek(indice, irmao->pont_pai, SEEK_SET);
                    salva_no_interno(d, w, indice);

                    fseek(dados, w->pont[guardaP], SEEK_SET);

                    if(op == RIGHT)
                        salva_no_folha(d, no, dados);
                    else if (op == LEFT)
                        salva_no_folha(d, irmao, dados);

                    fclose(indice);
                    fclose(dados);

                }

            }
        }
        return pos;
    }else return -1;
}

void carrega_dados(int d, char *nome_arquivo_entrada, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados) {

    TListaFunc *pizzas;
    fclose(fopen(nome_arquivo_dados, "wb"));//Apaga o conteudo do arquivo.
    fclose(fopen(nome_arquivo_indice, "wb"));
    fclose(fopen(nome_arquivo_metadados, "wb"));
    pizzas = le_funcs(nome_arquivo_entrada);

    for (int i = 0; i < pizzas->qtd; i++) {

        insere(pizzas->lista[i]->cod,
               pizzas->lista[i]->nome,
               pizzas->lista[i]->salario,
               nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d
        );


    }

}

//Imprime as func
int imprimirArvore(int d, char *nome_arquivo_dados, char *nome_arquivo_indice, char *nome_arquivo_metadados) {

    TMDados *metadados = le_arq_mdados(nome_arquivo_metadados);

    if (!metadados) return 0;//Caso nao tenha nada no metadados então a arvore está vazia


    if (metadados->pont_raiz == -1){
        return 0;
    }
    if(metadados->raiz_folha){

        FILE *arq_Dados = fopen(nome_arquivo_dados, "rb");
        fseek(arq_Dados, metadados->pont_raiz, SEEK_SET);
        TNoFolha *noFolha = le_no_folha(d, arq_Dados);
        imprime_no_folha(d, noFolha);
        return 1;
    }else {
        FILE *arq_Indice = fopen(nome_arquivo_indice, "rb");

        fseek(arq_Indice, metadados->pont_raiz, SEEK_SET);
        TNoInterno *noInterno = le_no_interno(d, arq_Indice);

        while(!noInterno->flag_aponto_folha){
            fseek(arq_Indice, noInterno->pont[0], SEEK_SET);
            noInterno = le_no_interno(d, arq_Indice);
        }
        FILE *arq_Dados = fopen(nome_arquivo_dados, "rb");

        fseek(arq_Dados, noInterno->pont[0] , SEEK_SET);

        TNoFolha *noFolha = le_no_folha(d, arq_Dados);
        if (noFolha) {
            imprime_no_folha(d, noFolha);
            while (noFolha->pont_prox != -1) {
                fseek(arq_Dados, noFolha->pont_prox, SEEK_SET);
                noFolha = le_no_folha(d, arq_Dados);
                imprime_no_folha(d, noFolha);
            }
            return 1;
        } else return 0;
    }

}

//Verifica se o codigo da funcionario existe
int buscaFuncionario(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d){

    int pos = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

    if(buscaFolha(pos, nome_arquivo_dados, cod, d)) {

        FILE *arq_Dados = fopen(nome_arquivo_dados, "rb");
        fseek(arq_Dados, pos, SEEK_SET);
        TNoFolha *noFolha = le_no_folha(d, arq_Dados);
        int i;
        for(i = 0; cod != noFolha->func[i]->cod; i++);
        TFunc *p = noFolha->func[i];
        imprime(p);
        fclose(arq_Dados);
        return 1;
    }else return 0;
}

//Modifica a funcionario
void modifica(int cod, char *nome, double salario, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d){

    TFunc *p = funcionario(cod, nome, salario);

    int pos = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

    FILE *arq_Dados = fopen(nome_arquivo_dados, "rb+");
    fseek(arq_Dados, pos, SEEK_SET);
    TNoFolha *noFolha = le_no_folha(d, arq_Dados);
    int i = 0;
    while(noFolha->func[i]->cod != cod) i++;

    noFolha->func[i] = p;

    fseek(arq_Dados, pos, SEEK_SET);
    salva_no_folha(d, noFolha, arq_Dados);

    libera_no_folha(d, noFolha);
    free(p);

    fclose(arq_Dados);
}