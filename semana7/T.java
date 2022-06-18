import java.util.ArrayList;
import java.util.Vector;

class T extends Thread {
    //identificador da thread
    private int id;
    //numero de threads criadas
    private  int Nthreads;
    //local de armazenamento da lista de numeros inteiros
    private ArrayList vetor;
    //objeto compartilhado com outras threads
    RecursoCompartilhado recurso;

    //construtor
    public T(int tid, RecursoCompartilhado recurso, ArrayList v, int Nthreads) {
        this.id = tid;
        this.recurso = recurso;
        this.vetor=v;
        this.Nthreads=Nthreads;
    }

    //metodo main da thread
    public void run() {
        for (int i=id; i<vetor.size(); i+=Nthreads) {
            int n = (int)vetor.get(i);
            if(n%2==0){
                this.recurso.inc();
            }

        }
    }
}