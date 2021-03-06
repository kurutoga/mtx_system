/*********** MTX4.2 kernel t.c file **********************/
#define NPROC    9  
#define SSIZE 1024
/******* PROC status ********/
#define FREE     0
#define READY    1
#define STOP     2
#define DEAD     3
typedef struct proc{
    struct proc *next;
    int    *ksp;
    int    pid;              // add pid for identify the proc
    int    ppid;             // parent pid;
    int    status;           // status = FREE|READY|STOPPED|DEAD, etc
    int    priority;         // scheduling priority
    int    kstack[SSIZE];    // proc stack area
}PROC;

PROC proc[NPROC], *running, *freeList, *readyQueue;
int  procSize = sizeof(PROC);

#include "print.c"    // include I/O functions based on getc()/putc() 
#include "queue.c"

int body(int pid)
{
    char c; 
    myprintf("proc %d starts from body()\n", pid);
    while(1){
       printList(freeList);// optional: show the freeList
       printList(readyQueue); // show the readyQueue
       myprintf("proc %d running: parent=%d\n",running->pid,running->ppid);
       myprintf("enter a char [s|f] : ");
       c = getc(); myprintf("%c\n", c);

       switch(c){
            case 'f' : do_kfork();    break;
            case 's' : do_tswitch();  break;
       }
    }
}

PROC *kfork() // create a child process, begin from body()
{
    int i,*kk;
    PROC *p = get_proc(&freeList);
    if (!p){
        myprintf("no more PROC, kfork() failed\n");
        return 0;           
    }
    p->status = READY;
    p->priority = 1;         // priority = 1 for all proc except P0
    p->ppid = running->pid;          // parent = running
/* initialize new proc's kstack[ ] */ 
    for (i=1; i<10; i++)   {          // saved CPU registers
        p->kstack[SSIZE-i]= 0 ;      // all 0's  
    }
    p->kstack[SSIZE-2] = (int)body;  // resume point=address of body()
    p->ksp = &p->kstack[SSIZE-9];    // proc saved sp
    p->kstack[SSIZE-1] = (int)p->pid;
    enqueue(&readyQueue, p); // enter p into readyQueue by priority
    return p;                        // return child PROC pointer
}

int init()
{
    PROC *p; int i; int *loc;
    myprintf("init ....\n");
    
    for (i=0; i<NPROC; i++){     // initialize all procs
        p = &proc[i];
        p->pid = i;
        p->status = FREE;
        p->priority = 0;     
        p->next = &proc[i+1];
        myprintf("Init P%d with stack at %d %d %d\n", i, (int)(p->ksp), (int)(p->kstack[SSIZE]+2), (int)(p->pid));
    }

proc[NPROC-1].next = 0;
freeList = &proc[0];         // all procs are in freeList
readyQueue = 0;
/**** create P0 as running ******/
p = get_proc(&freeList);     // allocate a PROC from freeList
p->ppid = 0;                 // P0’s parent is itself
p->status = READY;
running = p;                 // P0 is now running
} 

int scheduler()
{
    if (running->status == READY)     // if running is still READY
        enqueue(&readyQueue, running); // enter it into readyQueue
    running = dequeue(&readyQueue);   // new running
}

main()
{
    myprintf("MTX starts in main()\n");
    init();                 // initialize and create P0 as running
    do_kfork();                // P0 creates child P1
    while(1){               // P0 switches if readyQueue not empty
        if (readyQueue)
            tswitch();
    }
}

/*************** kernel command functions ****************/
int do_kfork( )
{
    PROC *p = kfork();
    if (p == 0){ myprintf("kfork failed\n"); return -1; }
    myprintf("PROC %d kfork a child %d\n", running->pid, p->pid);
    return p->pid;
}

int do_tswitch(){  tswitch(); }
