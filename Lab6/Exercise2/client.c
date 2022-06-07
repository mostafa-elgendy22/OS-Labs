#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


const char* key_file = "keyfile";   //the file used in the function ftok

union semun
{
    int val;               
    struct semid_ds *buf;  
    ushort *array;         
    struct seminfo *__buf; 
    void *__pad;
};


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
    union semun sem_union;  //to be used in semctl
    

    //creating the shared memory segment or return its id if it is already created
    key_t key_id = ftok(key_file, 'M');
    int shm_id = shmget(key_id, 256, IPC_CREAT | 0666);
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
    int sem_id = semget(key_id, 3, IPC_CREAT | 0666);
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
    while (1)
    {
        printf("Enter the text to be reversed: ");
        fgets(text, 256, stdin);            //get the text from the user

        down(sem_id, 0);                    //no two clients can write the text to the shared memory at the same time
        
        strcpy((char*)shm_addr, text);      //move the text to the shared memory
        up(sem_id, 2);                      //tell the server that there is text at the shared memory
        down(sem_id, 1);                    //wait until the server finishes reversing the text
        printf("%s\n", (char*)shm_addr);    //print the reversed text

        up(sem_id, 0);                      //another client can write the text to the shared memory after this is executed
    }


    return 0;
}