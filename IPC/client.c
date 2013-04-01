#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <string.h>

struct my_msgbuf {
    long mtype;
    char mtext[200];
};

/*
 * write pipe
 * */
void *writefunc(void *arg)
{
    struct my_msgbuf writebuf;
    int msqid_write;
    key_t key_write;
    if ((key_write = ftok("kirk2.c", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((msqid_write = msgget(key_write, 0644)) == -1) {
        perror("msgget");
        exit(1);
    }
    
    while(fgets(writebuf.mtext, sizeof writebuf.mtext, stdin) != NULL)      {
        int len = strlen(writebuf.mtext);
        if (writebuf.mtext[len-1] == '\n') writebuf.mtext[len-1] = '\0';
        if (msgsnd(msqid_write, &writebuf, len+1, 0) == -1) 
            perror("msgsnd");
        

        printf("client write: %s\n", writebuf.mtext);
     }

    if (msgctl(msqid_write, IPC_RMID, NULL) == -1) {
            perror("msgctl");
            exit(1);
    }    
}

int main(void)
{
    /**
     * client write pipe
     */
    pthread_t writeThread;
    int res = pthread_create(&writeThread, NULL, writefunc, NULL);
    if(res != 0) 
    {
        printf("create thread fail\n");
        return 0; 
    }
    
    /**
     * client read pipe
     */
    struct my_msgbuf recvbuf;
    int msqid_recv;
    key_t key_recv;

    if ((key_recv = ftok("kirk.c", 'B')) == -1) {  /* same key_recv as kirk.c */
        perror("ftok");
        exit(1);
    }

    if ((msqid_recv = msgget(key_recv, 0644)) == -1) { /* connect to the queue */
        perror("msgget");
        exit(1);
    }


    for(;;) { 
        if (msgrcv(msqid_recv, &recvbuf, sizeof(recvbuf.mtext), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        int len = strlen(recvbuf.mtext);
        if (recvbuf.mtext[len-1] == '\n') recvbuf.mtext[len-1] = '\0';
        printf("client read: %s\n", recvbuf.mtext);
    }

    return 0;
}

