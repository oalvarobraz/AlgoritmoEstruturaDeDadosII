#include "btree.h"

int calculate_offset(int disk, int order) {

    int size_of_btNode = (sizeof(int) * 3) + (sizeof(element) * order - 1) + (sizeof(int) * order);

    return size_of_btNode * disk;    // Retorna a posição do nó no arquivo

}

btNode disk_read(int disk, int order, FILE *fp) {
    btNode read_node;

    int offset = calculate_offset(disk, order);
    fseek(fp, offset, SEEK_SET);

    // Lendo as informações do arquiivo
    fread(&read_node.num_funcs, sizeof(read_node.num_funcs), 1, fp);
    fread(&read_node.flag_folha, sizeof(read_node.flag_folha), 1, fp);
    fread(&read_node.pos_in_disk, sizeof(read_node.pos_in_disk), 1, fp);

    read_node.funcs = malloc(sizeof(element) * order - 1);
    fread(read_node.funcs, sizeof(element), order - 1, fp);

    read_node.filhos = malloc(sizeof(int) * order);
    fread(read_node.filhos, sizeof(int), order, fp);


    return read_node;
}

void disk_write(btNode node, int order, FILE *fp) {

    int offset = calculate_offset(node.pos_in_disk, order);
    fseek(fp, offset, SEEK_SET);

    // Escrevendo no arquivo
    fwrite(&node.num_funcs, sizeof(node.num_funcs), 1, fp);
    fwrite(&node.flag_folha, sizeof(node.flag_folha), 1, fp);
    fwrite(&node.pos_in_disk, sizeof(node.pos_in_disk), 1, fp);
    fwrite(node.funcs, sizeof(element), order - 1, fp);
    fwrite(node.filhos, sizeof(int), order, fp);
}

btNode new_node(int order, int is_leaf) {
    btNode n;

    n.num_funcs = 0;
    n.flag_folha = is_leaf;

    n.funcs = malloc((order - 1) * sizeof(element));
    for (int i = 0; i < order - 1; i++) {
        n.funcs[i].codigo = -1;
        n.funcs[i].salario = -1;
    }

    n.filhos = malloc((order) * sizeof(int));
    for (int i = 0; i < order; i++) {
        n.filhos[i] = -1;
    }

    return n;
}

void print_node_keys(btNode node, int order) {
    printf("\n[ %d", node.pos_in_disk);
    for (int i = 0; i < order - 1; i++) {
        if (node.funcs[i].codigo != -1) {
            printf("\n  || Codigo: %d\n ", node.funcs[i].codigo);
            printf(" || Nome: Fulano\n ");
            printf(" || Salario: R$ %.2f\n ", node.funcs[i].salario);
        }
    }
    printf("] ");
}

void bt_split_child(btNode x, int pos, bTree *tree, FILE *fp, int split_root) {

    btNode y = disk_read(x.filhos[pos], tree->order, fp);
    if (split_root == 1) {
        tree->node_count++;
        y.pos_in_disk = tree->node_count;
    }
    btNode z = new_node(tree->order, y.flag_folha);
    tree->node_count++;
    z.pos_in_disk = tree->node_count;
    int t = (tree->order / 2);

    if (tree->order % 2 == 0) {
        t--;
    }
    z.num_funcs = t;

    if (tree->order % 2 != 0) {
        t--;
    }
    for (int j = 0; j <= t && (j + t + 1) <= y.num_funcs - 1; j++) {
        z.funcs[j] = y.funcs[j + t + 1];
        y.funcs[j + t + 1].codigo = -1;
        y.funcs[j + t + 1].salario = -1;
    }

    if (y.flag_folha == 0) {
        for (int j = 0; j <= t; j++) {
            z.filhos[j] = y.filhos[j + t + 1];
            y.filhos[j + t + 1] = -1;
        }
    }
    y.num_funcs = t;


    if (split_root == 1) {
        x.filhos[pos] = y.pos_in_disk;
        x.filhos[pos + 1] = z.pos_in_disk;
    } else {
        int j, i, r;
        for (j = 0; j < tree->order; j++) {
            if (x.filhos[j] == y.pos_in_disk) {
                for (i = j + 1; i < tree->order; i += 2) {
                    if (i + 1 < tree->order)
                        x.filhos[i + 1] = x.filhos[i];
                }
                r = j;
            }
        }
        x.filhos[r + 1] = z.pos_in_disk;
    }


    for (int j = pos; j < tree->order - 2; j += 2) {
        x.funcs[j + 1] = x.funcs[j];
    }

    x.funcs[pos] = y.funcs[t];
    y.funcs[t].codigo = -1;
    y.funcs[t].salario = -1;
    x.num_funcs++;

    disk_write(x, tree->order, fp);
    disk_write(y, tree->order, fp);
    disk_write(z, tree->order, fp);
}

btNode bt_insert_nonfull(btNode node, element key, bTree *tree, FILE *fp) {

    int pos = node.num_funcs;

    if (node.flag_folha == 1) {
        int i = pos - 1;
        while (i >= 0 && key.codigo < node.funcs[i].codigo) {
            node.funcs[i + 1] = node.funcs[i];
            node.funcs[i].codigo = -1;
            node.funcs[i].salario = -1;
            i--;
        }
        if (i + 1 != pos) {
            node.funcs[i + 1] = key;
        } else {
            node.funcs[pos] = key;
        }
        node.num_funcs++;
        disk_write(node, tree->order, fp);
        return node;
    } else {
        int n_pd = node.pos_in_disk;
        int i = pos - 1;
        while (key.codigo < node.funcs[i].codigo && i >= 0) {
            i--;
            pos--;
        }

        btNode x = disk_read(node.filhos[pos], tree->order, fp);
        if (x.num_funcs == tree->order - 1) {
            bt_split_child(node, pos, tree, fp, 0);
            btNode x1 = disk_read(n_pd, tree->order, fp);
            if (key.codigo > x1.funcs[pos].codigo)
                pos++;
        }
        btNode x1 = disk_read(n_pd, tree->order, fp);
        btNode x2 = disk_read(x1.filhos[pos], tree->order, fp);
        bt_insert_nonfull(x2, key, tree, fp);
    }
}


element bt_delete_max(btNode node, int order, FILE *fp) {
    if (node.flag_folha == 1) {
        node.funcs[node.num_funcs - 1].codigo = -1;
        node.funcs[node.num_funcs - 1].salario = -1;
        node.num_funcs--;
        disk_write(node, order, fp);
        return node.funcs[node.num_funcs - 1];
    } else {
        btNode x = disk_read(node.filhos[node.num_funcs], order, fp);
        bt_delete_max(x, order, fp);
    }
}

element bt_delete_min(btNode node, int order, FILE *fp) {
    if (node.flag_folha == 1) {
        element x = node.funcs[0];
        for (int j = 0; j < node.num_funcs; j++)
            node.funcs[j] = node.funcs[j + 1];
        node.num_funcs--;
        disk_write(node, order, fp);
        return x;
    } else {

        btNode x = disk_read(node.filhos[0], order, fp);
        bt_delete_min(x, order, fp);
    }
}

void bt_merge_children(btNode node, int pos, int order, FILE *fp) {
    int t = (order / 2);

    btNode y = disk_read(node.filhos[pos], order, fp);
    btNode z = disk_read(node.filhos[pos + 1], order, fp);

    y.funcs[t - 1] = node.funcs[pos];
    node.funcs[pos].codigo = -1;
    node.funcs[pos].salario = -1;
    for (int j = 0; j < t - 1; j++) {
        y.funcs[t + j] = z.funcs[j];
    }
    if (y.flag_folha == 0) {
        for (int j = 0; j < t; j++) {
            y.filhos[t + j] = z.filhos[j];
        }
    }
    y.num_funcs = order - 1;
    for (int j = pos + 1; j < node.num_funcs; j++) {
        node.funcs[j - 1] = node.funcs[j];
    }
    for (int j = pos + 2; j < node.num_funcs + 1; j++) {
        node.filhos[j - 1] = node.filhos[j];
    }
    node.num_funcs--;
    //free(z);
    disk_write(y, order, fp);
    disk_write(node, order, fp);
}

void bt_borrow_from_left_sibling(btNode node, int pos, int order, FILE *fp) {
    int t = (order / 2);
    btNode y = disk_read(node.filhos[pos], order, fp);
    btNode z = disk_read(node.filhos[pos - 1], order, fp);

    for (int j = t - 1; j > 0; j--) {
        y.funcs[j] = y.funcs[j - 1];
    }

    y.funcs[0] = node.funcs[pos - 1];
    node.funcs[pos - 1] = z.funcs[z.num_funcs - 1];

    z.funcs[z.num_funcs - 1].codigo = -1;
    z.funcs[z.num_funcs - 1].salario = -1;

    if (y.flag_folha == 0) {
        for (int j = t; j > 1; j--) {
            y.filhos[j + 1] = y.filhos[j];
        }
        y.filhos[1] = z.filhos[z.num_funcs + 1];
    }
    y.num_funcs = t;
    z.num_funcs--;
    disk_write(z, order, fp);
    disk_write(y, order, fp);
    disk_write(node, order, fp);
}

void bt_borrow_from_right_sibling(btNode node, int pos, int order, FILE *fp) {
    int t = (order / 2);
    btNode y = disk_read(node.filhos[pos], order, fp);
    btNode z = disk_read(node.filhos[pos + 1], order, fp);

    y.funcs[y.num_funcs] = node.funcs[pos];
    node.funcs[pos] = z.funcs[0];

    for (int j = 0; j < z.num_funcs; j++) {
        if (j + 1 == z.num_funcs) {
            z.funcs[j].codigo = -1;
            z.funcs[j].salario = -1;
        } else
            z.funcs[j] = z.funcs[j + 1];
    }


    if (y.flag_folha == 0) {
        for (int j = t; j > 1; j--) {
            y.filhos[j + 1] = y.filhos[j];
        }
        y.filhos[1] = z.filhos[z.num_funcs + 1];
    }
    y.num_funcs = t;
    z.num_funcs--;
    disk_write(z, order, fp);
    disk_write(y, order, fp);
    disk_write(node, order, fp);
}

void bt_delete_safe(btNode node, element key, int order, FILE *fp) {
    int t = (order / 2);
    int borrowed;
    int pos = 0;
    while (pos <= node.num_funcs - 1 && key.codigo > node.funcs[pos].codigo)
        pos++;
    if (pos <= node.num_funcs && key.codigo == node.funcs[pos].codigo) {
        if (node.flag_folha == 1) {
            for (int j = pos; j < node.num_funcs; j++)
                node.funcs[j] = node.funcs[j + 1];
            if (pos == node.num_funcs - 1) {
                node.funcs[pos].codigo = -1;
                node.funcs[pos].salario = -1;
            }
            node.num_funcs--;
            disk_write(node, order, fp);
        } else {
            btNode y = disk_read(node.filhos[pos], order, fp);
            if (y.num_funcs > t - 1) {
                node.funcs[pos] = bt_delete_max(y, order, fp);
                disk_write(node, order, fp);
            } else {
                btNode z = disk_read(node.filhos[pos + 1], order, fp);
                if (z.num_funcs > t - 1) {
                    node.funcs[pos] = bt_delete_min(z, order, fp);
                    disk_write(node, order, fp);
                } else {
                    bt_merge_children(node, pos, order, fp);
                    btNode node_child = disk_read(node.filhos[pos], order, fp);
                    bt_delete_safe(node_child, key, order, fp);
                }
            }
        }
    } else if (node.flag_folha == 0) {
        int m = pos; //default
        btNode y = disk_read(node.filhos[pos], order, fp);
        if (y.num_funcs == t - 1) {
            borrowed = 0;
            if (pos > 0) {
                btNode z = disk_read(node.filhos[pos - 1], order, fp);
                if (z.num_funcs > t - 1) {
                    bt_borrow_from_left_sibling(node, pos, order, fp);
                    borrowed = 1;
                } else {
                    m = pos - 1;
                }
            }
            if (borrowed == 0 && pos <= node.num_funcs && node.filhos[pos + 1] != -1) {
                btNode z = disk_read(node.filhos[pos + 1], order, fp);
                if (z.num_funcs > t - 1) {
                    bt_borrow_from_right_sibling(node, pos, order, fp);
                    borrowed = 1;
                } else {
                    m = pos;
                }
            }
            if (borrowed == 0) {
                bt_merge_children(node, m, order, fp);
                btNode x = disk_read(node.filhos[m], order, fp);
                y = x;
            }
        }
        if (m != pos) {
            bt_delete_safe(y, key, order, fp);
        } else {
            btNode new_y = disk_read(node.filhos[pos], order, fp);
            bt_delete_safe(new_y, key, order, fp);
        }
    }
}


bTree *btCreate(int order) {

    bTree *tree;
    if ((tree = malloc(sizeof(bTree))) == NULL)
        return NULL;

    btNode root = new_node(order, true);
    root.pos_in_disk = 0;

    tree->order = order;
    tree->root = root;
    tree->node_count = 0;

    return tree;

}

void btInsert(bTree *tree, element key, FILE *fp) {
    if (tree->node_count > 0)
        tree->root = disk_read(0, tree->order, fp);
    btNode root = tree->root;

    if (root.num_funcs == tree->order - 1) {
        btNode s = new_node(tree->order, 0);
        s.filhos[0] = root.pos_in_disk;
        bt_split_child(s, 0, tree, fp, 1);
        s = disk_read(0, tree->order, fp);
        tree->root = s;
        bt_insert_nonfull(s, key, tree, fp);
    } else {
        tree->root = bt_insert_nonfull(root, key, tree, fp);
    }

}

int btSearch(btNode node, int order, element key, FILE *fp) {

    int pos = 0;
    while (pos < node.num_funcs && key.codigo > node.funcs[pos].codigo) {
        pos++;
    }
    if (pos <= node.num_funcs && key.codigo == node.funcs[pos].codigo) {
        return node.pos_in_disk;
    } else if (node.flag_folha == 1) {
        return -1;
    } else {
        btNode x = disk_read(node.filhos[pos], order, fp);
        return btSearch(x, order, key, fp);
    }
}

int btDelete(bTree *tree, element key, FILE *fp) {

    btNode root = tree->root;
    bt_delete_safe(root, key, tree->order, fp);
    btNode new_root = disk_read(0, tree->order, fp);
    if (new_root.num_funcs == 0 && (new_root.flag_folha == 0)) {
        btNode x = disk_read(new_root.filhos[0], tree->order, fp);
        x.pos_in_disk = 0;
        disk_write(x, tree->order, fp);
        tree->root = x;
    } else {
        tree->root = new_root;
    }
    return 1;
}

element btfindMax(btNode node, int order, FILE *fp) {
    if (node.flag_folha == 1) {
        return node.funcs[node.num_funcs - 1];
    } else {
        btNode x = disk_read(node.filhos[node.num_funcs], order, fp);
        btfindMax(x, order, fp);
    }
}

element btfindMin(btNode node, int order, FILE *fp) {
    if (node.flag_folha == 1) {
        return node.funcs[0];
    } else {
        btNode x = disk_read(node.filhos[0], order, fp);
        btfindMin(x, order, fp);
    }
}

void btPrintTree(bTree *tree, queue *q, FILE *fp) {
    if (tree->root.num_funcs == 0) {
        printf("\nThe B-Tree is empty\n");
    } else {
        btNode end = {.num_funcs = -1};
        insert(q, tree->root);
        int item_count = 1;
        while (!isEmpty(q)) {
            btNode current = removeData(q);
            if (current.num_funcs == -1) {
                printf("\n");
                insert(q, end);
                if (item_count == 0)
                    break;
            } else {
                item_count--;
                print_node_keys(current, tree->order);
                if (current.pos_in_disk == 0)
                    insert(q, end);
                for (int i = 0; i < tree->order; i++) {
                    if (current.filhos[i] != -1) {
                        btNode x = disk_read(current.filhos[i], tree->order, fp);
                        insert(q, x);
                        item_count++;
                    }
                }
            }
        }
    }
}

void btDestroy(bTree *tree, FILE *fp) {
    free(tree);
    fclose(fp);
    if (remove("file.dat") == 0)
        printf("\nFile deleted successfully\n");
    else
        printf("\nError: unable to delete the file\n");
}

void btPrintElement(bTree tree, element key, FILE *fp) {
    btNode node = btSearchNode(tree.root, tree.order, key, fp);

    if (node.pos_in_disk == -1) {
        printf("Funcionario nao encontrado.\n");
        return;
    }

    int i = 0;
    while (i < node.num_funcs && node.funcs[i].codigo < key.codigo) {
        i++;
    }

    if (node.funcs[i].codigo == key.codigo) {
        printf("|| Funcionario encontrado:\n|| Nome: Fulano\n|| Codigo = %d\n|| Salario = %.2f\n", node.funcs[i].codigo,
               node.funcs[i].salario);
    } else {
        printf("Funcionario nao encontrado.\n");
    }
}

btNode btSearchNode(btNode node, int order, element key, FILE *fp) {
    int pos = 0;
    while (pos < node.num_funcs && key.codigo > node.funcs[pos].codigo) {
        pos++;
    }

    if (pos <= node.num_funcs && key.codigo == node.funcs[pos].codigo) {
        return node;
    } else if (node.flag_folha == 1) {
        btNode null_node = {0};
        null_node.pos_in_disk = -1;
        return null_node;
    } else {
        btNode x = disk_read(node.filhos[pos], order, fp);
        return btSearchNode(x, order, key, fp);
    }
}
