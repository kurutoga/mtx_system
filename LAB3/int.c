#define AX 8
#define PA 13

int kcinth() {
    
    u16 segment, offset;
    int a,b,c,d,r;

    segment = running->uss;
    offset  = running->usp;

    a = get_word(segment, offset + 2*PA);
    b = get_word(segment, offset + 2*(PA+1));
    c = get_word(segment, offset + 2*(PA+2));
    d = get_word(segment, offset + 2*(PA+3));

    switch(a) {

        case 0:
            r = getpid();
            break;
        case 1:
            r = kps();
            break;
        case 2:
            k = kchname();
            break;
        case 3:
            r = kkfork();
            break;
        case 4:
            r = kswitch();
            break;
        case 5:
            r = kwait(b);
            break;
        case 6:
            kexit();
            break;
        default:
            printf("invalid syscall %d\n", a);
    }

    put_word(r, segment, offset + 2*AX);

}

