
//Matheus Barroso de Santana - 120041661
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<time.h>


#define L 4 //numero de threads leitoras
#define E 2 //numero de threads escritoras

//variaveis do problema
int l=0; //contador de threads lendo
int e=0; //contador de threads escrevendo

sem_t leit, em_l, escr, em_e; //semaforos

//thread leitora
void * leitor (void * arg) {
    int *id = (int *) arg;
    while(1) { 
        sem_wait(&leit); 
        sem_wait(&em_l); l++; 
        if(l==1) sem_wait(&escr); 
        sem_post(&em_l); 
        sem_post(&leit); 
        printf("a thread leitora [%d], está lendo\n", *id); 
        sem_wait(&em_l); l--; 
        if(l==0) sem_post(&escr); 
        sem_post(&em_l); 
	    sleep(1);
    } 
  free(arg);
  pthread_exit(NULL);
}

//thread escritora
void * escritor (void * arg) {
  int *id = (int *) arg; 
while(1) {
	    printf("a thread escritora [%d], quer escrever\n", *id);
        sem_wait(&em_e); e++; 
        if(e==1) sem_wait(&leit);
        sem_post(&em_e);
        sem_wait(&escr);
        printf("a thread escritora [%d], está escrevendo\n", *id); 
        sem_post(&escr);
        sem_wait(&em_e); e--;
        if(e==0) sem_post(&leit);
        sem_post(&em_e);
	    sleep(1);
    }
  free(arg);
  pthread_exit(NULL);
}

//funcao principal
int main(void) {
  //identificadores das threads
  pthread_t tid[L+E];
  int id[L+E];

  //inicializa os semaforos
    sem_init(&escr, 0, 1); 
    sem_init(&em_e, 0, 1); 
    sem_init(&leit, 0, 1); 
    sem_init(&em_l, 0, 1); 


  //cria as threads leitoras
  for(int i=0; i<L; i++) {
    id[i] = i+1;
    if(pthread_create(&tid[i], NULL, leitor, (void *) &id[i])) exit(-1);
  } 
  
//   //cria as threads escritoras
   for(int i=0; i<E; i++) {
     id[i+L] = i+1;
     if(pthread_create(&tid[i+L], NULL, escritor, (void *) &id[i+L])) exit(-1);
   } 

  pthread_exit(NULL);
  return 0;
}

