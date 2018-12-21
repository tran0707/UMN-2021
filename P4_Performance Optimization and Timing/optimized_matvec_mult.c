#include "matvec.h"
#include <stdlib.h>

int optimized_matrix_trans_mult_vec(matrix_t mat, vector_t vec, vector_t res){
  long vec_len = vec.len;
  long res_len = res.len;
  long numr = mat.rows;
  long numc = mat.cols;
  if(mat.rows != vec_len){
    printf("mat.rows (%ld) != vec.len (%ld)\n", numr ,vec_len);
    return 1;
  }
  if(mat.cols != res_len){
    printf("mat.cols (%ld) != res.len (%ld)\n",numc ,res_len);
    return 2;
  }
  for (int j = 0; j < res_len ; j++){
      VSET(res,j,0);
  }
  int i;
  for (i = 0; i < numr ; i+=4){
      // get element of vec using rows i //
      int veci0 = VGET(vec,i+0);
      int veci1 = VGET(vec,i+1);
      int veci2 = VGET(vec,i+2);
      int veci3 = VGET(vec,i+3);

  for (int j = 0; j < numc; j++){
      // first column element //
      int elij0 = MGET(mat,i+0,j);
      int prod0 = elij0 * veci0;
      // second column element //
      int elij1 = MGET(mat,i+1,j);
      int prod1 = elij1 * veci1;
      // third column element //
      int elij2 = MGET(mat,i+2,j);
      int prod2 = elij2 * veci2;
      // fourth column element //
      int elij3 = MGET(mat,i+3,j);
      int prod3 = elij3 * veci3;
      // addition and set to res // 
      int cur = VGET(res,j);
      int sum = cur + prod0 + prod1 + prod2 + prod3;
      VSET(res,j,sum);
  }
}
  for(; i< numr; i++){ //clean up loop for the remain elements
      int veci = VGET(vec,i);
      for(int j = 0; j < numc; j++){
          int elij = MGET(mat,i,j);
          int prod = elij * veci;
          int cur = VGET(vec,j);
          int sum = cur + prod;
          VSET(res,j,sum);
      }
  }
  return 0;
}
