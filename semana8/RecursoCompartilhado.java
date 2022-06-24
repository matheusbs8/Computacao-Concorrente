public class RecursoCompartilhado {
    //recurso compartilhado
    private int contador;
    //construtor
    public RecursoCompartilhado() {
        this.contador = 0;
    }


    public  void inc() {
        this.contador++;
    }

    public int get() {
        return this.contador;
    }
    public void set(int n) {this.contador=n;}

}