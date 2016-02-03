#include "ucode.c"

//path storage
char path[256];
// protypes
void _bzero(char* buf, int size);
void store_input(char* buf, char inputs[20][64]);
void ahandler(char inputs[20][64], char* command);
void do_cd(char* pathname);
void do_exit();
int  do_fork(char* buf);
char* is_redirect(char *buf, int* mode, char* filename);
void do_redirect(char* filename, int mode);
int  has_pipe(char *buf);
void do_pipe(char *buf);
void do_exec(char *buf);

//main function
int main (int argc, char *argv[]) {
    char buf[256], inputs[20][64], temp[256];
    int len;
  
    getcwd(path);

    while(1) {
        getcwd(path);
        _bzero(buf, 256);
        printf("%s> ", path);
        gets(buf);
        len = strlen(buf);
        strcpy(temp, buf);
        if (buf[len-1]=='\n')
            buf[len-1]=0;
        store_input(temp, inputs);
        ahandler(inputs, buf);
    }
	return 0;
}

void _bzero(char *buf, int size) {
    int i;
    for (i=0;i<size;i++)
        buf[i]=0;
}

void store_input(char *buf, char inputs[20][64]) {
    int count = 0, i = 0;
    char *str = 0;

    str = strtok(buf, " ");
    while(str) {
        strcpy(inputs[count], str);
        count++;
        str = strtok(0, " ");
    }
    inputs[count][0] = 0;
    return;
 }

 void ahandler(char inputs[20][64], char *command)
 {
    char *temp;
    if(strcmp(inputs[0], "cd") == 0) {
        do_cd(inputs[1]);
    } else if(strcmp(inputs[0], "exit") == 0) {
        do_exit();
    } else {
        do_fork(command);
    }
    return;
}

 void do_cd(char* pathname)
 {
    if(pathname)
        chdir(pathname);
    else
        chdir(path);
 }

 void do_exit()
 {
    exit(1);
 }

 int do_fork(char* buf)
 {
    int pid;
    int r = 0,i=0;

    pid = fork();

    if(pid < 0) {
        printf ("Could not fork child\n");
        exit(1);
    }

    if(pid) {   
        printf("Parent %d forks child %d \n", getpid(), pid);
        pid = wait(&r);
        printf("Parent %d found dead child %d\n", getpid(), pid);
    } else {
        if(has_pipe(buf))
            do_pipe(buf);
        else
            do_exec(buf);
    }
    return r;
 }

 char* is_redirect(char *buf, int* mode, char *filename)
 {
    int i = 1;
    while(!(*mode) && buf[i]) {
        if(buf[i] == '<') {
            *mode = 1;   //input!
        } else if(buf[i] == '>') {
            //output
            if(buf[i+1] == '>') 
                *mode = 3;
            else 
                *mode = 2;
        }
        if(*mode) {
            //set redir in input array to NULL
            buf[i-1] = 0;
            buf[i++] = 0;
            buf[i++] = 0;
            if(buf[i] == ' ') 
                buf[i++] = 0;
            strcpy(filename, buf + i);
        }
        i++;
    }
    return;
 }

 void do_redirect(char* filename, int mode) {
    switch(mode) {
        case 1: close(0);
                open(filename, O_RDONLY);
                break;
        case 2: close(1);
                open(filename, O_WRONLY|O_CREAT, 0644);
                break;
        case 3: close(1);
                open(filename, O_APPEND|O_WRONLY|O_CREAT, 0644);
                break;
        default: printf("Error\n");
                break;
    }
 }

int has_pipe(char *buf) {
    int i = 0;
    while(buf[i] != 0)
        if(buf[i++] == '|') 
            return 1;
    return 0;
}

int get_tail(char *buf)
{
    int i=0, j=0;
    char temp[256];
    strcpy(temp, buf);

    while(temp[i] != '|') 
        i++;
    i += 2;
    while(temp[i])
        buf[j++] = temp[i++];
    buf[j] = 0;
}

int get_head(char *buf)
{
    int i;
    while(buf[i] != '|') 
        i++;
    buf[i-1] = 0;
}

void do_pipe(char* buf)
{
   int pd[2];
   int pid;

   pipe(pd); // pipe create
   pid = fork();
   if(!pid) //child
   {
        get_head(buf);
        close(1);
        dup2(pd[1], 1);
        close(pd[0]);
        do_exec(buf);
    } else { //parent
        get_tail(buf);
        printf("Parent %d forks child %d \n", getpid(), pid);
        close(0);
        dup2(pd[0], 0);
        close(pd[1]);
        if(has_pipe(buf))
            do_pipe(buf);
        else {
            close(1);
            open("/dev/tty0", O_WRONLY);
            do_exec(buf);
        }
    } 
}

void do_exec(char *buf)
{
    int mode = 0;
    char filename[64];

    is_redirect(buf, &mode, filename);
    if(mode)
        do_redirect(filename, mode);
    exec(buf);
}

