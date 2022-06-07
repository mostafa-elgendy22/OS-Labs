#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>


int shm_id;     //the shared memory segment id (it is global so that the signal handler can remove it)
int sem_id;     //the semaphore set id (it is global so that the signal handler can remove it)

const char* key_file = "keyfile";   //the file used in the function ftok

union semun
{
    int val;               
    struct semid_ds *buf;  
    ushort *array;         
    struct seminfo *__buf; 
    void *__pad;
};

void handler(int sigNum)    //SIGINT handler
{
    shmctl(shm_id, IPC_RMID, (struct shmid_ds *)0);
    semctl(sem_id, 0, IPC_RMID, (struct semid_ds *)0);
    exit(0);
}

void reverse(char* text, char* reversed) 
{
    for (int i = 0; i < strlen(text) - 1; i++)
    {
        reversed[i] = text[strlen(text) - i - 2];
    }
    reversed[strlen(text) - 1] = '\0';
}

void down(int sem_id, int sem_num)
{
    struct sembuf d_op;

    d_op.sem_num = sem_num;
    d_op.sem_op = -1;
    d_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem_id, &d_op, 1) == -1)
    {
        perror("Error in down operation");
        exit(-1);
    }
}

void up(int sem_id, int sem_num)
{
    struct sembuf u_op;

    u_op.sem_num = sem_num;
    u_op.sem_op = 1;
    u_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem_id, &u_op, 1) == -1)
    {
        perror("Error in up operation");
        exit(-1);
    }
}

int main()
{
    signal(SIGINT, handler);    //attach the SIGINT to the handler
    
    union semun sem_union;      //to be used in semctl


    //creating the shared memory segment or return its id if it is already created
    key_t key_id = ftok(key_file, 'M');
    shm_id = shmget(key_id, 256, IPC_CREAT | 0666);
    if (shm_id == -1)
    {
        perror("Error in creating shared memory segment");
        exit(-1);
    }
    void* shm_addr = shmat(shm_id, (void *)0, 0);


    //creating a semaphore set which contains three semaphores or return its id if it is already created
    //index 0: semaphore for synchronization between multiple clients so that only one client write the text in the shared memory
    //index 1: it is released (using up operation) after the server finishes moving the reversed text to the shared memory
    //index 2: it is released (using up operation) after the client finishes moving the text to the shared memory
    key_id = ftok(key_file, 'S');
    sem_id = semget(key_id, 3, IPC_CREAT | 0666);
    if (sem_id == -1)
    {
        perror("Error in creating semaphore");
        exit(-1);
    }


    //initializing the three semaphores
    for (int i = 0; i < 3; i++)
    {
        if (i == 0)
        {
            sem_union.val = 1;
        }
        else
        {
            sem_union.val = 0;
        }   
        if (semctl(sem_id, i, SETVAL, sem_union) == -1)
        {
            perror("Error while setting the value of the semaphore");
            exit(-1);
        }
    }

    
    char text[256];
    char reversed_text[256];
    while (1)
    {
        down(sem_id, 2);                  //wait until there is text at the shared memory
        strcpy(text, (char*)shm_addr);   //get the text from the shared memory
        reverse(text, reversed_text);   //reverse the text
        strcpy((char*)shm_addr, reversed_text); //move the reversed text to the shared memory
        up(sem_id, 1);                  //tell the client that the text is reversed
    }

    return 0;
}