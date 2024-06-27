#ifndef __EXECUTE_H
#define __EXECUTE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <dirent.h>
#include <getopt.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <signal.h>
#include "fcntl.h"

#include "pwd_.h"
#include "container.h"
#include "timetaken.h"
#include "neonate.h"
#include "iman.h"

#define OUTPUT_LIMIT 4095
#define INPUT_LIMIT 4095
#define EVENT_FILE_PATH "%s/Events.txt", pwd->homeDirectory
#define FILETYPE 8
#define DIRTYPE 4

void execute(char *command, struct PWD *pwd, Container *backgroundProcess, struct commandTime *timePrompt, Container* activities);
void actualExecute(char *command, struct PWD *pwd, struct commandTime *timePrompt, Container *backgroundProcess, Container* activities);
void andExecute(char *command, Container *backgroundProcess, struct PWD *pwd, Container* activities);
char* parsePath(char *path, char *homeDirectory, char *parsedPath, struct PWD *pwd);
int compareEntries(const struct dirent **a, const struct dirent **b);
int peekExecute(char *command, struct PWD *pwd, struct commandTime *timePrompt);
void printFileInfo(const char *filename, const struct stat *info);
void procloreExecute(char *command, struct PWD *pwd, struct commandTime *timePrompt);
int pastEventsExecute(char *command, struct PWD *pwd, struct commandTime *timePrompt);
int storeEvents(char *command, char **seperatedCommands, int numSeperatedCommands, struct PWD *pwd);
int correctEvents(char *command, struct PWD *pwd);
int seekExecute(char *command, struct PWD *pwd, struct commandTime *timePrompt);
int seekRecurse(char *curPath, char *ToFind, int dflag, int fflag, int eflag, int *found, int *foundType, char *foundFile);
void redirectionExecute(char *command, struct PWD *pwd, struct commandTime *timePrompt, Container *backgroundProcess, Container* activities);
void freeStringArr(char **s, int l);
int activityExecute(char *command, struct PWD *pwd, Container *backgroundProcess, struct commandTime *timePrompt, Container* activities);
int compareStrings(const void *a, const void *b);
int pingExecute(char *command, struct PWD *pwd, Container *backgroundProcess, struct commandTime *timePrompt, Container *activities);
int fgbgExecute(char *command, struct PWD *pwd, Container *backgroundProcess, struct commandTime *timePrompt, Container *activities);


#endif