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
    fclose(inputFile);
    return totalGrades;
}


int main(int argc, char** argv)
{
    //This is the parent process (department comittee)

    char* fileName = argv[1];
    int N = atoi(argv[2]);
    int P = atoi(argv[3]);
    int S;
    int* totalGrades = readFile(fileName, &S);
    int* pids = malloc(N * sizeof(int));

    int start = -1 * (S / N), stop = -1;
    int i;
    for (i = 0; i < N; i++)                
    {
        start += (S / N);
        stop += (S / N);
        if (i == N - 1)
        {
            stop = S - 1;
        }
        pids[i] = fork();            //creating N children processes 

        if (pids[i] == -1)
        {
            perror("Error in forking process");
        }
        else if (pids[i] == 0)
        {
            int passedStudents = 0;
            for (int j = start; j <= stop; j++)
            {
                if (totalGrades[j] >= P)
                {
                    passedStudents++;
                }
            }
            exit(passedStudents);
        }
    }   

    int* numPassedStudents = malloc(N * sizeof(int));    //saving the number of reported passing students by each TA
    for (int i = 0; i < N; i++)
    {
        waitpid(pids[i], &numPassedStudents[i], 0);     //wait the children in order of creation
    }
    
    for (int i = 0; i < N; i++)
    {
        numPassedStudents[i] = numPassedStudents[i] >> 8;
        printf("%d ", numPassedStudents[i]);
    }
    printf("\n");

    free(pids);
    free(totalGrades);
    free(numPassedStudents);

    return 0;
}