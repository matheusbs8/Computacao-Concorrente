//Nome: Matheus Barroso de Santana
//DRE: 120041661

#include<stdio.h>
#include<pthread.h>

#define NTHREADS 2
#define TAMANHO 1000

float vetor[TAMANHO];

//funcao que a thread ira executar
void * quadrado (void * arg) {
   int ident = * (int *) arg;
   for(int i = 500*ident; i<500*ident + 500; i++){
       vetor[i]=vetor[i]*vetor[i];
   }
   
   pthread_exit(NULL);
}

//funcao principal
int main(void) {
    pthread_t tid[NTHREADS]; //identificador da thread no sistema
    int ident[NTHREADS]; //identificador local da thread
    
    
//inicializando vetor de numeros    
    for(int i=0; i<TAMANHO; i++) 
       vetor[i]=i+1;
    
    //cria as threads 
    for(int i=0; i<NTHREADS; i++) {
       ident[i] = i;
       if (pthread_create(&tid[i], NULL, quadrado, (void *)&ident[i])) 
          printf("ERRO -- pthread_create\n");
    }
    
    //espera as threads terminarem 
    for(int i=0; i<NTHREADS; i++) {
       if (pthread_join(tid[i], NULL)) 
          printf("ERRO -- pthread_join\n");
    }
    //imprimir o vetor 
    for(int i=0; i<TAMANHO; i++) 
       printf("%f ", vetor[i]);


    return 0;
}