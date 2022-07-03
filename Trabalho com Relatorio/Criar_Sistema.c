#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>

long int size_linha, size_coluna, size_linha1, size_coluna1;
double *A;//matriz que vai ser criada
FILE *p;
int main(int argc, char* argv[]){

    if(argc<2) {
      printf("Digite: %s <quantidade de equações>\n", argv[0]);
      return 1;
    }
    size_linha = atoi(argv[1]);//numero de equações do sistema, numero de linhas da matriz

    size_coluna=size_linha+1;//numero de variaveis do sistema menos, pois a última coluna é dos resultados, numero de colunas da matriz 
    srand(time(NULL));
    p = fopen("Sistema" , "wb");//abertura de arquivo
    A = (double *) malloc(sizeof(double) * size_linha * size_coluna);//alocação de memória para matriz
    if (A == NULL) {printf("ERRO--malloc1\n"); return 2;}
    
    fwrite(&size_linha, sizeof(long int), 1, p);//escreve no arquivo o número de linhas da matriz
    fwrite(&size_coluna, sizeof(long int), 1, p);//escreve no arquivo o número de linhas da matriz
    
    for(long long int i=0; i<size_linha; i++){
        for(long long int j=0; j<size_coluna; j++){
            A[i*size_coluna +j]=(rand())%100;//atribuição de valores aleatórios a matriz            
        }       
    }

    fwrite(A, sizeof(double), size_coluna*size_linha, p);//escrevendo no arquivo o sistema de equações criado
    
    fclose(p);
    free(A);

    return 0;
}
