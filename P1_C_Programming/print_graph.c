#include <stdio.h>
#include <math.h>
#include <string.h>

void print_graph(int *data,int len, int max_height)
{
        int level;
        int max = data[0];
        int min = data[0];
        for( int i = 0 ; i < len; i++)  //find max and min
        {
            if(data[i]>max)
            {
                max = data[i];
            }
            if(data[i]<=min)
            {
                min = data[i];
            }
        }
        /////////////////////////
        //calculates and print out length-min-max-range-maxheight_units/height
        int range = max - min ;
        float units_per_height = (float)range / (float)max_height;
        printf("length: %d\n",len);
        printf("min: %d\n",min);
        printf("max: %d\n",max);
        printf("range: %d\n", range);
        printf("max_height: %d\n", max_height);
        printf("units_per_height: %.2f\n", units_per_height);
        /////////////////////////
        //print top axis
        printf("     ");
        for( int i = 0; i < len; i++){
             if(i % 5 != 0){
                  printf("-");
              }
              else{
                  printf("+");
              }
         }
         printf("\n");
         ////////////////////////
         // print main graph body
        while(max_height >=0)
        {
            level = min + max_height * units_per_height;
            printf("%3d |",level);
            for (int index = 0 ; index < len; index++)
            {
                if( data[index] >= level)
                {
                    printf("X");
                }
                else
                {
                    printf(" ");
                }
            }
            max_height --;
            printf("\n");
        }
        //////////////////////////
        //bottom axis
        printf("     ");
        for ( int i =0; i < len ; i++)
        {
            if (i % 5 != 0)
            {
                printf("-");
            }
            else
            {
                printf("+");
            }
        }
        // for loop to print the bottom number each 5 numbers
        printf("\n     ");
        for ( int j = 0 ; j < len ; j++)
        {
            if( j % 5 ==0)
            {
                printf("%-5d",j);
            }
        }
        /////////////////////////
}
