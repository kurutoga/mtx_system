int prints(char *s) {
    while (*s) {
        putc(*(s++));
    }
}

int gets(char s[]) {

    char c;
    int  len=0;
    while ((c=getc())!='\r') {
            s[len++]=c;
            putc(c);
    }
    s[len]=0;

}

main() {

    char name[64];
    while(1) {
        prints("What's your name?");
        gets(name);
        if (name[0]==0)
            break;
        prints("Welcome ");
        prints(name);
        prints("\n\r");
    }
    prints("return to assembly and hang \n\r");
}


