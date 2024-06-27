#ifndef __PROMPT_H
#define __PROMPT_H


#include <stdio.h>
#include "pwd_.h"
#include "timetaken.h"


void prompt(struct PWD *pwd, struct commandTime *timePrompt);
void makePromptText(char *promptText, char *curDirectory, char *homeDirectory, struct commandTime *timePrompt);

#endif