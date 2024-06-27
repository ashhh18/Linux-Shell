#ifndef __TIME_H
#define __TIME_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ANSI_COLOR_BLACK "\x1b[30m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_WHITE "\x1b[37m"
#define ANSI_COLOR_RESET "\x1b[0m"

struct commandTime
{
    int timeTaken;
    int timeToShow;
    char commandName[30];
};

struct commandTime *initTime();

#endif