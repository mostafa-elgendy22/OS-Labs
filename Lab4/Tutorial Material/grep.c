#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main()
{
    char bashCommand[] = "ps -e | grep a.out > /dev/null";
    int x = -1;
    printf("%d\n", x);
    x = system(bashCommand);
    printf("%d\n", x);
    

    // no output 256
    // output 0
    return 0;
}