.globl _main
.globl _getc, _putc

mov ax, #0x9000
mov es, ax

xor bx, bx

xor dx, dx
xor cx, cx
incb cl
mov ax, #0x0202
int 0x13

jmpi start, 0x9000

start:
    mov ax,cs
    mov ds,ax
    mov ss,ax
    mov es,ax
    mov sp,#8192

    mov ax,#0x0012
    int 0x10

    call _main
    jmpi 0,0x1000


_getc:
    xorb ah,ah
    int 0x16
    ret

_putc:
    push bp
    mov bp,sp

    movb al,4[bp]
    movb ah,#14
    movb bl,#0x0D
    int 0x10

    pop bp
    ret


