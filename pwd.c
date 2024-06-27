#include "pwd_.h"

void initPWD(struct PWD *p)
{

    p->currentDirectory = (char *)malloc(sizeof(char) * MAX_DIRECTORY_LENGTH);
    p->homeDirectory = (char *)malloc(sizeof(char) * MAX_DIRECTORY_LENGTH);
    p->prevDirectory = (char *)malloc(sizeof(char) * MAX_DIRECTORY_LENGTH);
    strcpy(p->prevDirectory, "");

    getcwd(p->homeDirectory, MAX_DIRECTORY_LENGTH);
    strcpy(p->currentDirectory, p->homeDirectory);
    for (int i = 0; p->currentDirectory[i] != '\0'; i++)
    {
        if (p->currentDirectory[i] == '/')
            p->depth++;
    }

    struct utsname sys_info;
    if (uname(&sys_info) != 0)
    {
        printf(ANSI_COLOR_RED);
        printf("Getting Username failed\n");
        printf(ANSI_COLOR_RESET);
        return;
    }

    p->userName = getlogin();

    p->systemName = (char *)malloc(sizeof(char) * MaxSystemLength);
    strcpy(p->systemName, sys_info.nodename);
}

void addDirectory(struct PWD *p, char *directory)
{
    if (p->currentDirectory[strlen(p->currentDirectory) - 1] != '/')
        strcat(p->currentDirectory, "/");
    strcat(p->currentDirectory, directory);
    p->depth++;
}

void delDirectory(struct PWD *p)
{
    if (strcmp(p->currentDirectory, "/home") == 0 || strcmp(p->currentDirectory, "/") == 0)
    {
        strcpy(p->currentDirectory, "/");
        return;
    }

    char *lastSlash = strrchr(p->currentDirectory, '/');

    if (lastSlash != NULL)
    {
        *lastSlash = '\0';
    }
    p->depth--;
}
