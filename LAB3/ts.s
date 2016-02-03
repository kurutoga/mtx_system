DATASEG  = 0x1000
    
.globl _main,_running,_scheduler
.globl _proc, _procSize
.globl _tswitch
                
jmpi   start,MTXSEG

start: mov  ax,cs
       mov  ds,ax
       mov  ss,ax
       mov  es,ax
       mov  sp,#_proc
       add  sp,_procSize
                                                
       call _main

_tswitch:
  SAVE: push ax
        push bx
        push cx
        push dx
        push bp
        push si
        push di
        pushf
        mov  bx,_running
        mov  2[bx],sp

FIND:   call _scheduler

RESUME:
        mov  bx,_running
        mov  sp,2[bx]
        popf
        pop  di
        pop  si
        pop  bp
        pop  dx
        pop  cx
        pop  bx
        pop  ax

        ret

USS = 4
USP = 6

_int80h:
        push bp
        push si
        push di
        push es
        push ds

        push cs
        pop  ds
        mov  si, _running
        mov  USS[si], ss
        mov  USP[si], sp

        mov  di, ds
        mov  es, di
        mov  ss, di

        mov  sp, _running
        mov  sp, _procSize
        
        call _kcinth
        jmp _goUmode

_goUmode:
        cli
        mov bx, _running
        mov cx, USS[bx]
        mov ss, cx
        mov sp, USP[bx]

        pop ds
        pop es
        pop di
        pop si
        pop bp

        iret


