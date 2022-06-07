#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

/* arg for semctl system calls. */
union Semun
{
    int val;               /* value for SETVAL */
    struct semid_ds *buf;  /* buffer for IPC_STAT & IPC_SET */
    ushort *array;         /* array for GETALL & SETALL */
    struct seminfo *__buf; /* buffer for IPC_INFO */
    void *__pad;
};

void down(int sem)
{
    struct sembuf p_op;

    p_op.sem_num = 0;
    p_op.sem_op = -1;
    p_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &p_op, 1) == -1)
    {
        perror("Error in down()");
        exit(-1);
    }
}

void up(int sem)
{
    struct sembuf v_op;

    v_op.sem_num = 0;
    v_op.sem_op = 1;
    v_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &v_op, 1) == -1)
    {
        perror("Error in up()");
        exit(-1);
    }
}

void do_child(int sem1, int sem2)
{
    printf("A\n");
    up(sem1);
    up(sem1);
    up(sem1);
    up(sem1);
    sleep(10);
    printf("B\n");
    down(sem2);
    printf("C\n");
}

void do_parent(int sem1, int sem2)
{
    printf("G\n");
    down(sem1);
    printf("F\n");
    up(sem2);
    printf("E\n");
}

int main()
{
    int pid;

    union Semun semun;

    int sem1 = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    int sem2 = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);

    if (sem1 == -1 || sem2 == -1)
    {
        perror("Error in create sem");
        exit(-1);
    }

    semun.val = 0; /* initial value of the semaphore, Binary semaphore */
    if (semctl(sem1, 0, SETVAL, semun) == -1)
    {
        perror("Error in semctl");
        exit(-1);
    }
    if (semctl(sem2, 0, SETVAL, semun) == -1)
    {
        perror("Error in semctl");
        exit(-1);
    }

    pid = fork();

    if (pid == 0)
        do_child(sem1, sem2);
    else if (pid != -1)
        do_parent(sem1, sem2);
    else
    {
        perror("Error in fork");
        exit(-1);
    }
    return 0;
}
