#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

struct my_msgbuf {
    long mtype;
    char mtext[200];
};


/*
 * read pipe
 * */
void *readfunc(void *arg)
{
    struct my_msgbuf recvbuf;

    int msqid_recv;
    key_t key_recv;
    if ((key_recv = ftok("kirk2.c", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((msqid_recv = msgget(key_recv, 0644 | IPC_CREAT)) == -1) {
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
        printf("server read: %s \n", recvbuf.mtext);
    }

    if (msgctl(msqid_recv, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }
}


int main(void)
{
    /**
     * server read pipe
     */
    pthread_t readThread;
    int res = pthread_create(&readThread, NULL, readfunc, NULL);
    if(res != 0) 
    {
        printf("create thread fail\n");
        return 0; 
    }

    /**
     * server write pipe
     */
    struct my_msgbuf sendbuf;
    int msqid_write;
    key_t key_write;

    if ((key_write = ftok("kirk.c", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((msqid_write = msgget(key_write, 0644 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Enter lines of text, ^D to quit:\n");
    sendbuf.mtype = 1; /* we don't really care in this case */
    while(fgets(sendbuf.mtext, sizeof sendbuf.mtext, stdin) != NULL) {
        int len = strlen(sendbuf.mtext);

        if (sendbuf.mtext[len-1] == '\n') sendbuf.mtext[len-1] = '\0';
        printf("server write: %s \n", sendbuf.mtext);
        if (msgsnd(msqid_write, &sendbuf, len+1, 0) == -1) /* +1 for '\0' */
            perror("msgsnd");
    }

    if (msgctl(msqid_write, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    return 0;
}
