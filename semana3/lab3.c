//encontrar o maior e menor de um vetor
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

long int dim; //dimensao do vetor de entrada
int nthreads; //numero de threads
int *vetor; //vetor de entrada com dimensao dim 

typedef struct{
   long int maior; 
   long int menor; 
} tArgs;

//fluxo das threads
void * tarefa(void * arg) {
   long int id = (long int) arg; //identificador da thread
   tArgs *fator = malloc(sizeof(tArgs));
   long int tamBloco = dim/nthreads;  //tamanho do bloco de cada thread 
   long int ini = id * tamBloco; //elemento inicial do bloco da thread
   long int maior=vetor[ini], menor=vetor[ini];//maior e menor locais 
//que guardarão o maior e menor valor respectivamente, de uma porção do vetor
   
   long int fim; //elemento final(nao processado) do bloco da thread
   if(id == nthreads-1) fim = dim;
   else fim = ini + tamBloco; //trata o resto se houver
   //encontrar o maior e menor valor do bloco da thread
   for(long int i=ini; i<fim; i++){
        if(vetor[i]>maior){
            maior = vetor[i];
        }
        if(vetor[i]<menor){
            menor = vetor[i];
        }
   }
     
     fator->maior=maior;
     fator->menor=menor;
   //retorna o o maior e menor valor do bloco do vetor
   pthread_exit((void *) fator); 
}

//fluxo principal
int main(int argc, char *argv[]) {
    double ini, fim, tempoC, tempoS;//tomada de tempo
    int maiorS, menorS, maiorC, menorC; 
    pthread_t *tid; //identificadores das threads no sistema
    tArgs *retorno; //valor de retorno das threads

    //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
    if(argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
        return 1; 
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);

    //aloca o vetor de entrada
    vetor = (int*) malloc(sizeof(int)*dim);
    if(vetor == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }
    //preenche o vetor de entrada
    for(long int i=0; i<dim; i++)
        vetor[i] = rand() % (i+1);
    
    //
    GET_TIME(ini);
    for(long int i=0; i<dim; i++){
        if(i==0){
            maiorS=vetor[i];
            menorS=vetor[i];
        } else {
            if(vetor[i]>maiorS){
                maiorS = vetor[i];
            }
            if(vetor[i]<menorS){
                menorS = vetor[i];
            }
        }
    }
    GET_TIME(fim);
    tempoS=fim-ini;
    printf("Tempo sequencial:  %lf\n", fim-ini);

    //encontrar maior e menor de forma concorrente
    GET_TIME(ini);
    maiorC=vetor[0];
    menorC=vetor[0];

    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid==NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }
    //criar as threads
    for(long int i=0; i<nthreads; i++) {
        if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
	
    //aguardar o termino das threads
    for(long int i=0; i<nthreads; i++) {
        if(pthread_join(*(tid+i), (void**) &retorno)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
        //maior e menor globais
        if(retorno->maior>maiorC)
        maiorC=retorno->maior;
        if(retorno->menor<menorC)
        menorC=retorno->menor;    

    }
    GET_TIME(fim);
    tempoC=fim-ini;
    printf("Tempo concorrente:  %lf\n", fim-ini);

    //exibir os resultados
	if(maiorC==maiorS && menorS==menorC)
    		printf("os resultados são compátibeis\n");
   	else
		printf("ERRO!!\nvalores diferentes\n");

      
 printf("percentual :%lf\n", tempoS/tempoC);
    //libera as areas de memoria alocadas
    free(vetor);
    free(tid);

    return 0;
}
