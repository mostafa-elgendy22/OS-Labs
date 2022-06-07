#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

int up_msgq, down_msgq; //the id of the message queues (it is global so that the signal handler can remove it)

const char* key_file = "keyfile";   //the file used in the function ftok


void handler(int sigNum)    //SIGINT handler
{
    msgctl(up_msgq, IPC_RMID, (struct msqid_ds *)0);
    msgctl(down_msgq, IPC_RMID, (struct msqid_ds *)0);
    exit(0);
}

struct msgbuff
{
    long mtype;
    char mtext[256];
};

void reverse(char* msg, char* reversed) 
{
    for (int i = 0; i < strlen(msg) - 1; i++)
    {
        reversed[i] = msg[strlen(msg) - i - 2];
    }
    reversed[strlen(msg) - 1] = '\0';
}

int main()
{
    signal(SIGINT, handler);    //attach the SIGINT to the handler

    //creating the up message queue or return its id if it is already created
    key_t key_id = ftok(key_file, 'U');
    up_msgq = msgget(key_id, IPC_CREAT | 0666);
    if (up_msgq == -1)
    {
        perror("Error in creating up message queue");
        exit(-1);
    }

    //creating the down message queue or return its id if it is already created
    key_id = ftok("keyfile", 'D');
    down_msgq = msgget(key_id, IPC_CREAT | 0666);
    if (down_msgq == -1)
    {
        perror("Error in creating down message queue");
        exit(-1);
    }


    struct msgbuff message;
    char reversed_mtext[256];
    while (1)
    {
        msgrcv(up_msgq, &message, sizeof(message.mtext), 0, !IPC_NOWAIT);   //recieve the message from the client through the up queue
        reverse(message.mtext, reversed_mtext);     //reverse the message
        strcpy(message.mtext, reversed_mtext);      //copy the reversed message to the message buffer
        msgsnd(down_msgq, &message, sizeof(message.mtext), !IPC_NOWAIT);    //send the reversed text through the down queue
    }
    
    return 0;
}
