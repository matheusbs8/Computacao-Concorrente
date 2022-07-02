#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NTHREADS  5
/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond_intermedio;
pthread_cond_t x_cond_final;

typedef struct{
   int id; //identificador do elemento que a thread ira processar
   char frase[200]; //frase relacionada a thread
} tArgs;
/* Threads que incrementam x */

void *Tarefa (void *t) {
  tArgs *my_info = (tArgs*)t;
 
  pthread_mutex_lock(&x_mutex);
  if (x == 0 && my_info->id==5) {
    printf("%s\n",my_info->frase);
    x=x+1;
    pthread_cond_broadcast(&x_cond_intermedio);
    pthread_mutex_unlock(&x_mutex);
  }
  else{
    if(my_info->id!=1){
      if(x<1)
        pthread_cond_wait(&x_cond_intermedio, &x_mutex);  
      printf("%s\n",my_info->frase);
      x=x+1;
      if(x==4)
        pthread_cond_signal(&x_cond_final);
      pthread_mutex_unlock(&x_mutex);
    }
    else{
      if(x<4)
        pthread_cond_wait(&x_cond_final, &x_mutex);  
      printf("%s\n",my_info->frase);
      pthread_mutex_unlock(&x_mutex);
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

  /* Inicilaiza o mutex (lock de exclusao mutua) e as variaveis de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond_intermedio, NULL);//variavel de condição das threads intermediárias
  pthread_cond_init (&x_cond_final, NULL);//variavel de condição da thread final
  
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
  
  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond_intermedio);
  pthread_cond_destroy(&x_cond_final);
}