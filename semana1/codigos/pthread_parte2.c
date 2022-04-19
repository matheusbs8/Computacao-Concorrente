#include <stdio.h>
#include<pthread.h>

#define NTHREADS 10

//função que a thread vai executar

void * tarefa (void * arg){
    int ident = *(int *) arg;
    printf("olá sou a thread nova, %d", ident);
    pthread_exit(NULL);
}




//função principal
int main(void){
    pthread_t tid[NTHREADS]; //identificador da thread no sistema
    int ident[NTHREADS];// identificador local da thread
    //cria novas threads
for(int i = 1; i<= NTHREADS; i++){}
    ident[i-1]=i;
    if(pthread_create(&tid[i-1], NULL, tarefa, (void *)&ident[i-1]))
        printf("DEU RUIM\n");
}
    //imprime a mensagem de boas vindas
    printf("olá sou a thread principal");
    //desvincula o termino da main do termiono do programa
    pthread_exit(NULL);
    return 0;
} 