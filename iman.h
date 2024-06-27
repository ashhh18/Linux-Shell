#ifndef __IMAN_H
#define __IMAN_H

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
#include <termios.h>
#include <ctype.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "pwd_.h"
#include "container.h"
#include "timetaken.h"
#include "fcntl.h"

#define OUTPUT_LIMIT 4095
#define INPUT_LIMIT 4095
#define EVENT_FILE_PATH "%s/Events.txt", pwd->homeDirectory
#define FILETYPE 8
#define DIRTYPE 4

int iManExecute(char *command, struct PWD *pwd, Container *backgroundProcess, struct commandTime *timePrompt, Container *activities);



#endif