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
    fread(&read_node.numKeys, sizeof(read_node.numKeys), 1, fp);
    fread(&read_node.isLeaf, sizeof(read_node.isLeaf), 1, fp);
    fread(&read_node.pos_in_disk, sizeof(read_node.pos_in_disk), 1, fp);

    read_node.keys = malloc(sizeof(element) * order - 1);
    fread(read_node.keys, sizeof(element), order - 1, fp);

    read_node.kids = malloc(sizeof(int) * order);
    fread(read_node.kids, sizeof(int), order, fp);


    return read_node;
}

void disk_write(btNode node, int order, FILE *fp) {

    int offset = calculate_offset(node.pos_in_disk, order);
    fseek(fp, offset, SEEK_SET);

    // Escrevendo no arquivo
    fwrite(&node.numKeys, sizeof(node.numKeys), 1, fp);
    fwrite(&node.isLeaf, sizeof(node.isLeaf), 1, fp);
    fwrite(&node.pos_in_disk, sizeof(node.pos_in_disk), 1, fp);
    fwrite(node.keys, sizeof(element), order - 1, fp);
    fwrite(node.kids, sizeof(int), order, fp);
}

btNode new_node(int order, int is_leaf) {
    btNode n;

    n.numKeys = 0;
    n.isLeaf = is_leaf;

    n.keys = malloc((order - 1) * sizeof(element));
    for (int i = 0; i < order - 1; i++) {
        n.keys[i].codigo = -1;
        n.keys[i].salario = -1;
    }

    n.kids = malloc((order) * sizeof(int));
    for (int i = 0; i < order; i++) {
        n.kids[i] = -1;
    }

    return n;
}

void print_node_keys(btNode node, int order) {
    printf("\n[ %d", node.pos_in_disk);
    for (int i = 0; i < order - 1; i++) {
        if (node.keys[i].codigo != -1) {
            printf("\n  || Codigo: %d\n ", node.keys[i].codigo);
            printf(" || Nome: Fulano\n ");
            printf(" || Salario: R$ %.2f\n ", node.keys[i].salario);
        }
    }
    printf("] ");
}

void bt_split_child(btNode x, int pos, bTree *tree, FILE *fp, int split_root) {

    btNode y = disk_read(x.kids[pos], tree->order, fp);
    if (split_root == 1) {
        tree->node_count++;
        y.pos_in_disk = tree->node_count;
    }
    btNode z = new_node(tree->order, y.isLeaf);
    tree->node_count++;
    z.pos_in_disk = tree->node_count;
    int t = (tree->order / 2);

    if (tree->order % 2 == 0) {
        t--;
    }
    z.numKeys = t;

    if (tree->order % 2 != 0) {
        t--;
    }
    for (int j = 0; j <= t && (j + t + 1) <= y.numKeys - 1; j++) {
        z.keys[j] = y.keys[j + t + 1];
        y.keys[j + t + 1].codigo = -1;
        y.keys[j + t + 1].salario = -1;
    }

    if (y.isLeaf == 0) {
        for (int j = 0; j <= t; j++) {
            z.kids[j] = y.kids[j + t + 1];
            y.kids[j + t + 1] = -1;
        }
    }
    y.numKeys = t;


    if (split_root == 1) {
        x.kids[pos] = y.pos_in_disk;
        x.kids[pos + 1] = z.pos_in_disk;
    } else {
        int j, i, r;
        for (j = 0; j < tree->order; j++) {
            if (x.kids[j] == y.pos_in_disk) {
                for (i = j + 1; i < tree->order; i += 2) {
                    if (i + 1 < tree->order)
                        x.kids[i + 1] = x.kids[i];
                }
                r = j;
            }
        }
        x.kids[r + 1] = z.pos_in_disk;
    }


    for (int j = pos; j < tree->order - 2; j += 2) {
        x.keys[j + 1] = x.keys[j];
    }

    x.keys[pos] = y.keys[t];
    y.keys[t].codigo = -1;
    y.keys[t].salario = -1;
    x.numKeys++;

    disk_write(x, tree->order, fp);
    disk_write(y, tree->order, fp);
    disk_write(z, tree->order, fp);
}

btNode bt_insert_nonfull(btNode node, element key, bTree *tree, FILE *fp) {

    int pos = node.numKeys;

    if (node.isLeaf == 1) {
        int i = pos - 1;
        while (i >= 0 && key.codigo < node.keys[i].codigo) {
            node.keys[i + 1] = node.keys[i];
            node.keys[i].codigo = -1;
            node.keys[i].salario = -1;
            i--;
        }
        if (i + 1 != pos) {
            node.keys[i + 1] = key;
        } else {
            node.keys[pos] = key;
        }
        node.numKeys++;
        disk_write(node, tree->order, fp);
        return node;
    } else {
        int n_pd = node.pos_in_disk;
        int i = pos - 1;
        while (key.codigo < node.keys[i].codigo && i >= 0) {
            i--;
            pos--;
        }

        btNode x = disk_read(node.kids[pos], tree->order, fp);
        if (x.numKeys == tree->order - 1) {
            bt_split_child(node, pos, tree, fp, 0);
            btNode x1 = disk_read(n_pd, tree->order, fp);
            if (key.codigo > x1.keys[pos].codigo)
                pos++;
        }
        btNode x1 = disk_read(n_pd, tree->order, fp);
        btNode x2 = disk_read(x1.kids[pos], tree->order, fp);
        bt_insert_nonfull(x2, key, tree, fp);
    }
}


element bt_delete_max(btNode node, int order, FILE *fp) {
    if (node.isLeaf == 1) {
        node.keys[node.numKeys - 1].codigo = -1;
        node.keys[node.numKeys - 1].salario = -1;
        node.numKeys--;
        disk_write(node, order, fp);
        return node.keys[node.numKeys - 1];
    } else {
        btNode x = disk_read(node.kids[node.numKeys], order, fp);
        bt_delete_max(x, order, fp);
    }
}

element bt_delete_min(btNode node, int order, FILE *fp) {
    if (node.isLeaf == 1) {
        element x = node.keys[0];
        for (int j = 0; j < node.numKeys; j++)
            node.keys[j] = node.keys[j + 1];
        node.numKeys--;
        disk_write(node, order, fp);
        return x;
    } else {

        btNode x = disk_read(node.kids[0], order, fp);
        bt_delete_min(x, order, fp);
    }
}

void bt_merge_children(btNode node, int pos, int order, FILE *fp) {
    int t = (order / 2);

    btNode y = disk_read(node.kids[pos], order, fp);
    btNode z = disk_read(node.kids[pos + 1], order, fp);

    y.keys[t - 1] = node.keys[pos];
    node.keys[pos].codigo = -1;
    node.keys[pos].salario = -1;
    for (int j = 0; j < t - 1; j++) {
        y.keys[t + j] = z.keys[j];
    }
    if (y.isLeaf == 0) {
        for (int j = 0; j < t; j++) {
            y.kids[t + j] = z.kids[j];
        }
    }
    y.numKeys = order - 1;
    for (int j = pos + 1; j < node.numKeys; j++) {
        node.keys[j - 1] = node.keys[j];
    }
    for (int j = pos + 2; j < node.numKeys + 1; j++) {
        node.kids[j - 1] = node.kids[j];
    }
    node.numKeys--;
    //free(z);
    disk_write(y, order, fp);
    disk_write(node, order, fp);
}

void bt_borrow_from_left_sibling(btNode node, int pos, int order, FILE *fp) {
    int t = (order / 2);
    btNode y = disk_read(node.kids[pos], order, fp);
    btNode z = disk_read(node.kids[pos - 1], order, fp);

    for (int j = t - 1; j > 0; j--) {
        y.keys[j] = y.keys[j - 1];
    }

    y.keys[0] = node.keys[pos - 1];
    node.keys[pos - 1] = z.keys[z.numKeys - 1];

    z.keys[z.numKeys - 1].codigo = -1;
    z.keys[z.numKeys - 1].salario = -1;

    if (y.isLeaf == 0) {
        for (int j = t; j > 1; j--) {
            y.kids[j + 1] = y.kids[j];
        }
        y.kids[1] = z.kids[z.numKeys + 1];
    }
    y.numKeys = t;
    z.numKeys--;
    disk_write(z, order, fp);
    disk_write(y, order, fp);
    disk_write(node, order, fp);
}

void bt_borrow_from_right_sibling(btNode node, int pos, int order, FILE *fp) {
    int t = (order / 2);
    btNode y = disk_read(node.kids[pos], order, fp);
    btNode z = disk_read(node.kids[pos + 1], order, fp);

    y.keys[y.numKeys] = node.keys[pos];
    node.keys[pos] = z.keys[0];

    for (int j = 0; j < z.numKeys; j++) {
        if (j + 1 == z.numKeys) {
            z.keys[j].codigo = -1;
            z.keys[j].salario = -1;
        } else
            z.keys[j] = z.keys[j + 1];
    }


    if (y.isLeaf == 0) {
        for (int j = t; j > 1; j--) {
            y.kids[j + 1] = y.kids[j];
        }
        y.kids[1] = z.kids[z.numKeys + 1];
    }
    y.numKeys = t;
    z.numKeys--;
    disk_write(z, order, fp);
    disk_write(y, order, fp);
    disk_write(node, order, fp);
}

void bt_delete_safe(btNode node, element key, int order, FILE *fp) {
    int t = (order / 2);
    int borrowed;
    int pos = 0;
    while (pos <= node.numKeys - 1 && key.codigo > node.keys[pos].codigo)
        pos++;
    if (pos <= node.numKeys && key.codigo == node.keys[pos].codigo) {
        if (node.isLeaf == 1) {
            for (int j = pos; j < node.numKeys; j++)
                node.keys[j] = node.keys[j + 1];
            if (pos == node.numKeys - 1) {
                node.keys[pos].codigo = -1;
                node.keys[pos].salario = -1;
            }
            node.numKeys--;
            disk_write(node, order, fp);
        } else {
            btNode y = disk_read(node.kids[pos], order, fp);
            if (y.numKeys > t - 1) {
                node.keys[pos] = bt_delete_max(y, order, fp);
                disk_write(node, order, fp);
            } else {
                btNode z = disk_read(node.kids[pos + 1], order, fp);
                if (z.numKeys > t - 1) {
                    node.keys[pos] = bt_delete_min(z, order, fp);
                    disk_write(node, order, fp);
                } else {
                    bt_merge_children(node, pos, order, fp);
                    btNode node_child = disk_read(node.kids[pos], order, fp);
                    bt_delete_safe(node_child, key, order, fp);
                }
            }
        }
    } else if (node.isLeaf == 0) {
        int m = pos; //default
        btNode y = disk_read(node.kids[pos], order, fp);
        if (y.numKeys == t - 1) {
            borrowed = 0;
            if (pos > 0) {
                btNode z = disk_read(node.kids[pos - 1], order, fp);
                if (z.numKeys > t - 1) {
                    bt_borrow_from_left_sibling(node, pos, order, fp);
                    borrowed = 1;
                } else {
                    m = pos - 1;
                }
            }
            if (borrowed == 0 && pos <= node.numKeys && node.kids[pos + 1] != -1) {
                btNode z = disk_read(node.kids[pos + 1], order, fp);
                if (z.numKeys > t - 1) {
                    bt_borrow_from_right_sibling(node, pos, order, fp);
                    borrowed = 1;
                } else {
                    m = pos;
                }
            }
            if (borrowed == 0) {
                bt_merge_children(node, m, order, fp);
                btNode x = disk_read(node.kids[m], order, fp);
                y = x;
            }
        }
        if (m != pos) {
            bt_delete_safe(y, key, order, fp);
        } else {
            btNode new_y = disk_read(node.kids[pos], order, fp);
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

    if (root.numKeys == tree->order - 1) {
        btNode s = new_node(tree->order, 0);
        s.kids[0] = root.pos_in_disk;
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
    while (pos < node.numKeys && key.codigo > node.keys[pos].codigo) {
        pos++;
    }
    if (pos <= node.numKeys && key.codigo == node.keys[pos].codigo) {
        return node.pos_in_disk;
    } else if (node.isLeaf == 1) {
        return -1;
    } else {
        btNode x = disk_read(node.kids[pos], order, fp);
        return btSearch(x, order, key, fp);
    }
}

int btDelete(bTree *tree, element key, FILE *fp) {

    btNode root = tree->root;
    bt_delete_safe(root, key, tree->order, fp);
    btNode new_root = disk_read(0, tree->order, fp);
    if (new_root.numKeys == 0 && (new_root.isLeaf == 0)) {
        btNode x = disk_read(new_root.kids[0], tree->order, fp);
        x.pos_in_disk = 0;
        disk_write(x, tree->order, fp);
        tree->root = x;
    } else {
        tree->root = new_root;
    }
    return 1;
}

element btfindMax(btNode node, int order, FILE *fp) {
    if (node.isLeaf == 1) {
        return node.keys[node.numKeys - 1];
    } else {
        btNode x = disk_read(node.kids[node.numKeys], order, fp);
        btfindMax(x, order, fp);
    }
}

element btfindMin(btNode node, int order, FILE *fp) {
    if (node.isLeaf == 1) {
        return node.keys[0];
    } else {
        btNode x = disk_read(node.kids[0], order, fp);
        btfindMin(x, order, fp);
    }
}

void btPrintTree(bTree *tree, queue *q, FILE *fp) {
    if (tree->root.numKeys == 0) {
        printf("\nThe B-Tree is empty\n");
    } else {
        btNode end = {.numKeys = -1};
        insert(q, tree->root);
        int item_count = 1;
        while (!isEmpty(q)) {
            btNode current = removeData(q);
            if (current.numKeys == -1) {
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
                    if (current.kids[i] != -1) {
                        btNode x = disk_read(current.kids[i], tree->order, fp);
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
    while (i < node.numKeys && node.keys[i].codigo < key.codigo) {
        i++;
    }

    if (node.keys[i].codigo == key.codigo) {
        printf("|| Funcionario encontrado:\n|| Nome: Fulano\n|| Codigo = %d\n|| Salario = %.2f\n", node.keys[i].codigo,
               node.keys[i].salario);
    } else {
        printf("Funcionario nao encontrado.\n");
    }
}

btNode btSearchNode(btNode node, int order, element key, FILE *fp) {
    int pos = 0;
    while (pos < node.numKeys && key.codigo > node.keys[pos].codigo) {
        pos++;
    }

    if (pos <= node.numKeys && key.codigo == node.keys[pos].codigo) {
        return node;
    } else if (node.isLeaf == 1) {
        btNode null_node = {0};
        null_node.pos_in_disk = -1;
        return null_node;
    } else {
        btNode x = disk_read(node.kids[pos], order, fp);
        return btSearchNode(x, order, key, fp);
    }
}
