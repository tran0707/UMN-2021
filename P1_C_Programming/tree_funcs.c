#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
//Some code is inspired from the lab 2
int compare = -1;
void bst_init(bst_t *tree){
    tree->root = NULL;
    tree->size = 0;
    return;
}
int bst_insert(bst_t *tree, char name[]){
    node_t *result = node_insert(tree->root, name);
    if(result == NULL){
        return 0;
    }
    else{
        tree->root = result;
        tree->size++;
        return 1;
    }
}
node_t *node_insert(node_t *cur, char name[]){
    if(cur == NULL){
        cur = malloc(sizeof(node_t));
        strcpy(cur->name, name);
        cur->left = NULL;
        cur->right = NULL;
        return cur;
    }
    else if(cur != NULL){
        compare = strcmp(name, cur->name);
        if(compare == 0){
            return NULL;
        }
        else if(compare != 0){
            if (compare <0){
                node_t *output1 = node_insert(cur->left,name);
                if(output1 == NULL){
                    return NULL;
                }
                else{
                    cur->left = output1;
                    return cur;
                }
            }
            else if(compare > 0){
                node_t *output2 = node_insert(cur->right,name);
                if(output2 == NULL){
                    return NULL;
                }
                else{
                    cur->right = output2;
                    return cur;
                }
            }
        }
    }
    return cur;
}
int bst_find(bst_t *tree, char name[]){
    int output = node_find(tree->root,name);
    if(output == 1){
        return 1;
    }
    else{
        return 0;
    }
}
int node_find(node_t *cur,char name[]){
    int save;
    if(cur==NULL){
        return 0;
    }
    compare = strcmp(name,cur->name);
    if(compare < 0){
        save = node_find(cur->left,name);
    }
    else if(compare > 0){
        save = node_find(cur->right,name);
    }
    else if(compare == 0){
        save = 1;
    }
    return save;
}
void bst_clear(bst_t *tree){
    node_remove_all(tree->root);
    tree->size = 0;
    tree->root = NULL;
    return;
}
void node_remove_all(node_t *cur){
    if(cur == NULL)
        return;
    node_remove_all(cur->left);
    node_remove_all(cur->right);
    free(cur);
}
void bst_print_inorder(bst_t *tree){
    node_print_inorder(tree->root);
}
void node_print_inorder(node_t *cur){
    if(cur)
    {
        node_print_inorder(cur->left);
        printf("%s\n", cur->name);
        node_print_inorder(cur->right);
    }
}
void bst_print_preorder(bst_t *tree){
    node_write_preorder(tree->root, stdout, 0);
}
void bst_save(bst_t *tree,char *fname){
    FILE *fl = fopen(fname,"w");
    if(fl == NULL){
        return;
    }
    else{
        node_write_preorder(tree->root, fl, 0);
    }
    fclose(fl);
}
void node_write_preorder(node_t *cur, FILE *out, int depth){
    //this will determin how many indent needed to print out
    for( int index = 1; index <= depth; index ++){
        fprintf(out,"%c",' ');
    }
    if(cur == NULL){
        return;
    }
        fprintf(out, "%s\n", cur->name);
        if(cur->left != NULL){
        node_write_preorder(cur->left, out, depth + 1);
        }
        if(cur->right != NULL){
        node_write_preorder(cur->right, out, depth + 1);
        }
}
int bst_load(bst_t *tree, char *fname){
    bst_clear(tree);
    FILE *fls = fopen(fname,"r");
    int num;
    if(fls == NULL){
        return 0;
    }
    while(1){
        num = fscanf(fls, "%s", fname);
        if(num == EOF){
            break;
        }
        bst_insert(tree,fname);
    }
    fclose(fls);
    return 1;
}
