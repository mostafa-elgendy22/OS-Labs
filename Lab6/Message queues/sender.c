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
    int msgq_id, send_val;

    key_id = ftok("keyfile", 65);
    msgq_id = msgget(key_id, 0666 | IPC_CREAT);

    if (msgq_id == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    printf("Message Queue ID = %d\n", msgq_id);

    char str[] = "\nMessage from sender to receiver..\n";
    struct msgbuff message;

    message.mtype = 7; /* arbitrary value */
    strcpy(message.mtext, str);

    send_val = msgsnd(msgq_id, &message, sizeof(message.mtext), !IPC_NOWAIT);

    if (send_val == -1)
        perror("Errror in send");

    return 0;
}
