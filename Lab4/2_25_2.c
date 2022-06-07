#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>


int* readFile (char* fileName, int* S)
{
    FILE* inputFile = fopen(fileName, "r");

    fscanf(inputFile, "%d", S);
    int* totalGrades = malloc(*S * sizeof(int));

    for (int i = 0; i < *S; i++)
    {
        int midtermGrade, finalGrade;
        fscanf(inputFile, "%d", &midtermGrade);
        fscanf(inputFile, "%d", &finalGrade);
        totalGrades[i] = midtermGrade + finalGrade;
    }
    return totalGrades;
}

char* toString(int num)
{
    if (num == 0)
    {
        char* zero = malloc(1 * sizeof(char));
        zero[0] = 0 + '0';
        return zero;
    }
    int temp = num;
    int size = 0;
    while (temp != 0)
    {
        temp /= 10;
        size++;
    }
    char* str = malloc(size * sizeof(char));
    int remainder;
    for (int i = 0; i < size; i++)
    {
        remainder = num % 10;
        num /= 10;
        str[size - (i + 1)] = remainder + '0';
    }
    return str;
}

int main(int argc, char** argv)
{
    char* fullBinaryFile = argv[0];
    char* binaryFile;
    strcpy(binaryFile, fullBinaryFile + 2);
    char* fileName = argv[1];
    int N = atoi(argv[2]);
    int P = atoi(argv[3]);
    int S;
    int* totalGrades = readFile(fileName, &S);
    int mainPID = getpid();
    int* pids = malloc(N * sizeof(int));

    int start = -1 * (S / N), stop = -1;
    for (int i = 0; i < N; i++)
    {
        start += (S / N);
        stop += (S / N);
        if (i == N - 1)
        {
            stop = S - 1;
        }
        pids[i] = fork();
        if (pids[i] == 0)
        {
            int passedStudents = 0;
            for (int j = start; j <= stop; j++)
            {
                if (totalGrades[j] >= P)
                {
                    passedStudents++;
                }
            }
            if (i > 0)
            {
                char bashCommand[] = "ps -e -l | grep '^.*";
                strcat(bashCommand, toString(pids[i - 1]));
                strcat(bashCommand, " *");
                strcat(bashCommand, toString(mainPID));
                strcat(bashCommand, ".*");
                strcat(bashCommand, binaryFile);
                strcat(bashCommand, "$' > /dev/null");
                int indicator = -1;
                do
                {
                    indicator = system(bashCommand);
                } 
                while (indicator == 0);
            }
            printf("%d ", passedStudents);
            exit(0);
        }
    }   

    for (int i = 0; i < N; i++)
    {
        wait(NULL);
    }
    
    printf("\n");

    free(pids);
    free(totalGrades);

    return 0;
}