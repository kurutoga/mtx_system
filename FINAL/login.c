#include "ucode.c"
#include "util.c"

char *tty;
char user[64], pass[64], home[64], exe[64], buf[128];
char *temp;
int stdin, stdout, stderr, uid;

int main(int argc, char *argv[]) {
    
    tty = argv[1];
    close(0); close(1); close(2);

    stdin = open(tty, 0);
    stdout = open(tty, 1);
    stderr = open(tty, 2);

    settty(tty);

    signal(2,0);

    while(1)
    {
        printf("==Login==\n");
        printf("Username: ");
        gets(user);
        printf("Password: ");
        gets(pass);
        if(check_pass(user, pass)) {
            setuid(uid);
            chdir(home);
            exec(exe);
        }
        printf("Incorrect Username and/or password\n");
    }
}

int check_pass(char *u, char *p) {
    struct stat fs;
    int len, n;
    char _uid[32];
    int pfile = open("/etc/passwd", O_RDONLY);
    if(!pfile) {printf("Error opening file\n"); return 0;}

    fstat(pfile, &fs);
    len = fs.st_size;
    _bzero(buf, 128);
    while(len > 0) {
        if(len >= 128) 
            n = readline(buf, len, pfile,0);
        else 
            n = readline(buf, len, pfile,0);
        len -= n;
        temp = strtok(buf, ":");
        if(strcmp(temp, u) == 0)
        {
            temp = strtok(0, ":");
            if(strcmp(temp, p) == 0)
            {
                temp = strtok(0, ":");
                strcpy(_uid, temp); // uid
                uid = atoi(_uid);
                temp = strtok(0, ":");
                temp = strtok(0, ":");
                temp = strtok(0, ":");
                strcpy(home, temp); // home
                temp = strtok(0, ":");
                strcpy(exe, temp);
                return 1;
            }
        }
        _bzero(buf, 128);
    }
    close(pfile);
    return 0;
}
