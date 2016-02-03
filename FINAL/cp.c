#include "ucode.c"
#include "util.c"

int main(char *argc, char *argv[]) {
    do_cp(argv);
    exit(0);
}

int do_cp(char *paths[])
{
    int fd = 0, gd = 0, n = 0, len = 0;
    struct stat fs;
    char buf[1024];

    fd = open(paths[1], O_RDONLY);
    
    if(fd < 0)
        return -1;

    fstat(fd, &fs);
    len = fs.st_size;
    gd = open(paths[2], O_WRONLY|O_CREAT);
    if(gd < 0) {
        close(fd); 
        return -1; 
    }

    _bzero(buf, 1024);
    
    while(len > 0) {
        if(len >= 1024)
            n = readline(buf, 1024, fd, 0);
        else
            n = readline(buf, len, fd, 0);
        write(gd, buf, n);
        _bzero(buf, 1024);
        len -= n;
    }
    close(fd);
    close(gd);
   return 1; 
}


