#include <stdio.h>
#include<pthread.h>

//função que a thread vai executar

void * tarefa (void * arg){
    printf("olá sou a thread nova");
    pthread_exit(NULL);
}




//função principal
int main(void){
    pthread_t tid; //identificador da thread no sistema
    //cria nova thread
    if(pthread_create(&tid, NULL, tarefa, NULL))
        printf("DEU RUIM\n");
    //imprime a mensagem de boas vindas
    printf("olá sou a thread principal");
    //desvincula o termino da main do termiono do programa
    pthread_exit(NULL);
    return 0;
}