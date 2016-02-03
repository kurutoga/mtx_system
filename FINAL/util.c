void _bzero(char *buf, int size) {
    int i;
    for(i = 0; i < size; i++)
        buf[i] = 0;
}

int readline(char *buf, int size, int file, int *eof)
{
    int i = 0;
    char c = 0;
    read(file, &c, 1);
    while(c != '\n' && c != 0 && i < size)
    {
        buf[i++] = c;
        read(file, &c, 1);
    }
    if(c == '\n' || c == '\r') {buf[i++] = '\n';}
    if(c == 0) { *eof = 1; }
    return i;
}

