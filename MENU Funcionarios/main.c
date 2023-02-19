#include <stdio.h>
#include "interface.h"

int main()
{
    //declara ponteiro para arquivo
    FILE *arq;
    //abre arquivo para leitura e escrita
    if ((arq = fopen("funcionario.dat", "wb+")) == NULL) {
        printf("Erro ao abrir arquivo\n");
    } else {
        MENU(arq);
        fclose(arq);
    }

    return 0;
}
