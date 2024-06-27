#ifndef __CONTAINER_H
#define __CONTAINER_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "pwd_.h"

#define maxContain 1000

typedef struct Container
{
    int idContainer[maxContain];
    char *stringContainer[maxContain];
    int numContent;
} Container;

Container *initContainer();
void addContent(Container *c, int id, char name[]);
void remContent(Container *c, int id);
int checkCompletion(Container *backgroundProcess, struct PWD *pwd);

#endif