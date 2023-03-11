#include "btree.h"
#include "queue.h"

int main() {
    FILE *fp;
    fp = fopen("file.dat", "wb+");

    bTree *tree = btCreate(4);

    for (int i = 1; i <= 350; ++i) {
        element n1 = {.codigo = i, .salario = 3000};
        btInsert(tree, n1, fp);
    }

    queue *q0 = createQueue(2000);

    btPrintTree(tree, q0, fp);
    printf("\n");

    element key = {.codigo = 1000};
    
    element n2 = {.codigo = 150};
    btDelete(tree, n2, fp);

    int search = btSearch(tree->root, tree->order, key, fp);
    printf("%d\n", search);
//    queue *q = createQueue(15);
//
//    btPrintTree(tree, q, fp);
//    printf("\n");

//    queue *q1 = createQueue(15);
//
//    btPrintTree(tree, q1, fp);
//    printf("\n");

    element max = btfindMax(tree->root, tree->order, fp);
    printf("max: %d\n", max.codigo);

    element min = btfindMin(tree->root, tree->order, fp);
    printf("min: %d\n", min.codigo);

    btDestroy(tree, fp);
//    system("PAUSE");
    return 0;
}
