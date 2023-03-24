#include <stdio.h>
#include "interface.h"

int main() {

    int tam;
    printf("|| Informe o tamanho: ");
    scanf("%d", &tam);
    MENU("funcionarios.dat", tam);

    return 0;
}
