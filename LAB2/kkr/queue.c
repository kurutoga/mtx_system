int enqueue(PROC **queue, PROC *p) {

    PROC *tmp, *q;
    if (*queue==0 || (*queue)->priority<p->priority) {
        p->next=*queue;
        *queue=p;
        return;
    }
    q=*queue;
    tmp=(*queue)->next;
    while(q->priority>=p->priority) {
        if (tmp==0 || tmp->priority<p->priority) {
            q->next=p;
            p->next=tmp;
            return;
        }
        q=q->next;
        tmp=tmp->next;
    }    
}

PROC* dequeue(PROC **queue) {

    PROC *p=*queue;
    *queue=(*queue)->next;
    return p;

}

int printq(PROC **queue) {

    PROC *p=*queue;
    while (p) {
        myprintf("(%d, %d) -> ", p->pid, p->priority);
        p=p->next;
    }
    myprintf("NULL\n");

}


