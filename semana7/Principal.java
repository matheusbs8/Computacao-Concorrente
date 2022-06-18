import java.util.ArrayList;
import java.util.Random;

class Principal{
    static final int N = 4;

    public static void main (String[] args) {
        //reserva espaço para um vetor de threads
        Thread[] threads = new Thread[N];
        //contador de números pares
        int contador=0;

        //preenche vetor
        ArrayList vetor = new <Integer>ArrayList();
        Random a = new Random();
        //tamanho do valor aleatório
        int tamanho = a.nextInt(10,1000);
        for (int i =0; i<tamanho; i++){
            //valor aleatório
            vetor.add(a.nextInt(i+1, 1000));
        }

        //calculo sequencial
        for (int i =0; i<tamanho; i++){
            int n = (int)vetor.get(i);
            if(n%2==0){
               contador++;
            }
        }

        //cria uma instancia do recurso compartilhado entre as threads
        RecursoCompartilhado recurso = new RecursoCompartilhado();

        //cria as threads da aplicacao
        for (int i=0; i<threads.length; i++) {
            threads[i] = new T(i, recurso, vetor, N);
        }

        //inicia as threads
        for (int i=0; i<threads.length; i++) {
            threads[i].start();
        }

        //espera pelo termino de todas as threads
        for (int i=0; i<threads.length; i++) {
            try { threads[i].join(); } catch (InterruptedException e) { return; }
        }
        //verificação do calculo
        if(contador == recurso.get())
            System.out.println("quantidade de número de pares = " + recurso.get());
        else
            System.out.println("Ocorreu um erro no cálculo");
    }
}