#include "ucode.c"

int pid, child[3], status;
int stdin, stdout;

int main(int argc, char *argv[])
{
    stdin = open("/dev/tty0", O_RDONLY);
    stdout = open("/dev/tty0", O_WRONLY);

    printf("INIT : fork a login task on console\n");
    child[0] = fork();
    if(!child[0]){ exec("login /dev/tty0"); }
    
    parent();
}

int parent()
{
    while(1) 
    {
        printf("INIT : waiting ....\n");
        pid = wait(&status);
        if(pid == child[0]) {
            child[0] = fork();
            if(!child[0]) 
                exec("login /dev/tty0");
        }
        else if(pid == child[1]) {
            child[1] = fork();
            if(!child[1]) 
                exec("login /dev/ttyS0");
        }
        else if(pid == child[2]) {
            child[2] = fork();
            if(!child[2]) 
                exec("login /dev/ttyS1");
        }
        else
            printf("INIT : buried an orphan child %d\n", pid);
    }
}
