class T2 extends Thread {
    int id; //identificador da thread
    RecursoCompartilhado variavelCentral; //variavel central
    LE monitor; //objeto monitor para coordenar a lógica de execução das threads

    // Construtor
    T2 (int id, RecursoCompartilhado variavelCentral, LE m) {
        this.id = id;
        this.variavelCentral=variavelCentral;
        this.monitor = m;
    }

    // Método executado pela thread
    public void run () {
        try {

        this.monitor.EntraLeitor(this.id);
        if (variavelCentral.get() % 2 == 0)//verificação par ou impar
            System.out.println("thread2 de id: (" + id + "), constatou o valor par");
        else
            System.out.println("thread2 de id: (" + id + "), constatou o valor ímpar");
        this.monitor.SaiLeitor(this.id);
    sleep(11111);

}catch(InterruptedException e){return;}

    }
}