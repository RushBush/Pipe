#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <sys/wait.h>

#define N 10

int main()
{
    int fd[2];

    int cnt=0;
    pipe(fd);
    char *name1=malloc(15*sizeof(char));
    char *name2=malloc(15*sizeof(char));

    char **args1=malloc(N*sizeof(char*));
    char **args2=malloc(N*sizeof(char*));

    char *token1,*token2;

    for (int i=0;i<N;i++)
    {
        args1[i]=malloc(15*sizeof(char));
        args2[i]=malloc(15*sizeof(char));
    }

    fgets(name1,15,stdin);
    strtok(name1,"\n");
    fgets(name2,15,stdin);
    strtok(name2,"\n");

    token1=strtok(name1," ");

    while(token1!=NULL)
    {
        strncpy(args1[cnt],token1,15);
        cnt++;
        token1=strtok(NULL," ");
    }

    args1[cnt]=NULL;
    cnt=0;

    token2=strtok(name2," ");

    while(token2!=NULL)
    {
        strncpy(args2[cnt],token2,15);
        cnt++;
        token2=strtok(NULL," ");
    }

    args2[cnt]=NULL;

    int pid=fork();

    if (pid==0)
    {
        close(fd[0]);
        dup2(fd[1],1);
        execv(args1[0],args1);
    }
    else
    {
        int ppid=fork();

        if (ppid==0)
        {
            close(fd[1]);
            dup2(fd[0],0);
            execv(args2[0],args2);
        }
        else
        {
            wait(NULL);
        }

    }


    free(name1);
    free(name2);

    for (int i=0;i<N;i++)
    {
        free(args1[i]);
        free(args2[i]);
    }

    free(args1);
    free(args2);

    return 0;
}
