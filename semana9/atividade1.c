#include <pthread.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <semaphore.h>  
#include <string.h>  
#define NTHREADS  5  
/* Variaveis globais */  
sem_t condInte, condFinal; 
int x = 3; 
  
typedef struct{  
   int id; //identificador do elemento que a thread ira processar  
   char frase[200]; //frase relacionada a thread  
} tArgs;  
 
 
/* Threads que incrementam x */  
  
void *Tarefa (void *t) {  
  tArgs *my_info = (tArgs*)t;  
   
  if (my_info->id==5) {  
    printf("%s\n",my_info->frase);  
    sem_post(&condInte); 
  }  
  else{  
    if(my_info->id!=1){  
      sem_wait(&condInte); 
      x--; 
      printf("%s\n",my_info->frase);  
      sem_post(&condInte); 
      if(x==0) 
      sem_post(&condFinal); 
    }  
    else{  
      sem_wait(&condFinal); 
      printf("%s\n",my_info->frase);  
    }  
  }  
  free(my_info);  
  pthread_exit(NULL);  
}  
  
/* Funcao principal */  
int main(int argc, char *argv[]) {  
  int i;   
  tArgs *t1, *t2, *t3, *t4, *t5;  
  pthread_t threads[NTHREADS];  
  //aloca espaco para os identificadores das threads  
  t1=malloc(sizeof(tArgs));   
  t2=malloc(sizeof(tArgs));   
  t3=malloc(sizeof(tArgs));   
  t4=malloc(sizeof(tArgs));   
  t5=malloc(sizeof(tArgs));   
  t1->id=1, t2->id=2, t3->id=3, t4->id=4, t5->id=5;  
  strcpy(t1->frase, "Volte sempre!");  
  strcpy(t2->frase, "Fique a vontade.");  
  strcpy(t3->frase, "Sente-se por favor.");  
  strcpy(t4->frase, "Aceita um copo d’agua?.");  
  strcpy(t5->frase, "Seja bem-vindo!");  
  
  sem_init(&condInte, 0, 0); 
  sem_init(&condFinal, 0, 0); 
  /* Cria as threads */  
  pthread_create(&threads[0], NULL, Tarefa, (void *)t1);  
  pthread_create(&threads[2], NULL, Tarefa, (void *)t3);  
  pthread_create(&threads[3], NULL, Tarefa, (void *)t4);  
  pthread_create(&threads[4], NULL, Tarefa, (void *)t5);  
  pthread_create(&threads[1], NULL, Tarefa, (void *)t2);  
    
  /* Espera todas as threads completarem */  
  for (i = 0; i < NTHREADS; i++) {  
    pthread_join(threads[i], NULL);  
  }  
    
  
 
}