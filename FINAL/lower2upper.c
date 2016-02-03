#include "ucode.c"
#include "util.c"

int main(char *argc, char *argv[])
{
    char buf[1025];
    int len = 0;
    struct stat fs;
    int fdin, fdout, left, eof = 0, n = 0;

    if(argc == 1)    {
        fstat(0, &fs);
        if(fs.st_mode & 0010000) {
            // Important: FIFO (PIPE)
            while(n = readline(buf, 256, 0, &eof)) {
                if(buf[0] == '~') {exit(0);}
                l2u(buf);
                write(1, buf, n);
                putc('\r');
                _bzero(buf, 256);
                if(eof) {putc(0); break;}
            }
        } else {
            while(gets(buf)) {
                if(buf[0] == '~') {exit(0);}
                l2u(buf);
                printf("%s\n", buf);
                _bzero(buf, 256);
            }
        }
    } else { 
        fdin = open(argv[1], O_RDONLY);
        if(fdin < 0) 
            return -1;
        if(argc == 3) {
            fdout = open(argv[2], O_WRONLY|O_CREAT); 
        } else { fdout = 1; }

        if(fdout < 0) { 
            close(fdin); return -1; 
        }

        fstat(fdin, &fs);
        left = fs.st_size;
        _bzero(buf, 1025);
        while(left > 0) {
            if(left >= 1024) {
                n = readline(buf, 1024, fdin, &eof);
            } else {
                n = readline(buf, left, fdin, &eof);
            }
            buf[n+1] = 0;
            l2u(buf);
            write(fdout, buf, n);
            if(argc < 3) {putc('\r');}
            _bzero(buf, 1025);
            left -= n;
        }
        close(fdin);
        if(argc == 3) 
            close(fdout);
    }
    exit(0);
}


int l2u(char *buf)
{
    int i, len;
    len = strlen(buf);
    for(i = 0; i < len; i++) {
        if (((int)buf[i] > 96) && ((int)buf[i] < 123)) //lower case
        buf[i] -= 32;
    }
    return 1;
}

