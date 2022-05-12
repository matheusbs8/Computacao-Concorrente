// gcc lab4.c -o lab4 -lpthread -lm
//encontrar o maior e menor de um vetor
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"
long long int dim; //dimensao do vetor de entrada
int nthreads, i_global=0, *vetorEntrada; //numero de threads
double  *vetorSaidaSequencial, *vetorSaidaConcorrente;
pthread_mutex_t mutex; //variavel de lock para exclusao mutua

int ehPrimo(long long int n){
    if(n<=1) return 0;
    if(n==2) return 1;
    if(n%2==0) return 0;
    for(int i=3; i<sqrt(n)+1; i+=2)
        if(n%i==0) return 0;
    return 1;    
}

void * Tarefa_processaPrimos(void * arg) {
    int i_local;
    
    pthread_mutex_lock(&mutex);
    i_local=i_global;
    i_global++;
    pthread_mutex_unlock(&mutex);
    
    while(i_local<dim) {
        if (ehPrimo(vetorEntrada[i_local]))
            vetorSaidaConcorrente[i_local] = sqrt(vetorEntrada[i_local]);
        else
            vetorSaidaConcorrente[i_local] = vetorEntrada[i_local];
        
        pthread_mutex_lock(&mutex);
        i_local=i_global;
        i_global++;
        pthread_mutex_unlock(&mutex);    
    
    }
    pthread_exit(NULL);
}

void processaPrimos(int *vetorEntrada, double *vetorSaida, int dim) {
    for(int i=0; i<dim; i++) {
        if (ehPrimo(vetorEntrada[i]))
            vetorSaida[i] = sqrt(vetorEntrada[i]);
         else
            vetorSaida[i] = vetorEntrada[i];
    }
}


//fluxo principal
int main(int argc, char *argv[]) {
    double ini, fim, tempoC, tempoS;//tomada de tempo
    pthread_t *tid; //identificadores das threads no sistema
 
    //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
    if(argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
        return 1; 
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);

    //alocação de memória aos vetores
    vetorEntrada = (int*) malloc(sizeof(int)*dim);
    vetorSaidaSequencial = (double*) malloc(sizeof(double)*dim);
    vetorSaidaConcorrente = (double*) malloc(sizeof(double)*dim);
    
    if(vetorEntrada == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }

    if(vetorSaidaConcorrente == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }

    if(vetorSaidaSequencial == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }
    //preenche o vetor de entrada
    for(long int i=0; i<dim; i++)
        vetorEntrada[i] = rand() % (i+1);
    
    //execução da função que retorna um vetor no qual, ocorre alteração no vetor de entrada, 
    //no qual os valores que forem primos se tornam suas raizez quadrada, de forma sequencial
    GET_TIME(ini);
    processaPrimos(vetorEntrada, vetorSaidaSequencial, dim);
    GET_TIME(fim);

    tempoS=fim-ini;
    printf("Tempo sequencial:  %lf\n", fim-ini);
    
    //execução da função que retorna um vetor no qual, ocorre alteração no vetor de entrada, 
    //no qual os valores que forem primos se tornam suas raizez quadrada, de forma concorrente
    GET_TIME(ini);
  
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid==NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }
    pthread_mutex_init(&mutex, NULL);
    //criar as threads
    for(long int i=0; i<nthreads; i++) {
        if(pthread_create(tid+i, NULL, Tarefa_processaPrimos, (void*) i)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
    
    //aguardar o termino das threads
    for(long int i=0; i<nthreads; i++) {
        if(pthread_join(*(tid+i), NULL)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
          
    }
    GET_TIME(fim);
    tempoC=fim-ini;
    printf("Tempo concorrente:  %lf\n", fim-ini);

    //verifica resultados
    int verifica =1;
    for(long int i=0; i<dim; i++){
        if(abs(vetorSaidaConcorrente[i]-vetorSaidaSequencial[i])>0.000000001){
            verifica=0;
            break;
        }
    }

    //exibe o resultado
    if(verifica)
        printf("os resultados são compátibeis\n");
    else
        printf("ERRO!!\nvalores diferentes\n");
      
    printf("percentual :%lf\n", tempoS/tempoC);

    //libera as areas de memoria alocadas
    pthread_mutex_destroy(&mutex);
    free(vetorEntrada);
    free(vetorSaidaSequencial);
    free(vetorSaidaConcorrente);
    free(tid);
    return 0;
}