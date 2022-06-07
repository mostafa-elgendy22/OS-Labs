#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct msgbuff
{
    long mtype;
    char mtext[70];
};

int main()
{
    key_t key_id;
    int rec_val, msgq_id;

    key_id = ftok("keyfile", 65);               //create unique key
    msgq_id = msgget(key_id, 0666 | IPC_CREAT); //create message queue and return id

    if (msgq_id == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    printf("Message Queue ID = %d\n", msgq_id);

    struct msgbuff message;
    while(1)
    {
        /* receive all types of messages */
        rec_val = msgrcv(msgq_id, &message, sizeof(message.mtext), 0, !IPC_NOWAIT);

        if (rec_val == -1)
            perror("Error in receive");
        else
            printf("\nMessage received: %s\n", message.mtext);
    }

    msgctl(msgq_id, IPC_RMID, (struct msqid_ds *)0);

    return 0;
}
