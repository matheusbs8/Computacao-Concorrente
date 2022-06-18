public class RecursoCompartilhado {
        //recurso compartilhado
        private int contador;
        //construtor
        public RecursoCompartilhado() {
            this.contador = 0;
        }


        public synchronized void inc() {
            this.contador++;
        }

        public synchronized int get() {
            return this.contador;
        }

    }