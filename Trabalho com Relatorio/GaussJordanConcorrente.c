#include<stdio.h>
#include<math.h>
#include<stdlib.h>
// #include<pthread.h>
// #include "timer.h"

long int size_linha=10000;
long int size_coluna=10000;
int minimo;
int nthreads;
float *A, *identidade, *Acopia, *matrizFinal, tempofunction=0;
typedef struct{
   int id;
   int i; 
   int j; 
} tArgs;
void estica_encolhe(int indice,float fator){
    for(int i=0; i<minimo; i++)
      identidade[i*minimo + indice]*=(1.0/fator);
       
    for(int i=0; i<size_coluna; i++)
        A[indice*size_coluna + i] *= fator;  
}
void soma_subtracao(int i_static,int j_static, float fator){
    if(!(j_static>minimo || i_static>minimo)){
        for(int i=0; i<minimo; i++)
            identidade[i*minimo + j_static]+=(fator)*identidade[i*minimo + i_static];
    }
    for(int i=0; i<size_coluna; i++)
        A[i_static*size_coluna + i] -= fator* A[j_static*size_coluna + i];  
}
void troca(int i_static, int j_static){
    float aux;
    for(int i=0; i<size_coluna; i++){
        aux=A[i_static*size_coluna + i];
        A[i_static*size_coluna + i]=A[j_static*size_coluna + i];
        A[j_static*size_coluna + i]=aux;
    }
   if(!(j_static>minimo || i_static>minimo)){
        for(int i=0; i<minimo; i++){
            aux=identidade[i_static + i*minimo];
            identidade[i_static + i*minimo]=identidade[j_static + i*minimo];
            identidade[j_static + i*minimo]=aux;
        }
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

void preencheIdentidade(){
    for(long long int i=0; i<minimo; i++){
        for(long long int j=0; j<minimo; j++){
            if(i==j)
                identidade[i*minimo +j]=1.0;
            else
                identidade[i*minimo +j]=0.0;    
            }    
    }
}
void preencheMatrizA(){
    for(long long int i=0; i<size_linha; i++){
        for(long long int j=0; j<size_coluna; j++){
            A[i*size_coluna +j]=rand() % ((i+1)*(j+1));
            Acopia[i*size_coluna +j]=rand() % ((i+1)*(j+1));              
        }    
    }
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
void  Gauss_Jordan_Concorrente(){
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
int min(){
    if(size_coluna<size_linha)
        return size_coluna;
    else
         return size_linha;
}
void MultiSequencial(int dim){
 
    float aux=0;
    if(size_coluna<size_linha){
        for(int i=0; i<size_linha; i++){
            for(int j=0; j<dim; j++){ 
                matrizFinal[i*dim +j]=0;
                aux=0;
                for(int k=0; k<dim; k++){
                    aux+=A[i*dim +k]*identidade[k*dim+j];               
                }
                matrizFinal[i*dim+j]=aux;
            }
        }
    }
    else{
        for(int i=0; i<minimo; i++){
            for(int j=0; j<size_coluna; j++){ 
                matrizFinal[i*size_coluna +j]=0;
                aux=0;
                for(int k=0; k<dim; k++){
                    aux+=identidade[i*dim +k]*A[k*size_coluna+j];               
                }
                matrizFinal[i*size_coluna+j]=aux;
            }
        }
    }
}
int verifica(){
    int aux = 0;
        for(int i=0; i<size_coluna; i++){
            for(int j=0; j<size_linha; j++){
                if(matrizFinal[i*size_coluna+j]!=Acopia[i*size_coluna+j]){
                    aux=1;
                    break;
                }
            }     
            if(aux==1)
            break;            
        }
    return aux;
 }
int main(int argc, char* argv[]){

       if(argc<2) {
      printf("Digite: %s <número de threads>\n", argv[0]);
      return 1;
   }
   size_linha = atoi(argv[1]);
   size_coluna = atoi(argv[2]);
   nthreads = atoi(argv[3]);
    A = (float *) malloc(sizeof(float) * size_linha * size_coluna);
    if (A == NULL) {printf("ERRO--malloc\n"); return 2;}
    Acopia = (float *) malloc(sizeof(float) * size_linha * size_coluna);
    if (Acopia == NULL) {printf("ERRO--malloc\n"); return 2;}
    matrizFinal = (float *) malloc(sizeof(float) * size_linha * size_coluna);
    if (matrizFinal == NULL) {printf("ERRO--malloc\n"); return 2;}
    minimo = min();
    identidade = (float *) malloc(sizeof(float) * minimo * minimo);
    if (identidade == NULL) {printf("ERRO--malloc\n"); return 2;}
    
    float ini, fim, tempoGauss, tempoS; printf("aaa");
    preencheIdentidade();
    preencheMatrizA();
    GET_TIME(ini);
    Gauss_Jordan_Concorrente();
    GET_TIME(fim);
    tempoGauss=fim-ini;
    printf("%lf",tempoGauss);
    MultiSequencial(min());
    if(verifica(min()))
        printf("\na multiplicação das matrizes obtidas pela fatoração gausseana, é igual a matriz original\n");
   
    free(A);
    free(Acopia);
    free(identidade);
    free(matrizFinal);
    return 0;
}