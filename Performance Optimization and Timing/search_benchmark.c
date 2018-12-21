#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "search.h"
#include <error.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    if (argc < 4){
        printf("usage: %s <minpow> <maxpow> <repeat> [which]\n"
               "a : Linear Array Search\n"
               "l : Linear List Search\n"
               "b : Binary Array Search\n"
               "t : Binary Tree Seatch\n"
               "(default all)\n" , argv[0]);
        return 1;
    }
    if (argc > 5){                          //More then 5 argument.
        printf("\n");   
        printf("%s is not a correct input argument!!!\n"
               "Try ./search_benchmark for more intructions.", argv[4]);   
        printf("\n");   
        return 1;                           // end benchmark 
    }
    ///////// Argunment Inputs ///////////
    int minpow = atoi(argv[1]);
    int maxpow = atoi(argv[2]);
    int repeat = atoi(argv[3]);

    ////////// Declare Variables /////////
    int *array_arr;  
    list_t *array_list;
    bst_t *array_tree;
    int len_timing = 0;                         // For example, a array from 1 to 100
    int smallNum = 0;                           // smallNum is 1 2 3 4 ....59 
    int largeNum = 0;                           // largeNum is 50.51.52 .... 99
    double cpu_time_1 = 0;                      // same thing with negative number
    double cpu_time_2 = 0;
    double cpu_time_3 = 0;
    double cpu_time_4 = 0;
    clock_t begin,end;
    /////// PRINT LENGTH,SEARCHES,and Which ALGORiTHMS ///////
        printf("%6s ", "LENGTH");
        printf("%10s ", "SEARCHES");
        int len_input = 0;
        char *name_search = "";
        char temp;

        if(argc == 4){
            name_search = "ablt";
        }
        else{
            name_search = argv[4];
        }
        len_input = strlen(name_search);
      /////////Which algorithms? Also Checking isn't correct input/////////
      for(int index = 0; index < len_input; index ++){
            temp = name_search[index];
            if(temp == 'a'){
                 printf("%13s ", "array");
            }
            else if(temp == 'l'){
                 printf("%13s ", "list");
            }
            else if(temp == 'b'){
                 printf("%13s ", "binary");
            }
            else if (temp == 't'){
                 printf("%13s ", "tree");
            }
            else{                           // wrong input string
                 printf("\n");   
                 printf("%s is not a correct input argument!!!\n"
                        "Try ./search_benchmark for more intructions.", argv[4]); 
                        
                 printf("\n");   
                 return 1;                  // end benchmark 
            }
            
        }
        printf("\n");


//////////////// TIMING FUNCTIONS /////////////////
    ////////// linear_array_search ///////////
    void linear_array_search_timing (int query){
        begin = clock();
        linear_array_search (array_arr, len_timing , query );
        end = clock();
        cpu_time_1 += ((double) (end - begin)) / CLOCKS_PER_SEC;
    }
    /////////// linkedlist_search //////////////
    void linkedlist_search_timing (int query){
        begin = clock();
        linkedlist_search (array_list, len_timing , query);
        end = clock();
        cpu_time_2 += ((double) (end - begin)) / CLOCKS_PER_SEC;
    }
    /////////// binary_array_search //////////////
    void binary_array_search_timing (int query) {
        begin = clock();
        binary_array_search (array_arr, len_timing , query);
        end = clock();
        cpu_time_3 += ((double) (end - begin)) / CLOCKS_PER_SEC;
    }
    /////////// binary_tree_search ///////////////
    void binary_tree_search_timing (int query) { 
        begin = clock();
        binary_tree_search (array_tree, len_timing , query);
        end = clock();
        cpu_time_4 += ((double) (end - begin)) / CLOCKS_PER_SEC;
    }
///////////////////////// TIMING LOOP ///////////////////////
// This work by running both success queries and failing query for odd and
// even cases. For example: 0,50,1,51,2,52,3,53. 0 will be the odd run
// and 50 will the even run.....
    for(int i= minpow; i<=maxpow; i++)
    {
        len_timing = (1 << i);
        array_arr = make_sequential_array (len_timing);  
        array_list = make_sequential_list (len_timing);
        array_tree = make_sequential_tree (len_timing);
        int numSearch = 0; 
        cpu_time_1 = 0;                         // reset time for array
        cpu_time_2 = 0;                         // reset time for list
        cpu_time_3 = 0;                         // reset time for binary
        cpu_time_4 = 0;                         // reset time for tree
        for (int numrep = 0; numrep < repeat; numrep++)
        {
            smallNum = 0;
            largeNum = len_timing/2;
            int runInd = 0;                            // odd or even runs
            while(largeNum<len_timing)                 // successful case
            {
                if(runInd % 2 != 0)                    //odd run
                {
                    linear_array_search_timing(largeNum);
                    linkedlist_search_timing(largeNum);
                    binary_array_search_timing(largeNum);
                    binary_tree_search_timing(largeNum);
                    largeNum++;
                }
                else                                   // even run
                {
                    linear_array_search_timing(smallNum);
                    linkedlist_search_timing(smallNum);
                    binary_array_search_timing(smallNum);
                    binary_tree_search_timing(smallNum);
                    smallNum++;
                }
                runInd++;
                numSearch++ ;
            }
            runInd   = 0;
            smallNum = -1;
            largeNum = (-len_timing/2) -1 ;
            while(largeNum >= 0-len_timing)            // unsuccessful case
            {
                if(runInd % 2 != 0)                    //odd run
                {
                    linear_array_search_timing(largeNum);
                    linkedlist_search_timing(largeNum);
                    binary_array_search_timing(largeNum);
                    binary_tree_search_timing(largeNum);
                    largeNum--;
                }
                else                                    // even run
                {
                    linear_array_search_timing(smallNum);
                    linkedlist_search_timing(smallNum);
                    binary_array_search_timing(smallNum);
                    binary_tree_search_timing(smallNum);
                    smallNum--;
                }
                runInd++;
                numSearch++;
            }
         }
  /////// FREE ARRAYY ////////
  free(array_arr);
  list_free(array_list);
  bst_free(array_tree);
    ////////////// PRINT TIMING NUMBER ///////////////
        printf("%5d ",len_timing);
        printf("%8d",numSearch);    
        for(int index = 0; index < len_input; index ++){
            temp = name_search[index];
            if(temp == 'a'){
                printf("      %10.4e", cpu_time_1);
            }
            else if(temp == 'l'){
                printf("      %10.4e", cpu_time_2);
             }
            else if(temp == 'b'){
                printf("      %10.4e", cpu_time_3);
            }
            else if (temp == 't'){
                printf("      %10.4e", cpu_time_4);
            }
        }
        printf("\n");
   }
  return 0;
}
