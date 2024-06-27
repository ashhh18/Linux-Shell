#ifndef __PWD_H
#define __PWD_H
#define IGNORE_REDI

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

#define MAX_DIRECTORY_LENGTH 10000
#define MaxSystemLength 100

#define ANSI_COLOR_BLACK "\x1b[30m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_WHITE "\x1b[37m"
#define ANSI_COLOR_RESET "\x1b[0m"

struct PWD{
    int depth;
    char* currentDirectory;
    char *prevDirectory;
    char* homeDirectory;
    char* userName;
    char* systemName;
};

void initPWD(struct PWD* p);
void addDirectory(struct PWD* p, char* directory);
void delDirectory(struct PWD* p);

extern int curForeground;

#endif
