#include <stdio.h>
#include <malloc.h>
#include <string.h>


void lower_case(char* str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] >= 65 && str[i] <= 90)
        {
            str[i] = str[i] + 32;
        }
    }
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("Too few arguments..\n");
    }
    else if (argc > 3)
    {
        printf("Too many arguments..\n");
    }
    else
    {
        if (strlen(argv[1]) != strlen(argv[2]))
        {
            char* str1;
            char* str2;
            if (strlen(argv[1]) < strlen(argv[2]))
            {
                str1 = (char*) malloc(strlen(argv[2]) * sizeof(char));
                str2 = (char*) malloc(strlen(argv[1]) * sizeof(char));
                strcpy(str1, argv[2]);
                strcpy(str2, argv[1]);
            }
            else
            {
                str1 = (char*) malloc(strlen(argv[1]) * sizeof(char));
                str2 = (char*) malloc(strlen(argv[2]) * sizeof(char));
                strcpy(str1, argv[1]);
                strcpy(str2, argv[2]);
            }
            lower_case(str1);
            lower_case(str2);
            /*printf("%s\n%s\n", str1, str2);*/
            char isJKP = '0';
            int i = 0;
            int j = 0;
            while (i != strlen(str2))
            {
                while (j != strlen(str1))
                {
                    if (i < strlen(str2) && str2[i] == str1[j])
                    {
                        i++;
                    }
                    j++;
                }
                if (j == strlen(str1))
                {
                    break;
                }
            }

            if (i == strlen(str2))
            {
                isJKP = '1';
            }
            printf("%c\n", isJKP);
        }

        else
        {
           printf("0\n");
        }
    }
    return 0;
}