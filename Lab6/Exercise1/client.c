#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

const char* key_file = "keyfile";   //the file used in the function ftok

struct msgbuff
{
    long mtype;
    char mtext[256];
};

int main()
{
    //creating the up message queue or return its id if it is already created
    key_t key_id = ftok(key_file, 'U');
    int up_msgq = msgget(key_id, IPC_CREAT | 0666);
    if (up_msgq == -1)
    {
        perror("Error in creating up message queue");
        exit(-1);
    }

    //creating the down message queue or return its id if it is already created
    key_id = ftok(key_file, 'D');
    int down_msgq = msgget(key_id, IPC_CREAT | 0666);
    if (down_msgq == -1)
    {
        perror("Error in creating down message queue");
        exit(-1);
    }



    int client_id = getpid() % 10000;   //the last four digits of the client pid

    struct msgbuff message;
    message.mtype = client_id;
    while (1)
    {
        printf("Enter the text to be reversed: ");
        fgets(message.mtext, 256, stdin);       //get the message from the user

        msgsnd(up_msgq, &message, sizeof(message.mtext), !IPC_NOWAIT);  //send the message through the up queue

        msgrcv(down_msgq, &message, sizeof(message.mtext), client_id, !IPC_NOWAIT); //recieve the reversed message throught the down queue
        printf("%s\n", message.mtext);  //print the reversed message
    }
    

    return 0;
}
