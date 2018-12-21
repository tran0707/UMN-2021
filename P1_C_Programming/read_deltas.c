#include <stdio.h>
#include <stdlib.h>
#include "deltas.h"
#include <sys/stat.h>

//////////////READ TEXT//////////////
int *read_text_deltas(char *fname, int *len)
{
    int store;
    int scan_file;
    FILE *text1 = fopen(fname, "r");
    if(text1 == NULL)               // Can't open
    {
        *len = -1;
        return NULL;
    }
    *len = -1;
    while(1){                       // loop to count the number of elemets
        scan_file = fscanf(text1,"%d", &store);
        (*len) = (*len) +1;
        if(scan_file == EOF) {      //scan utin the end of file 
            break;		            // jump out while loop
	}
    }

    if(*len == 0){                  //No number in file
        *len  = -1;
        return NULL;
    }
    int *new_array = malloc(sizeof(int)*(*len));
    rewind(text1);
    for ( int i = 0; i<(*len); i++)
    {
        if(i==0){
	        fscanf(text1,"%d", &store);
            new_array[0]= store;
        }
        else if ( i > 0 )
        {
 	        fscanf(text1,"%d", &store);
            new_array[i] = store + new_array[i-1] ;
        }
    }
    fclose(text1);
    return new_array;

}
///////////////READ  BINARY INT////////////////////////
int *read_int_deltas(char *fname, int *len)
{
    int store;
    FILE *text2 = fopen(fname,"r");            
    if (text2 == NULL)             //File can't open
    {
        *len = -1;
        return NULL;
    }
    struct stat sb;
    int result = stat(fname,&sb);
    if (result == -1 || sb.st_size < sizeof(int))
    {                              // No integer
        *len = -1;
        return NULL;
    }
    int total_byte = sb.st_size;
    int *int_deltas_array = malloc(total_byte);
    int where = (*len) = total_byte/sizeof(int);
    for ( int j =0; j<where;j++)
    {
         if(j == 0){
	        fread(&store, sizeof(int),1,text2);
            int_deltas_array[0]= store;
         }
         else
         {
	        fread(&store, sizeof(int),1,text2);
	        if(j>0)
                int_deltas_array[j] = store + int_deltas_array[j-1];
         }
    }
    fclose(text2);
    return int_deltas_array;
}
