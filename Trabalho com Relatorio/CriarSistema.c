#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include <conio.h>
#include<time.h>//necessário p/ função time()
// #include "timer.h"
long int size_linha, size_coluna, size_linha1, size_coluna1;
double *A, *B;
FILE *p;
int main(int argc, char* argv[]){

    if(argc<2) {
      printf("Digite: %s <quantidade de equações>\n", argv[0]);
      return 1;
   }
   size_linha = atoi(argv[1]);
   size_coluna=size_linha+1;
    srand(time(NULL));
   p = fopen("Sistema" , "wb");
   A = (double *) malloc(sizeof(double) * size_linha * size_coluna);
   if (A == NULL) {printf("ERRO--malloc1\n"); return 2;}
     fwrite(&size_linha, sizeof(long int), 1, p);
     fwrite(&size_coluna, sizeof(long int), 1, p);
    for(long long int i=0; i<size_linha; i++){
        for(long long int j=0; j<size_coluna; j++){
            A[i*size_coluna +j]=(rand())%100;   
            printf("%f ", A[i*size_coluna +j]);          
        }    
        printf("\n");
    }
    fwrite(A, sizeof(double), size_coluna*size_linha, p);
    
    fclose(p);
    free(A);

    return 0;
}