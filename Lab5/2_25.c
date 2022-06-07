#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>


int X;
int pids[2];

void handler(int sigNum)
{
    int stat_loc;
    int pid;

    pid = wait(&stat_loc);
    stat_loc = stat_loc >> 8;
 

    int i;
    for (i = 0; i < 2; i++)
    {
        if (pids[i] == pid)
        {
            break;
        }
    }
    
    printf("Child %d: Value %d found at position %d\n", i + 1, X, stat_loc);
    killpg(getpgrp(), SIGKILL);
    signal(SIGUSR1, handler);
}


int main(int argc, char** argv)
{
    signal(SIGUSR1, handler);

    X = atoi(argv[1]);

    int N = argc - 2;
    int* list = malloc(N * sizeof(int));
    for (int j = 0; j < N; j++)
    {
        list[j] = atoi(argv[j + 2]);
    }

    printf("I am the parent, PID = %d\n", getpid());
    

    int start = -1 * (N / 2), stop = 0;

    for (int i = 0; i < 2; i++)
    {
        start += N / 2;
        stop += (N / 2) - 1;
        if (i == 1)
        {
            stop = N - 1;
        }
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("Error in forking process\n");
        }
        else if (pids[i] == 0)
        {
            char* rank;
            switch (i)
            {
                case 0:
                    rank = "first";
                    break;
                
                case 1: 
                    rank = "second";
                    break;
            }

            printf("I am the %s child, PID = %d, PPID = %d\n", rank, getpid(), getppid());
            
            for (int j = start; j <= stop; j++)
            {
                if (list[j] == X)
                {
                    kill(getppid(), SIGUSR1);
                    exit(j);
                }
            }
            sleep(3);
            printf("Child %d terminates\n", i + 1);
            exit(0);
        }
    }
    sleep(5);
    for (int i = 0; i < 2; i++)
    {
        int pid, stat_loc;
        pid = wait(&stat_loc);
        printf("A child with pid %d terminated with exit code %x\n", pid, stat_loc >> 8);
    }
    printf("Value not found\n");



    free(list);
    return 0;
}

/*
Non - code questions:

1- The process that will execute the handler's code is the parent (main) process.
   This is because the first (or the second) child sends the signal to the parent 
   (using the system call 'kill'), so the parent is the process which handles the sent signal.



2- The signal is sent from the any of the children to the parent only when the element X is found at 
   a certain position and then the child sends this postion as an exit code. So, it is not possible 
   that the signal handler doesn't recieve any exit code (when a signal is sent to the parent, a system
   call 'exit' is executed).

*/