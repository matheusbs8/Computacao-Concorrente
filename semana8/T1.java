class T1 extends Thread {
    int id; //identificador da thread
    RecursoCompartilhado variavelCentral; //variavel central
    LE monitor;//objeto monitor para coordenar a lógica de execução das threads

    // Construtor
    T1 (int id, RecursoCompartilhado variavelCentral, LE m) {
        this.id = id;
        this.variavelCentral=variavelCentral;
        this.monitor = m;
    }

    // Método executado pela thread
    public void run () {
        this.monitor.EntraEscritor(this.id);
        variavelCentral.inc();//incrementa o valor em 1 unidade
        this.monitor.SaiEscritor(this.id);
    }
}