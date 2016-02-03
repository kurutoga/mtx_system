PROC *kfork(char *filename) {

}

int body() {

    char c;
    printf("proc %d resumes to body()\n", running->pid);
    while (1) {

        printf("--------------------------------------\n");
        printq(&freeList);
        printq(&readyQueue);
        printf("--------------------------------------\n");

        printf("proc %d running: parent=%d enter a char[s|f|w|q|u] : ",
                    running->pid, running->parent->pid);
        c = getc();
        printf("%c\n", c);
        switch(c) {

            case 's': 
                do_tswitch();
                break;
            case 'f':
                do_kfork();
                break;
            case 'w':
                do_wait();
                break;
            case 'q':
                do_exit();
                break;
            case 'u':
                goUmode();
                break;

        }
    }
}

int do_ps() {

}

int chname (char *newname) {

}


