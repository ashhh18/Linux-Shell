#include "container.h"

Container *initContainer()
{
    Container *C = (Container *)malloc(sizeof(Container));
    C->numContent = 0;
    for (int i = 0; i < maxContain; i++)
    {
        C->idContainer[i] = -1;
    }
    return C;
}

void addContent(Container *c, int id, char name[])
{
    for (int i = 0; i < c->numContent; i++)
    {
        if (c->idContainer[i] == id)
            return;
    }
    c->idContainer[c->numContent] = id;
    c->stringContainer[c->numContent] = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(c->stringContainer[c->numContent], name);
    c->numContent++;
}

void remContent(Container *c, int id)
{
    int n = c->numContent;
    int idx;
    for (int i = 0; i < n; i++)
    {
        if (c->idContainer[i] == id)
        {
            idx = i;
            break;
        }
    }
    for (int i = idx; i < n - 1; i++)
    {
        c->idContainer[i] = c->idContainer[i + 1];
        c->stringContainer[i] = (char *)realloc(c->stringContainer[i + 1], sizeof(char) * (strlen(c->stringContainer[i + 1] + 1)));
    }
    c->numContent--;
    // free(c->stringContainer[c->numContent]);
}

int checkCompletion(Container *backgroundProcess, struct PWD *pwd)
{
    for (int i = 0; i < backgroundProcess->numContent; i++)
    {
        int status;
        int completed = waitpid(backgroundProcess->idContainer[i], &status, WNOHANG);
        if (completed)
        {

            char fileName[4095];
            sprintf(fileName, "%s/%dtemp.txt", pwd->homeDirectory, backgroundProcess->idContainer[i]);
#ifndef IGNORE_REDIRECT
            FILE *file = fopen(fileName, "r");

            if (file != NULL)
            {
                char buffer[1000];

                while (fgets(buffer, 1000, file) != NULL)
                {
                    printf("%s", buffer);
                }
                fclose(file);
                remove(fileName);
            }
#endif

            printf(ANSI_COLOR_RED);
            if (WIFEXITED(status))
                printf("%s with pid %d exited normally\n", backgroundProcess->stringContainer[i], backgroundProcess->idContainer[i]);
            else
                printf("%s with pid %d exited abnormally\n", backgroundProcess->stringContainer[i], backgroundProcess->idContainer[i]);
            printf(ANSI_COLOR_RESET);

            remContent(backgroundProcess, backgroundProcess->idContainer[i]);
            i--;
        }
    }
}
