#include <stdio.h>
#include "tree.h"
#include <string.h>

//This is inspired from the lab 2 main function
// global variable to control echoing, 0: echo off, 1: echo on
int ECHO = 0;
// Print the given string if echoing is turned on
void echo(char *s){
  if(ECHO){
    printf("%s",s);
  }
}

int main(int argc, char *argv[]){
  if(argc > 1 && strcmp("-echo",argv[1])==0) {
      ECHO=1;
  }
  printf("BST Demo\n");
  printf("Commands:\n");
  printf("  print:          shows contents of the tree in sorted order\n");
  printf("  show:           shows contents in pre-order indicating its structure\n");
  printf("  clear:          eliminates all elements from the tree\n");
  printf("  quit:           exit the program\n");
  printf("  add <name>:     inserts the given string into the tree, duplicates ignored\n");
  printf("  find <name>:    prints FOUND if the name is in the tree, NOT FOUND otherwise\n");
  printf("  save <file>:    writes the contents of the tree in post order to the given file\n");
  printf("  load <file>:    clears the current tree and loads the one in the given file\n");

  char cmd[128];
  int success;
  bst_t tree;
  bst_init(&tree);
  while(1){
        printf("BST>");
        success = fscanf(stdin,"%s",cmd);
        if(success == EOF){
            printf("\n");
            break;
        }
        else{
            echo(" "); echo(cmd);
        }

        if(strcmp("quit",cmd)==0){
            printf("\n");
            break;
        }

        else if(strcmp("print", cmd) == 0){
            echo("\n");
            bst_print_inorder(&tree);
        }
        else if(strcmp("show", cmd )== 0){
             echo("\n");
            bst_print_preorder(&tree);
        }
        else if(strcmp("clear",cmd)==0){
            bst_clear(&tree);
             echo("\n");

        }
        else if(strcmp("add",cmd)==0){
            fscanf(stdin,"%s",cmd);
            echo(" "); echo(cmd);echo("\n");
            success = bst_insert(&tree, cmd);
            if(!success){
              printf("add fail\n");
            }
        }
        else if(strcmp("find",cmd)==0){
            fscanf(stdin,"%s",cmd);
            echo(" "); echo(cmd); echo("\n");
            success = bst_find(&tree, cmd);
            if(success == 1){
                printf("FOUND\n");
            }
            else{
                printf("NOT FOUND\n");
            }
        }
        else if(strcmp("save",cmd)==0){
            fscanf(stdin,"%s",cmd);
            echo(" "); echo(cmd); echo("\n");
            bst_save(&tree, cmd);
        }
        else if(strcmp("load",cmd)==0){
            fscanf(stdin,"%s",cmd);
            echo(" "); echo(cmd); echo("\n");
            bst_load(&tree,cmd);
        }
        else{
            printf("unkown command %s\n",cmd);
        }
      }
    bst_clear(&tree);
    return 0;
}
