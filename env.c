#include <stdio.h>

//main function 
int main(int argc, char *argv[], char * envp[])
{
    int i=0;
    while (envp[i++]!=NULL)
    {
        printf("\n%s", envp[i]);
    }
    return 0;
}