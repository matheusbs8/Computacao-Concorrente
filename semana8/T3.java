class T3 extends Thread {
    int id; //identificador da thread
    RecursoCompartilhado variavelCentral; //variavel central
    LE monitor; //objeto monitor para coordenar a lógica de execução das threads

    // Construtor
    T3 (int id, RecursoCompartilhado variavelCentral, LE m) {
        this.id = id;
        this.variavelCentral=variavelCentral;
        this.monitor = m;
    }

    // Método executado pela thread
    public void run () {

                int i;
                double j = 234243424;
                this.monitor.EntraLeitor(this.id);
                System.out.println("thread3 de id: (" + id + "), constatou " + variavelCentral.get());//le a variavel
                this.monitor.SaiLeitor(this.id);
            //Processamento bobo mais significativo
                for (i = 0; i < 100000000; i++) {
                    j = j / 2;
                } try {
                sleep(11111);
                }catch (InterruptedException e){return;}

                this.monitor.EntraEscritor(this.id);
                variavelCentral.set(this.id);//atribui o valor do id a variavel central
                this.monitor.SaiEscritor(this.id);

    }
}