/* Multiplicacao de matriz-vetor (considerando matrizes quadradas) */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

float *mat1; //matriz de entrada 1
float *mat2;  //matriz de entrada 2
float *Matriz_saida; //vetor de saida
float *Matriz_saida_Concorrente;
int nthreads; //numero de threads

typedef struct{
   int id; //identificador do elemento que a thread ira processar
   int dim; //dimensao das estruturas de entrada
} tArgs;

void MultiSequencial(int dim){
   Matriz_saida = (float *) malloc(sizeof(float) * dim * dim);
  
	float aux=0;
		for(int i=0; i<dim; i++){
	      	for(int j=0; j<dim; j++){ 
				Matriz_saida[i*dim +j]=0;
				for(int k=0; k<dim; k++){
					aux+=mat1[i*dim +k]*mat2[k*dim+j];				
				}
			    Matriz_saida[i*dim+j]=aux;
				aux=0;
			}
		}
}

int verifica(int dim){
int aux = 1;
	for(int i=0; i<dim; i++){
	    for(int j=0; j<dim; j++){
		    if(Matriz_saida[i*dim+j]!=Matriz_saida_Concorrente[i*dim+j]){
			    aux=0;
			    break;
		      }
	    }     
		if(aux==0)
		  break;
		  
	}
return aux;
 }


//funcao que as threads executarao
void * tarefa(void *arg) {
   tArgs *args = (tArgs*) arg;
   //printf("Thread %d\n", args->id);
int aux =0;
   for(int i=args->id; i<args->dim; i+=nthreads){
      for(int j=0; j<args->dim; j++){ 
         Matriz_saida_Concorrente[i*args->dim +j]=0;
		for(int k=0; k<args->dim; k++){
		    aux+=mat1[i*args->dim +k]*mat2[k*args->dim+j];				
		}
		Matriz_saida_Concorrente[i*args->dim+j]=aux;
		aux=0;
	}
	}



   pthread_exit(NULL);
}

//fluxo principal
int main(int argc, char* argv[]) {
   int dim; //dimensao da matriz de entrada
   pthread_t *tid; //identificadores das threads no sistema
   tArgs *args; //identificadores locais das threads e dimensao
   double inicio, fim, delta, tempoS, tempoC;
   

   //leitura e avaliacao dos parametros de entrada
   if(argc<3) {
      printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
      return 1;
   }
   dim = atoi(argv[1]);
   nthreads = atoi(argv[2]);
   if (nthreads > dim) nthreads=dim;

   //alocacao de memoria para as estruturas de dados
   mat1 = (float *) malloc(sizeof(float) * dim * dim);
   if (mat1 == NULL) {printf("ERRO--malloc\n"); return 2;}
   mat2 = (float *) malloc(sizeof(float) * dim * dim);
   if (mat2 == NULL) {printf("ERRO--malloc\n"); return 2;}
   Matriz_saida_Concorrente = (float *) malloc(sizeof(float) * dim * dim);
   if (Matriz_saida_Concorrente == NULL) {printf("ERRO--malloc\n"); return 2;}

   //inicializacao das estruturas de dados de entrada e saida
   for(int i=0; i<dim; i++) {
      for(int j=0; j<dim; j++){
         mat1[i*dim+j] = rand()%10;    //número aleatório
	 mat2[i*dim+j] = rand()%10;
	}      
   }
 
   //multiplicacao das matrizes sequencial
   GET_TIME(inicio);
   MultiSequencial(dim);
   GET_TIME(fim);
   delta = fim - inicio;
   tempoS=delta;
   printf("Tempo multiplicação sequencial:%lf\n", delta);	

   //multiplicacao das matrizes concorrente
   GET_TIME(inicio);
   //alocacao das estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}
   args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
   if(args==NULL) {puts("ERRO--malloc"); return 2;}
   //criacao das threads
   for(int i=0; i<nthreads; i++) {
      (args+i)->id = i;
      (args+i)->dim = dim;
      if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
         puts("ERRO--pthread_create"); return 3;
      }
   } 
   //espera pelo termino da threads
   for(int i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), NULL);
   }
   GET_TIME(fim)   
   delta = fim - inicio;
   tempoC=delta;
   printf("Tempo multiplicacao concorrente:%lf\n", delta);
  printf("percentual :%lf\n", tempoS/tempoC);
	if(verifica(dim))
   		printf("os resultados são compatíbeis\n");
	else
	printf("os resultados não são compatíbeis\n");


   free(mat1);
   free(mat2);
   free(Matriz_saida_Concorrente);
   free(Matriz_saida);
   free(args);
   free(tid);

   return 0;
}
