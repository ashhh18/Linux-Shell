#include "prompt.h"

void prompt(struct PWD *pwd, struct commandTime *timePrompt)
{
    char promptText[MAX_DIRECTORY_LENGTH];
    strcpy(promptText, "<");
    strcat(promptText, pwd->userName);
    strcat(promptText, "@");
    strcat(promptText, pwd->systemName);
    strcat(promptText, ":");
    makePromptText(promptText, pwd->currentDirectory, pwd->homeDirectory, timePrompt);
    if (timePrompt->timeToShow)
    {
        strcat(promptText, " ");
        strcat(promptText, timePrompt->commandName);
        strcat(promptText, " : ");

        char time[50];
        sprintf(time, "%d", timePrompt->timeTaken);
        strcat(promptText, time);
        strcat(promptText, "s");
    }

    strcat(promptText, ">");

    printf(ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET, promptText);
}

void makePromptText(char *promptText, char *curDirectory, char *homeDirectory, struct commandTime *timePrompt)
{
    int isPreString = 1;
    char formattedDirectory[strlen(curDirectory)];
    int l = strlen(homeDirectory);
    int n = strlen(curDirectory);
    for (int i = 0; i < l; i++)
    {
        if (n < l)
        {
            isPreString = 0;
            break;
        }
        if (curDirectory[i] != homeDirectory[i])
        {
            isPreString = 0;
            break;
        }
    }
    if (isPreString)
    {
        strcpy(formattedDirectory, "~");
        strcat(formattedDirectory, curDirectory + l);
    }
    else
    {
        strcpy(formattedDirectory, curDirectory);
    }

    strcat(promptText, formattedDirectory);
}
