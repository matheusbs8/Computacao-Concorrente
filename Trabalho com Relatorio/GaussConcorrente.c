#include<stdio.h>
#include<math.h>
#include<stdlib.h>
//#include <pthread.h>
// #include "timer.h"
long int size_linha;
long int size_coluna;
int nthreads;
double *A, tempofunction=0, *Coeficientes, *Resultado, *ResultadoGauss, *ResultadoFinal;
typedef struct{
   int id;
   int i; 
   int j; 
} tArgs;
FILE *p;
void estica_encolhe(int indice,double fator){
    for(int i=0; i<size_coluna; i++)
        A[indice*size_coluna + i] *= fator;  
}

void soma_subtracao(int i_static,int j_static, double fator){
    for(int i=0; i<size_coluna; i++)
        A[i_static*size_coluna + i] -= fator* A[j_static*size_coluna + i];  
}

void troca(int i_static, int j_static){
    double aux;
    for(int i=0; i<size_coluna; i++){
        aux=A[i_static*size_coluna + i];
        A[i_static*size_coluna + i]=A[j_static*size_coluna + i];
        A[j_static*size_coluna + i]=aux;
    }

}

int encontra_pivo(int i,int j){
    int i_troca=-1;
    for(int k = i; k<size_linha; k++){
        if(A[k*size_coluna + j]!=0.0){
            i_troca=k;
            break;
        }
    }
    return i_troca;  
}

void * Zerar (void * arg) {
   tArgs *args = (tArgs*) arg;
   for(int k = args->id; k<size_linha; k+=nthreads){
        if (k!=args->i)
            soma_subtracao(k, args->i, A[k*size_coluna +args->j]);
    }
   
   pthread_exit(NULL);
}

void zerar_coluna_concorrente(int i_static,int j_static){
    pthread_t tid[nthreads];
    tArgs *args;
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
   if(args==NULL) {puts("ERRO--malloc"); }
    for(int i=0; i<nthreads; i++){
        (args+i)->id = i;
        (args+i)->i = i_static;
        (args+i)->j=j_static;
        if (pthread_create(&tid[i], NULL, Zerar, (void *)(args + i))) 
            printf("ERRO -- pthread_create\n");
    }
    
    //espera as threads terminarem 
    for(int i=0; i<nthreads; i++) {
       if (pthread_join(tid[i], NULL)) 
          printf("ERRO -- pthread_join\n");
    }
    free(args);
 }
void  Gauss_Jordan(){
    
    int i_atual=0;
    int j_atual=0;
    while(i_atual<size_linha && j_atual<size_coluna){
       
        int i_troca = encontra_pivo(i_atual, j_atual);
            
        if(i_troca == -1){
            if(i_atual==size_linha){
                j_atual=size_coluna;
                continue;
            }
            j_atual+=1;
            continue;
        }
      
        if(i_atual!=i_troca)
            troca(i_atual, i_troca);
       
        estica_encolhe(i_atual, 1.0/A[i_atual*size_coluna + j_atual]);
        
        zerar_coluna_concorrente(i_atual, j_atual);
        
        i_atual+=1;
        j_atual+=1;  
       
    }
}


int verifica(){
    int aux = 1;
    for(int i=0; i<size_linha; i++){
        printf("%f - %f\n", Resultado[i], ResultadoFinal[i]);
        if(abs(Resultado[i]-ResultadoFinal[i])>0.000001){
            aux=0;
            break;
        }
    }     
    return aux;
 }

void ResolveSistema(){
 
    double aux=0;
    for(int i=0; i<size_linha; i++){
        
      for(int k=0; k<size_linha; k++) 
            aux+=Coeficientes[i*size_linha+ k]*ResultadoGauss[k];
        ResultadoFinal[i]=aux;
        aux=0;
    
    }
}


void separa(){
    for(int i=0; i<size_linha; i++){
        for(int j=0; j<size_coluna; j++){
            if(j==size_coluna-1){
                Resultado[i] =A[i*size_coluna+j]; 
            }
            else{
                Coeficientes[i*size_linha + j] =A[i*size_coluna+j];   
            }
        }     
                    
    }  
}

void extrairResultado(){
    for(int i=0; i<size_linha; i++){
        ResultadoGauss[i]=A[i*size_coluna + size_coluna-1];
    }    
}


int main(int argc, char* argv[]){
    if(argc<2) {
      printf("Digite: %s <número de threads>\n", argv[0]);
      return 1;
   }
   nthreads = atoi(argv[1]);
    p = fopen("Sistema" , "rb");

    fread(&size_linha, sizeof(long int), 1, p);
    fread(&size_coluna, sizeof(long int), 1, p);
    
    A = (double *) malloc(sizeof(double) * size_linha * size_coluna);
    if (A == NULL) {printf("ERRO--malloc1\n"); return 2;}

    fread(A, sizeof(double), size_coluna*size_linha, p);

    Coeficientes  = (double *) malloc(sizeof(double) * size_linha * size_linha);
    if (Coeficientes == NULL) {printf("ERRO--malloc2\n"); return 2;}

    Resultado  = (double *) malloc(sizeof(double) * size_linha);
    if (Resultado == NULL) {printf("ERRO--malloc3\n"); return 2;}

    ResultadoGauss  = (double *) malloc(sizeof(double) * size_linha);
    if (ResultadoGauss == NULL) {printf("ERRO--malloc4\n"); return 2;}

    ResultadoFinal  = (double *) malloc(sizeof(double) * size_linha);
    if (ResultadoFinal == NULL) {printf("ERRO--malloc5\n"); return 2;}
    separa();

    double ini, fim, tempoGauss, tempoS; 
    // //GET_TIME(ini);
    Gauss_Jordan();

    extrairResultado();
    // //GET_TIME(fim);
    ResolveSistema();
    
    // tempoGauss=fim-ini;
    // //printf("%lf",tempoGauss);
   
    if(verifica())
        printf("\na multiplicação das matrizes obtidas pela fatoração gausseana, é igual a matriz original\n");
   
    free(A);
    free(Coeficientes);
    free(ResultadoFinal);
    free(Resultado);
    free(ResultadoGauss);
    return 0;
}