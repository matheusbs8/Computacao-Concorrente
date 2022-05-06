# Laboratório 03 | Relatório
Análise de desempenho e cálculo da aceleração do programa de solução sequencial e uma solução concorrente para o problema de encontrar o menor e o maior valor de um vetor de numeros reais.


## Índice
* [Informações sobre o hardware](#informações-sobre-o-hardware)
* [Estratégia de Divisão das threads](#estrategia-de-divisao-das-threads)
* [Como rodar o programa](#como-rodar-o-programa)
* [Cálculo da aceleração](#cálculo-da-aceleração)

## Informações sobre o hardware
- Distro: Ubuntu 20.04.4 LTS
- Nome do Modelo: Intel(R) Core(TM) i3-8130U CPU @ 2.10GHz
- Arquitetura: x86_64

## Estratégia de Divisão das threads
:exclamation: Optei por fazer uma divisão das threads em blocos, portanto, tendo 4 threads, dividimos o vetor em 4 grandes blocos, nos quais cada thread estará processando seu bloco respectivo.

## Como rodar o programa
:thinking: Compila e executa o programa através dos comandos:

$ gcc lab3.c -o maior-menor -Wall -lpthread 


$ ./maior-menor <dimensão do vetor> <número de threads>


## Cálculo da aceleração
:abacus: Para realizar o cálculo da aceleração e poder estimar o ganho de desempenho, usaremos a Lei de Amdahl. O ganho de velocidade da execução é dado por: 
T<sub>sequencial</sub> / t<sub>s</sub> + t<sub>s</sub>, onde
- T<sub>sequencial</sub>: Tempo total de execução do programa sequencial
- t<sub>s</sub>: Tempo da parte sequencial do programa
- t<sub>s</sub>: Tempo da parte concorrente do programa usando P processadores

### Dimensão 10<sup>5</sup>
 | Threads | Aceleração |
 | --- | --- |
| 1 Threads | 0.940478 |
| 2 Threads | 1.328333 |
| 4 Threads | 0.718548 |
 
### Dimensão 10<sup>7</sup>
 | Threads | Aceleração |
 | --- | --- |
 | 1 Threads | 1.013941 |
 | 2 Threads | 2.000502 |
 | 4 Threads | 2.241935 |
 
 ### Dimensão 10<sup>8</sup>
 | Threads | Aceleração |
 | --- | --- |
 | 1 Threads | 1.053315 |
 | 2 Threads | 2.081315 |
 | 4 Threads | 2.363092 |
