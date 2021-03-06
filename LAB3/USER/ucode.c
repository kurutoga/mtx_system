char *cmd[] = {"getpid", "ps", "chname", "kfork", "switch", "wait", "exit", 0};

int show_menu() {

    printf("****************** MENU *******************\n");
    printf("  ps  chname  kfork  switch  wait  exit    \n");
    printf("*******************************************\n");

}

int find_cmd(char *name) {

    int i=0;
    char *p=cmd[0];
    while(p) {
        if (strcmp(p, name)==0)
            return i;
        i++;
        p = cmd[i];
    }
    return -1;

}

int getpid() {

    return syscall(0,0,0);

}

int ps() {

    return syscall(1,0,0);

}

int chname() {

    char s[64];
    printf("input new name: ");
    gets(s);
    syscall(2, s, 0);

}

int kfork() {

    int pid;
    printf("proc %d enter a kernel to kfork a child\n", getpid());
    pid = syscall(3,0,0);
    if (pid>0)
        printf("proc %d kforked a child %d\n", getpid(), pid);
    else
        printf("kforked failed\n");

}

int kswitch() {

    printf("proc %d enter kernel to switch process\n", getpid());
    syscall(4,0,0);
    printf("proc %d back from Kernel\n", getpid());

}

int wait() {

    int child, exitValue;
    printf("proc %d enter kernel to wait for a child to die\n" getpid());
    child = syscall(5, &exitValue, 0);
    printf("proc %d back from wait, dead child=%d", getpid(), child);
    if (child>=0)
        printf("exitValue=%d", exitValue);
    printf("\n");

}

int exit() {

    int exitValue;
    printf("\nenter an exitvalue (0-9) : ");
    exitValue = (getc()&0x7F) - '0';
    printf("enter kernel to die with exitValue=%d\n", exitValue);
    _exit(exitValue);

}

int _exit(int exitValue) {

    syscall(6,exitValue,0);

}

int invalid (char *name) {

    printf("invalid command: %s\n", name);

}
