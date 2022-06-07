#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

int main()
{
    int pid = fork();
    if (pid == 0)
    {
        sleep(2);
        printf("Child Awaken\n");
        exit(99);
    }
    else
    {
        int stat_loc;
        wait(&stat_loc);
        printf("%d\n", stat_loc >> 8);
    }
}