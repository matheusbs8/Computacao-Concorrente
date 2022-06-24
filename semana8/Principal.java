
class Principal {
    static final int L = 4;
    static final int E = 3;

    public static void main (String[] args) {
        int i;
        RecursoCompartilhado r = new RecursoCompartilhado();
        LE monitor = new LE();            // Monitor (objeto compartilhado entre leitores e escritores)
        T1[] t1 = new T1[L];       // Threads leitores
        T2[] t2 = new T2[E];   // Threads escritores
        T3[] t3 =new T3[E*L];



        for (i=0; i<L; i++) {
            t1[i] = new T1(i+1, r, monitor);
            t1[i].start();
        }
        for (i=0; i<E; i++) {
            t2[i] = new T2(i+1, r, monitor);
            t2[i].start();
        }

        for (i=0; i<E*L; i++) {
            t3[i] = new T3(i+1, r, monitor);
            t3[i].start();
        }
//join para teste
        for ( i=0; i<t1.length; i++) {
            try { t1[i].join(); } catch (InterruptedException e) { return; }
        }
        for ( i=0; i<t2.length; i++) {
            try { t2[i].join(); } catch (InterruptedException e) { return; }
        }
        for ( i=0; i<t3.length; i++) {
            try { t3[i].join(); } catch (InterruptedException e) { return; }
        }
       System.out.println("valor final da variavel central: "+ r.get());

    }
}