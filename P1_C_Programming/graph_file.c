#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deltas.h"
//declare function
void print_graph(int *data, int data_len, int max_height);

int main(int argc , char *argv[]){
    if(argc < 3){
        printf("usage: %s <format> <filename>\n",argv[0]);
        printf(" <format> is one of\n");
        printf(" text : text ints are in the given filename\n");
        printf(" int  : binary ints are in the given filename\n");
        return 1;
    }
    char *format = argv[1];
    char *fname = argv[2];
    char *height = argv[3];
    int data_len = -1;
    int max_height = atoi(height);
    int *data_vals = NULL;
    //////////////Text Format///////////////
    if(strcmp("text",format)==0){
        printf("Reading text format\n");
        data_vals = read_text_deltas(fname,&data_len);
    }
    ///////////////Int Format///////////////
    else if( strcmp("int",format)==0){
        printf("Reading binary int format\n");
        data_vals = read_int_deltas(fname,&data_len);
    }
    ///////////////Unknown Format///////////////
    else{
        printf("Unknown format '%s'\n", format);
        return 1;
    }
    ///////////calling fuction to print and free///////////////
    print_graph(data_vals, data_len, max_height);
    free(data_vals);
    return 0;
}
