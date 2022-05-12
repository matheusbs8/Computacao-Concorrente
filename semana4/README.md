# Laboratório 04 | Relatório
Análise de desempenho e cálculo da aceleração do programa de solução sequencial e uma solução concorrente para o problema de retornar um vetor, no qual é uma modificação de um vetor de entrada, onde as posições contendo números primos receberão a raiz quadrada do valor inicial


## Índice
* [Informações sobre o hardware](#informações-sobre-o-hardware)
* [Cálculo da aceleração](#cálculo-da-aceleração)

## Informações sobre o hardware
- Distro: Ubuntu 20.04.4 LTS
- Nome do Modelo: Intel(R) Core(TM) i3-8130U CPU @ 2.10GHz
- Arquitetura: x86_64


## Cálculo da aceleração
:abacus: Para realizar o cálculo da aceleração e poder estimar o ganho de desempenho, usaremos a Lei de Amdahl. O ganho de velocidade da execução é dado por: 
T<sub>sequencial</sub> / t<sub>s</sub> + t<sub>s</sub>, onde
- T<sub>sequencial</sub>: Tempo total de execução do programa sequencial
- t<sub>s</sub>: Tempo da parte sequencial do programa
- t<sub>s</sub>: Tempo da parte concorrente do programa usando P processadores

### Dimensão 10<sup>5</sup>
 | Threads | Aceleração |
 | --- | --- |
| 1 Threads | 0.585214 |
| 2 Threads | 0.789706 |
| 4 Threads | 1.243356 |
 
### Dimensão 10<sup>7</sup>
 | Threads | Aceleração |
 | --- | --- |
 | 1 Threads | 0.984620 |
 | 2 Threads | 1.847164 |
 | 4 Threads | 2.133143 |
 
 ### Dimensão 10<sup>8</sup>
 | Threads | Aceleração |
 | --- | --- |
 | 1 Threads | não executou |
 | 2 Threads | 1.972931 |
 | 4 Threads | 2.982562 |
