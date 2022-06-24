#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include <pthread.h>
#include "timer.h"
long int size_linha;
long int size_coluna;
double *A, tempofunction=0, *Coeficientes, *Resultado, *ResultadoGauss, *ResultadoFinal;

FILE *p;
void estica_encolhe(int indice,double fator){
    for(int i=0; i<size_coluna; i++)
        A[indice*size_coluna + i] *= fator;  
}

void soma_subtracao(int i_static,int j_static, double fator){
    for(int i=0; i<size_coluna; i++)
        A[i_static*size_coluna + i] -= fator* A[j_static*size_coluna + i];  
}

void troca(int i_static, int j_static){
    double aux;
    for(int i=0; i<size_coluna; i++){
        aux=A[i_static*size_coluna + i];
        A[i_static*size_coluna + i]=A[j_static*size_coluna + i];
        A[j_static*size_coluna + i]=aux;
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

void zerar_coluna(int i,int j){
    // double iniF=0, fimF=0;
    // GET_TIME(iniF);  
    for(int k = 0; k<size_linha; k++){
        if (k!=i)
            soma_subtracao(k, i, A[k*size_coluna +j]);
    }
    // GET_TIME(fimF);
    // tempofunction+=(fimF -iniF);
 }

void  Gauss_Jordan(){
    
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
        
        zerar_coluna(i_atual, j_atual);
        
        i_atual+=1;
        j_atual+=1;  
       
    }
}


int verifica(){
    int aux = 1;
    for(int i=0; i<size_linha; i++){
       
        if(abs(Resultado[i]-ResultadoFinal[i])>0.00000001){
            aux=0;
            break;
        }
    }     
    return aux;
 }

void ResolveSistema(){
 
    double aux=0;
    for(int i=0; i<size_linha; i++){
        
      for(int k=0; k<size_linha; k++) 
            aux+=Coeficientes[i*size_linha+ k]*ResultadoGauss[k];
        ResultadoFinal[i]=aux;
        aux=0;
    
    }
}


void separa(){
    for(int i=0; i<size_linha; i++){
        for(int j=0; j<size_coluna; j++){
            if(j==size_coluna-1){
                Resultado[i] =A[i*size_coluna+j]; 
            }
            else{
                Coeficientes[i*size_linha + j] =A[i*size_coluna+j];   
            }
        }     
                    
    }  
}

void extrairResultado(){
    for(int i=0; i<size_linha; i++){
        ResultadoGauss[i]=A[i*size_coluna + size_coluna-1];
    }    
}


int main(int argc, char* argv[]){

    p = fopen("Sistema" , "rb");

    fread(&size_linha, sizeof(long int), 1, p);
    fread(&size_coluna, sizeof(long int), 1, p);
    
    A = (double *) malloc(sizeof(double) * size_linha * size_coluna);
    if (A == NULL) {printf("ERRO--malloc1\n"); return 2;}

    fread(A, sizeof(double), size_coluna*size_linha, p);

    Coeficientes  = (double *) malloc(sizeof(double) * size_linha * size_linha);
    if (Coeficientes == NULL) {printf("ERRO--malloc2\n"); return 2;}

    Resultado  = (double *) malloc(sizeof(double) * size_linha);
    if (Resultado == NULL) {printf("ERRO--malloc3\n"); return 2;}

    ResultadoGauss  = (double *) malloc(sizeof(double) * size_linha);
    if (ResultadoGauss == NULL) {printf("ERRO--malloc4\n"); return 2;}

    ResultadoFinal  = (double *) malloc(sizeof(double) * size_linha);
    if (ResultadoFinal == NULL) {printf("ERRO--malloc5\n"); return 2;}
    separa();

    double ini, fim, tempoGauss, tempoS; 
    GET_TIME(ini);
    Gauss_Jordan();
    GET_TIME(fim);
    extrairResultado();
    
    ResolveSistema();
    
    tempoGauss=fim-ini;
    printf("tempo de execução: %lf\n",tempoGauss);
   
    if(verifica())
        printf("\nresultado verificado\n");
   
    free(A);
    free(Coeficientes);
    free(ResultadoFinal);
    free(Resultado);
    free(ResultadoGauss);
    return 0;
}