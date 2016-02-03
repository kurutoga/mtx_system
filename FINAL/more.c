#include "ucode.c"
#include "util.c"

int main(char *argc, char *argv[])
{
    char buf[256], c;
    int count = 0, fd = 0, n = 0, eof = 0;
    _bzero(buf, 256);
    //getc();
    if(argc == 1)
    {
        fd = open("/dev/tty0", O_RDONLY);
        while(n = readline(buf, 256, 0, &eof)) {
             if(buf[0] == '~') 
                 exit(0);
             write(1, buf, n);
             putc('\r');
             _bzero(buf, 256);
             count++;
             if(count > 22) {
                read(fd, &c, 1);
                count = 0;
             }
             if(eof) break;
        }
        close(fd);
    }
    else { more(argv[1]); }
    exit(1);
}

int more(char *pathname) {
    char buf[1025], c;
    struct stat fs;
    int eof = 0, fd = 0, len = 0, lines = 0, tot = 0, n = 0;
    _bzero(buf[1025]);
    
    if(!pathname) 
        fd = 0;
    else 
        fd = open(pathname, O_RDONLY);
    if(fd < 0) 
        return -1;
    fstat(fd, &fs);
    len = fs.st_size;
    while(len > 0)     {
        lines = 0;
        tot = 0;
        while(lines < 23 && len > 0) {
            if(len >= 1024) {
                n = readline(buf, 1024, fd, &eof);
            } else {
                n = readline(buf, len, fd, &eof);
            }
            buf[n] = 0;
            write(1, buf, n);
            putc('\r');
            _bzero(buf, 1025);
            len -= n;
            tot += n;
            lines++;
            if(eof) { len = 0; break;}
        }
        if(len > 0) {getc(&c); }
    }
    if(pathname) {close(fd);}
    return 1;
}

