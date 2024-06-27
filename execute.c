#include "execute.h"

void execute(char *command, struct PWD *pwd, Container *backgroundProcess, struct commandTime *timePrompt, Container *activities)
{
    fflush(stdout);
    fflush(stdin);
    // fprintf(stderr, ".Error %s.\n", command);

    char *copy10 = strdup(command);
    char *tokenn = strtok(copy10, " \t\n");
    if (tokenn == NULL)
        return;
    // put spaces in between ; and &
    correctEvents(command, pwd);
    char *cpyCommand = strdup(command);

    // put seperate commands in seperate lists
    char *seperatedCommands[INPUT_LIMIT];
    int numberOfCommands = 0;
    char delim[] = " \t\n";
    char *token = strtok(command, delim);
    while (token != NULL)
    {
        seperatedCommands[numberOfCommands] = (char *)malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(seperatedCommands[numberOfCommands], token);
        numberOfCommands++;
        token = strtok(NULL, delim);
    }

    // put command with arduments in a string and execute it
    int i = 0;
    char commandToExec[INPUT_LIMIT];
    strcpy(commandToExec, "");

    while (i <= numberOfCommands)
    {
        if (i == numberOfCommands || strcmp(seperatedCommands[i], ";") == 0)
        {
            if (strcmp(commandToExec, "") == 0)
            {
                i++;
                continue;
            }
            timePrompt->timeToShow = 0;

            commandToExec[strlen(commandToExec) - 1] = '\0';
            actualExecute(commandToExec, pwd, timePrompt, backgroundProcess, activities);
            strcpy(commandToExec, "");
        }

        else if (strcmp(seperatedCommands[i], "&") == 0)
        {
            timePrompt->timeToShow = 0;
            if (strcmp(commandToExec, "") == 0)
            {
                i++;
                continue;
            }
            commandToExec[strlen(commandToExec) - 1] = '\0';
            andExecute(commandToExec, backgroundProcess, pwd, activities);
            strcpy(commandToExec, "");
        }
        else
        {
            timePrompt->timeToShow = 0;

            strcat(commandToExec, seperatedCommands[i]);
            strcat(commandToExec, " ");
        }
        i++;
    }
    storeEvents(cpyCommand, seperatedCommands, numberOfCommands, pwd);
}

int storeEvents(char *command, char **seperatedCommands, int numSeperatedCommands, struct PWD *pwd)
{
    int shudAdd = 1;
    for (int i = 0; i < numSeperatedCommands; i++)
    {
        if (strcmp(seperatedCommands[i], "pastevents") == 0)
        {
            shudAdd = 0;
        }
    }
    char filePath[INPUT_LIMIT];
    sprintf(filePath, EVENT_FILE_PATH);

    if (shudAdd)
    {
        FILE *file = fopen(filePath, "a");
        fclose(file);
        file = fopen(filePath, "r");
        char *fileContent = (char *)malloc(sizeof(char) * INPUT_LIMIT);
        char *trackFileContent = fileContent;
        int bytesRead = fread(fileContent, 1, INPUT_LIMIT, file);
        fclose(file);
        if (bytesRead == 0)
        {
            strcpy(fileContent, command);
        }
        else
        {
            fileContent[bytesRead] = '\0';
            char *lastline = fileContent + strlen(fileContent);
            lastline--;
            lastline--;
            while (lastline[0] != '\n' && lastline != fileContent)
                lastline--;
            if (lastline != fileContent)
                lastline++;
            if (strcmp(lastline, command) != 0)
                strcat(fileContent, command);
        }
        int lineCount = 0;
        for (int i = 0; fileContent[i] != '\0'; i++)
        {
            if (fileContent[i] == '\n')
            {
                lineCount++;
            }
        }
        if (lineCount > 15)
        {
            char *secondLine = strchr(fileContent, '\n');
            fileContent = secondLine + 1;
        }
        file = fopen(filePath, "w");
        fprintf(file, "%s", fileContent);
        fclose(file);
        free(trackFileContent);
    }
}

int correctEvents(char *command, struct PWD *pwd)
{
    //
    char temCommand[strlen(command) + 1];
    strcpy(temCommand, command);
    strcpy(command, "\0");
    int lenTemCommand = strlen(temCommand);
    for (int i = 0; i < lenTemCommand; i++)
    {
        if ((temCommand[i] == '>' && temCommand[i + 1] == '>') && (i > 1 && temCommand[i - 1] != ' ') || (temCommand[i] == '>' && temCommand[i + 1] == '>') && (temCommand[i + 2] != ' '))
        {
            if ((temCommand[i] == '>' && temCommand[i + 1] == '>') && (i > 1 && temCommand[i - 1] != ' ') && (temCommand[i] == '>' && temCommand[i + 1] == '>') && (temCommand[i + 2] != ' '))
            {
                strcat(command, " >> ");
            }
            else if ((temCommand[i] == '>' && temCommand[i + 1] == '>') && (i > 1 && temCommand[i - 1] != ' '))
            {
                strcat(command, " >>");
            }
            else if ((temCommand[i] == '>' && temCommand[i + 1] == '>') && (temCommand[i + 2] != ' '))
            {
                strcat(command, ">> ");
            }
            i++;
            continue;
        }
        if (temCommand[i] == '>' && temCommand[i + 1] == '>')
        {
            strcat(command, ">>");
            i++;
            continue;
        }
        if ((temCommand[i] == ';' || temCommand[i] == '&' || temCommand[i] == '<' || temCommand[i] == '|' || temCommand[i] == '>') && (i > 0 && temCommand[i - 1] != ' '))
            strcat(command, " ");

        int n = strlen(command);
        command[n] = temCommand[i];
        command[n + 1] = '\0';

        if ((temCommand[i] == ';' || temCommand[i] == '&' || temCommand[i] == '<' || temCommand[i] == '|' || temCommand[i] == '>') && (temCommand[i + 1] != ' '))
            strcat(command, " ");
    }
    //
    char *seperatedCommands[INPUT_LIMIT];
    int numSeperatedCommands = 0;
    char delim[] = " \t\n";
    char *token = strtok(command, delim);
    while (token != NULL)
    {
        seperatedCommands[numSeperatedCommands] = (char *)malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(seperatedCommands[numSeperatedCommands], token);
        numSeperatedCommands++;
        token = strtok(NULL, delim);
    }
    //
    char *tempCommand = strdup(command);
    strcpy(command, "");

    for (int i = 0; i < numSeperatedCommands; i++)
    {
        if (strcmp(seperatedCommands[i], "pastevents") == 0 && i + 1 < numSeperatedCommands && strcmp(seperatedCommands[i + 1], "execute") == 0)
        {
            int lineToReplace = atoi(seperatedCommands[i + 2]);
            char filePath[INPUT_LIMIT];
            sprintf(filePath, EVENT_FILE_PATH);
            FILE *file = fopen(filePath, "a");
            fclose(file);
            file = fopen(filePath, "r");
            char fileContent[INPUT_LIMIT];
            int bytesRead = fread(fileContent, 1, INPUT_LIMIT, file);
            fclose(file);
            if (bytesRead == 0)
            {
                printf("pastevents: cannot execute\n");
                return 1;
            }
            int lineCount = 0;
            for (int i = 0; fileContent[i] != '\0'; i++)
            {
                if (fileContent[i] == '\n')
                {
                    lineCount++;
                }
            }
            if (lineCount < lineToReplace)
            {
                printf("pastevents: cannot execute\n");
                return 1;
            }
            lineToReplace = lineCount - lineToReplace + 1;
            lineCount = 1;
            for (int i = 0; fileContent[i] != '\0'; i++)
            {
                if (lineCount == lineToReplace)
                {
                    char *line = fileContent + i;
                    char *eol = strchr(line, '\n');
                    eol[0] = '\0';
                    strcat(command, line);
                    strcat(command, " ");
                    break;
                }
                if (fileContent[i] == '\n')
                {
                    lineCount++;
                }
            }
            i += 2;
        }
        else
        {
            strcat(command, seperatedCommands[i]);
            strcat(command, " ");
        }
    }
    command[strlen(command) - 1] = '\n';
}

void actualExecute(char *command, struct PWD *pwd, struct commandTime *timePrompt, Container *backgroundProcess, Container *activities)
{
    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] == '>' || command[i] == '<' || command[i] == '|')
        {
            redirectionExecute(command, pwd, timePrompt, backgroundProcess, activities);
            return;
        }
    }
    char commandChecker[strlen(command) + 1];
    strcpy(commandChecker, command);
    char *firstSpaceinCommand = strchr(commandChecker, ' ');
    if (firstSpaceinCommand != NULL)
        *firstSpaceinCommand = '\0';

    if (strcmp(commandChecker, "fg") == 0 || strcmp(commandChecker, "bg") == 0)
    {
        fgbgExecute(command, pwd, backgroundProcess, timePrompt, activities);
        return;
    }
    if (strcmp(commandChecker, "iMan") == 0)
    {
        iManExecute(command, pwd, backgroundProcess, timePrompt, activities);
        return;
    }
    if (strcmp(commandChecker, "neonate") == 0)
    {
        neonExecute(command, pwd, backgroundProcess, timePrompt, activities);
        return;
    }
    if (strcmp(commandChecker, "ping") == 0)
    {
        pingExecute(command, pwd, backgroundProcess, timePrompt, activities);
        return;
    }

    if (strcmp(commandChecker, "activities") == 0)
    {
        activityExecute(command, pwd, backgroundProcess, timePrompt, activities);
        return;
    }

    if (strcmp(commandChecker, "pastevents") == 0)
    {
        pastEventsExecute(command, pwd, timePrompt);
        return;
    }
    if (strcmp(commandChecker, "peek") == 0)
    {
        peekExecute(command, pwd, timePrompt);
        return;
    }
    if (strcmp(commandChecker, "seek") == 0)
    {
        seekExecute(command, pwd, timePrompt);
        return;
    }

    else if (strcmp(commandChecker, "proclore") == 0)
    {
        procloreExecute(command, pwd, timePrompt);
        return;
    }

    else if (strcmp(commandChecker, "cd") == 0 || strcmp(commandChecker, "warp") == 0)
    {
        timePrompt->timeToShow = 0;

        if (strcmp(commandChecker, "warp") == 0)
        {
            char temp[strlen(command) + 1];
            strcpy(temp, command);
            strcpy(command, "cd");
            command[3] = '\0';
            strcat(command, temp + 4);
        }
        char temprev[strlen(pwd->prevDirectory) + 1];
        strcpy(temprev, pwd->prevDirectory);
        char temcur[strlen(pwd->currentDirectory) + 1];
        strcpy(temcur, pwd->currentDirectory);

        if (strcmp(command, "cd -") == 0)
        {
            if (strcmp(pwd->prevDirectory, "") == 0)
            {
                printf(ANSI_COLOR_RED "Old PWD not set\n" ANSI_COLOR_RESET);
                return;
            }
            char tem[strlen(pwd->prevDirectory) + 1];
            strcpy(tem, pwd->prevDirectory);
            strcpy(pwd->prevDirectory, pwd->currentDirectory);
            strcpy(pwd->currentDirectory, tem);
        }
        else
        {
            strcpy(pwd->prevDirectory, pwd->currentDirectory);

            int n = strlen(command);
            char cdArg[INPUT_LIMIT];
            int cdArgLength = 0;
            strcpy(cdArg, command + 3);
            cdArgLength = strlen(cdArg);
            /////////////////////////////////////
            if (cdArgLength == 0)
            {
                strcpy(pwd->currentDirectory, pwd->homeDirectory);
            }

            else
            {
                char *individualArg[INPUT_LIMIT];
                int numIndividualArg = 0;
                char delim[] = " ";
                char *token = strtok(cdArg, delim);
                while (token != NULL)
                {
                    individualArg[numIndividualArg] = (char *)malloc(sizeof(char) * (strlen(token) + 1));
                    strcpy(individualArg[numIndividualArg], token);
                    numIndividualArg++;
                    // printf("%s\n", token);
                    token = strtok(NULL, delim);
                }
                for (int i = 0; i < numIndividualArg; i++)
                {
                    strcpy(pwd->prevDirectory, pwd->currentDirectory);

                    parsePath(individualArg[i], pwd->homeDirectory, pwd->currentDirectory, pwd);
                    if (chdir(pwd->currentDirectory) != 0)
                    {
                        printf(ANSI_COLOR_RED);
                        strcpy(pwd->currentDirectory, temcur);
                        strcpy(pwd->prevDirectory, temprev);
                        perror("chdir");
                        printf(ANSI_COLOR_RESET);
                    }
                    else
                        printf("%s\n", pwd->currentDirectory);
                }

                return;
            }
        }
        if (chdir(pwd->currentDirectory) != 0)
        {
            printf(ANSI_COLOR_RED);
            strcpy(pwd->currentDirectory, temcur);
            strcpy(pwd->prevDirectory, temprev);
            perror("chdir");
            printf(ANSI_COLOR_RESET);
        }
        else
            printf("%s\n", pwd->currentDirectory);

        return;
    }

    else if (strcmp(commandChecker, "exit") == 0)
    {
        for (int i = 0; i < backgroundProcess->numContent; i++)
        {
            char filename[OUTPUT_LIMIT];
            sprintf(filename, "%s/%dtemp.txt", pwd->homeDirectory, backgroundProcess->idContainer[i]);
            remove(filename);
        }
        exit(0);
    }

    timePrompt->timeToShow = 0;

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    char *args[] = {"bash", "-c", command, NULL};
    int pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            printf(ANSI_COLOR_RED);
            perror("Error executing command");
            printf(ANSI_COLOR_RESET);
            exit(EXIT_FAILURE);
        }
        exit(0);
    }
    fflush(stdout);
    int status;
    curForeground = pid;
    int completed = waitpid(pid, &status, WUNTRACED);
    curForeground = -1;

    // check if stat is T
    char fileName[OUTPUT_LIMIT];
    sprintf(fileName, "/proc/%d/stat", pid);
    FILE *f = fopen(fileName, "r");
    if (f != NULL)
    {
        char statFile[OUTPUT_LIMIT];
        fgets(statFile, OUTPUT_LIMIT, f);
        int count = 0;
        for (int i = 0; i < strlen(statFile); i++)
        {
            if (statFile[i] == ' ')
                count++;
            if (count == 2)
            {
                if (statFile[i + 1] == 'T')
                {
                    char delim[] = " \t\n";

                    char *token = strtok(strdup(command), delim);
                    addContent(backgroundProcess, pid, token);
                    printf("\n[%d] %d\n", backgroundProcess->numContent, pid);
                }

                break;
            }
        }
        fclose(f);
    }

    gettimeofday(&end_time, NULL);
    timePrompt->timeTaken = (int)(((end_time.tv_sec - start_time.tv_sec) +
                                   (end_time.tv_usec - start_time.tv_usec) / 1000000.0) +
                                  0.5);

    if (((end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0) >= 2)
    {
        timePrompt->timeToShow = 1;
        char *firstSpace = strchr(command, ' ');
        if (firstSpace != NULL)
            *firstSpace = '\0';
        strcpy(timePrompt->commandName, command);
    }
    fflush(stdout);
    addContent(activities, pid, command);
}

void procloreExecute(char *command, struct PWD *pwd, struct commandTime *timePrompt)
{
    int procArg = 0;
    char *procArgString;
    int givenPid;
    for (int i = 8; command[i] != '\0'; i++)
    {
        if (command[i] == ' ' || command[i] == '\t')
            continue;
        else
        {
            procArg = 1;
            procArgString = command + i;
            break;
        }
    }
    if (procArg)
    {
        givenPid = atoi(procArgString);
    }
    else
        givenPid = getpid();

    char processStatus[10] = "";
    int processGroup;
    char processVirtualMemomory[30] = "";
    char processExecPath[OUTPUT_LIMIT] = "";

    char procPath[50] = "";
    sprintf(procPath, "/proc/%d/status", givenPid);
    char buffer[OUTPUT_LIMIT];
    FILE *procFile = fopen(procPath, "r");
    if (procFile == NULL)
    {
        printf(ANSI_COLOR_RED);
        printf("Invalid PID\n");
        printf(ANSI_COLOR_RESET);
        return;
    }
    while (fgets(buffer, OUTPUT_LIMIT, procFile))
    {
        if (strncmp(buffer, "State:", 6) == 0)
        {
            processStatus[0] = buffer[7];
            processStatus[1] = '\0';
        }
        else if (strncmp(buffer, "VmSize:", 7) == 0)
        {
            sscanf(buffer, "VmSize: %s", processVirtualMemomory);
        }
    }
    fclose(procFile);

    sprintf(procPath, "/proc/%d/stat", givenPid);
    procFile = fopen(procPath, "r");
    (fgets(buffer, OUTPUT_LIMIT, procFile));
    int wordCount = 0;
    int runningGroup;
    char *token = strtok(buffer, " ");
    while (token != NULL)
    {
        if (wordCount == 7)
        {
            runningGroup = atoi(token);
        }
        wordCount++;
        token = strtok(NULL, " ");
    }
    if (runningGroup == givenPid)
    {
        processStatus[1] = '+';
        processStatus[2] = '\0';
    }

    processGroup = getpgid(givenPid);

    sprintf(procPath, "/proc/%d/exe", givenPid);
    readlink(procPath, processExecPath, OUTPUT_LIMIT);
    printf("pid : %d\n", givenPid);
    printf("process status : %s\n", processStatus);
    printf("Process Group : %d\n", processGroup);
    printf("Virtual Memory : %s\n", processVirtualMemomory);
    printf("executable Path : %s\n", processExecPath);
}

void andExecute(char *command, Container *backgroundProcess, struct PWD *pwd, Container *activities)
{
    char *args[] = {"bash", "-c", command, NULL};
    int pid = fork();
    if (pid == 0)
    {
        char fileName[INPUT_LIMIT];
        sprintf(fileName, "%s/%dtemp.txt", pwd->homeDirectory, getpid());
#ifndef IGNORE_REDIRECT
        freopen(fileName, "w", stdout);
#endif
        if (execvp(args[0], args) == -1)
        {
            printf(ANSI_COLOR_RED);
            perror("Error executing command");
            printf(ANSI_COLOR_RESET);
            exit(EXIT_FAILURE);
        }
        exit(0);
    }
    addContent(activities, pid, command);
    char delim[] = " \t\n";
    char *token = strtok(command, delim);
    addContent(backgroundProcess, pid, token);
    printf("[%d] %d\n", backgroundProcess->numContent, pid);
}

char *parsePath(char *path, char *homeDirectory, char *parsedPath, struct PWD *pwd)
{
    if (parsedPath == NULL)
    {
        parsedPath = (char *)malloc(sizeof(char) * INPUT_LIMIT);
        parsedPath[0] = '\0';
    }
    strcpy(parsedPath, pwd->currentDirectory);
    if (path[0] == '/')
    {
        strcpy(parsedPath, "/");
    }
    char delim[] = "/\n";
    char *token = strtok(path, delim);
    while (token != NULL)
    {
        if (strcmp(token, "..") == 0)
        {
            if (strcmp(token, "/home") == 0 || strcmp(token, "/") == 0)
            {
                strcpy(parsedPath, "/");
                continue;
            }

            char *lastSlash = strrchr(parsedPath, '/');

            if (lastSlash != NULL)
            {
                *lastSlash = '\0';
            }
        }
        else if (strcmp(token, ".") == 0)
        {
            strcpy(parsedPath, pwd->currentDirectory);
            token = strtok(NULL, delim);
            continue;
        }
        else if (strcmp(token, "~") == 0)
        {
            strcpy(parsedPath, homeDirectory);
        }
        else
        {
            if (parsedPath[strlen(parsedPath) - 1] != '/')
                strcat(parsedPath, "/");
            strcat(parsedPath, token);
        }
        token = strtok(NULL, delim);
    }
    char *ret = strdup(parsedPath);
    return ret;
}

int compareEntries(const struct dirent **a, const struct dirent **b)
{
    return strcmp((*a)->d_name, (*b)->d_name);
}

int peekExecute(char *command, struct PWD *pwd, struct commandTime *timePrompt)
{
    // handling no argument
    if (strcmp("peek", command) == 0)
        strcat(command, " .");
    // cheking for peek -
    if (strcmp(command, "peek") != 0)
    {
        char *lastSpaceDash = strchr(command, ' ');
        if (lastSpaceDash[1] == '-' && lastSpaceDash[2] == '\0')
        {
            command[strlen(command) - 1] = '\0';
            strcat(command, pwd->prevDirectory);
        }
    }

    // getiing argumnet and passing it into parser
    char peekArg[strlen(command) - 3];
    char *lastSpace = strrchr(command, ' ');
    if (lastSpace != NULL)
        strcpy(peekArg, lastSpace + 1);
    if (peekArg[0] == '-')
        peekArg[0] = '\0';
    char parsedPeekArg[INPUT_LIMIT];
    parsePath(peekArg, pwd->homeDirectory, parsedPeekArg, pwd);

    // using getopts
    int maxToken = 10;
    char *commandArr[maxToken];
    int numCommandArr = 0;
    char *delim = " \t";
    char *token = strtok(command, delim);
    while (token != NULL)
    {
        commandArr[numCommandArr] = strdup(token);
        numCommandArr++;
        token = strtok(NULL, delim);
    }
    int lFlag = 0, aFlag = 0;
    for (int i = 0; i < numCommandArr; i++)
    {
        if (commandArr[i][0] == '-')
        {
            if (commandArr[i][1] == 'l')
                lFlag = 1;
            if (commandArr[i][1] == 'a')
                aFlag = 1;
            if (commandArr[i][2] == 'l')
                lFlag = 1;
            if (commandArr[i][2] == 'a')
                aFlag = 1;
        }
    }

    // using dirent.h to scan directory
    struct dirent **filesInPath;
    int fileCount = scandir(parsedPeekArg, &filesInPath, NULL, compareEntries);
    if (fileCount == -1)
    {
        printf(ANSI_COLOR_RED);
        perror("scandir");
        printf(ANSI_COLOR_RESET);
        return EXIT_FAILURE;
    }

    if (!lFlag)
    {
        for (int i = 0; i < fileCount; i++)
        {
            if (filesInPath[i]->d_type == FILETYPE)
                printf(ANSI_COLOR_WHITE);
            chdir(parsedPeekArg);
            if (access(filesInPath[i]->d_name, X_OK) == 0)
                printf(ANSI_COLOR_GREEN);
            chdir(pwd->currentDirectory);
            if (filesInPath[i]->d_type == DIRTYPE)
                printf(ANSI_COLOR_BLUE);

            if (aFlag || filesInPath[i]->d_name[0] != '.')
            {
                printf("%s\n", filesInPath[i]->d_name);
            }
            free(filesInPath[i]);
            printf(ANSI_COLOR_RESET);
        }
    }
    else
    {
        for (int i = 0; i < fileCount; i++)
        {
            if (filesInPath[i]->d_type == FILETYPE)
                printf(ANSI_COLOR_WHITE);
            chdir(parsedPeekArg);
            if (access(filesInPath[i]->d_name, X_OK) == 0)
                printf(ANSI_COLOR_GREEN);
            chdir(pwd->currentDirectory);
            if (filesInPath[i]->d_type == DIRTYPE)
                printf(ANSI_COLOR_BLUE);
            if (aFlag || filesInPath[i]->d_name[0] != '.')
            {
                char pathToFilePrint[OUTPUT_LIMIT];
                pathToFilePrint[0] = '\0';
                strcpy(pathToFilePrint, parsedPeekArg);
                strcat(pathToFilePrint, "/");
                strcat(pathToFilePrint, filesInPath[i]->d_name);

                struct stat info;
                if (stat(pathToFilePrint, &info) == -1)
                {
                    perror("stat");
                    continue;
                }

                printFileInfo(filesInPath[i]->d_name, &info);

                free(filesInPath[i]);
            }
            printf(ANSI_COLOR_RESET);
        }
    }
}

int pastEventsExecute(char *command, struct PWD *pwd, struct commandTime *timePrompt)
{
    // get pastevent A B into seperate indices of array
    if (strcmp(command, "pastevents purge") == 0)
    {
        char filePath[INPUT_LIMIT];
        sprintf(filePath, EVENT_FILE_PATH);
        FILE *file = fopen(filePath, "w");
        fclose(file);
    }
    else
    {
        char filePath[INPUT_LIMIT];
        sprintf(filePath, EVENT_FILE_PATH);
        FILE *file = fopen(filePath, "r");
        char buffer[OUTPUT_LIMIT];
        int byte = fread(buffer, 1, OUTPUT_LIMIT, file);
        buffer[byte] = '\0';
        fclose(file);
        printf("%s", buffer);
    }
}

int seekExecute(char *command, struct PWD *pwd, struct commandTime *timePrompt)
{
    int dflag = 0, fflag = 0, eflag = 0;
    int diffDir = -1;
    char toFind[INPUT_LIMIT];
    char DIR[INPUT_LIMIT];
    int commandValid = 0;
    char *tok = strtok(command, " \t");
    while (tok != NULL)
    {
        if (strcmp(tok, "seek") == 0)
        {
            tok = strtok(NULL, " \t");
            continue;
        }
        else if (tok[0] == '-')
        {
            // commandValid = 1;
            if (tok[2] != '\0')
            {
                printf(ANSI_COLOR_RED);
                printf("Invalid flags!\n");
                printf(ANSI_COLOR_RESET);
                return 0;
            }
            if (tok[1] == 'd')
            {
                dflag = 1;
            }
            if (tok[1] == 'e')
            {
                eflag = 1;
            }
            if (tok[1] == 'f')
            {
                fflag = 1;
            }
        }
        else if (diffDir == 0)
        {
            commandValid = 1;
            diffDir = 1;
            strcpy(DIR, tok);
        }
        else
        {
            commandValid = 1;
            diffDir = 0;
            strcpy(toFind, tok);
        }

        tok = strtok(NULL, " \t");
    }
    if (!commandValid)
    {
        printf(ANSI_COLOR_RED);
        printf("Invalid Use of Seek\n");
        printf(ANSI_COLOR_RESET);
        return (EXIT_FAILURE);
    }
    if ((dflag == 1 && fflag == 1))
    {
        printf(ANSI_COLOR_RED);
        printf("Invalid flags!\n");
        printf(ANSI_COLOR_RESET);
        return 0;
    }
    char pasrsedDIR[INPUT_LIMIT];
    if (strcmp(DIR, "") == 0)
        strcpy(pasrsedDIR, pwd->currentDirectory);
    else
        parsePath(DIR, pwd->homeDirectory, pasrsedDIR, pwd);

    if (chdir(pasrsedDIR) != 0)
    {
        printf(ANSI_COLOR_RED);
        perror("chdir");
        printf(ANSI_COLOR_RESET);
        return 1;
    }

    char funcCurPath[OUTPUT_LIMIT];
    strcpy(funcCurPath, ".");
    int found = 0;
    int founfType = 0;
    char foundFile[OUTPUT_LIMIT];
    seekRecurse(funcCurPath, toFind, dflag, fflag, eflag, &found, &founfType, foundFile);

    if (found == 0)
    {
        printf(ANSI_COLOR_RED);
        printf("No match found!\n");
        printf(ANSI_COLOR_RESET);
    }
    if (chdir(pwd->currentDirectory) != 0)
    {
        printf(ANSI_COLOR_RED);
        perror("chdir");
        printf(ANSI_COLOR_RESET);
        return 1;
    }
    if (eflag == 1 && found == 1)
    {
        if (founfType == FILETYPE)
        {
            if (access(foundFile, R_OK) != 0)
            {
                printf(ANSI_COLOR_RED);
                printf("Missing permissions for task!\n");
                printf(ANSI_COLOR_RESET);
                return -1;
            }
            FILE *file = fopen(foundFile, "r");
            if (file == NULL)
            {
                printf(ANSI_COLOR_RED);
                perror("fopen");
                printf(ANSI_COLOR_RESET);
                return -1;
            }
            char buffer[50000];
            fread(buffer, 1, 50000, file);
            printf("%s", buffer);
            fclose(file);
        }
        else if (founfType == DIRTYPE)
        {
            if (access(foundFile, X_OK) != 0)
            {
                printf(ANSI_COLOR_RED);
                printf("Missing permissions for task!\n");
                printf(ANSI_COLOR_RESET);
                return -1;
            }
            if (chdir(foundFile) != 0)
            {
                printf(ANSI_COLOR_RED);
                perror("chdir");
                printf(ANSI_COLOR_RESET);
                return -1;
            }
        }
    }
    char buffer[OUTPUT_LIMIT];
    getcwd(buffer, OUTPUT_LIMIT);
    strcpy(pwd->prevDirectory, pwd->currentDirectory);
    strcpy(pwd->currentDirectory, buffer);
}

int seekRecurse(char *curPath, char *ToFind, int dflag, int fflag, int eflag, int *found, int *foundType, char *foundFile)
{
    // printf("%s\n", curPath);
    char curAbsDir[OUTPUT_LIMIT];
    getcwd(curAbsDir, OUTPUT_LIMIT);
    struct dirent **filesInPath;
    int fileCount = scandir(curAbsDir, &filesInPath, NULL, compareEntries);
    if (fileCount == -1)
    {
        perror("scandir");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < fileCount; i++)
    {
        if (strncmp(filesInPath[i]->d_name, ToFind, strlen(ToFind)) == 0)
        {
            if (!dflag && filesInPath[i]->d_type == FILETYPE || !fflag && filesInPath[i]->d_type == DIRTYPE)
            {
                if (filesInPath[i]->d_name[0] != '.')
                {
                    if (filesInPath[i]->d_type == FILETYPE)
                        printf(ANSI_COLOR_WHITE);
                    if (access(filesInPath[i]->d_name, X_OK) == 0)
                        printf(ANSI_COLOR_GREEN);
                    if (filesInPath[i]->d_type == DIRTYPE)
                        printf(ANSI_COLOR_BLUE);
                    printf("%s/%s\n", curPath, filesInPath[i]->d_name);
                    *found = *found + 1;
                    if (filesInPath[i]->d_type == FILETYPE)
                    {
                        *foundType = FILETYPE;
                        sprintf(foundFile, "%s/%s", curAbsDir, filesInPath[i]->d_name);
                    }
                    else
                    {
                        *foundType = DIRTYPE;
                        sprintf(foundFile, "%s/%s", curAbsDir, filesInPath[i]->d_name);
                    }
                    printf(ANSI_COLOR_RESET);
                }
            }
        }
        if (filesInPath[i]->d_type == DIRTYPE && filesInPath[i]->d_name[0] != '.')
        {
            char nextPath[INPUT_LIMIT];
            char nextAbsPath[INPUT_LIMIT + 1000];

            sprintf(nextPath, "%s/%s", curPath, filesInPath[i]->d_name);
            sprintf(nextAbsPath, "%s/%s", curAbsDir, filesInPath[i]->d_name);

            if (chdir(nextAbsPath) != 0)
            {
                perror("chdir");
                return -1;
            }
            seekRecurse(nextPath, ToFind, dflag, fflag, eflag, found, foundType, foundFile);
            if (chdir(curAbsDir))
            {
                perror("chdir");
                return -1;
            }
        }
        free(filesInPath[i]);
    }
}

void printFileInfo(const char *filename, const struct stat *info)
{
    struct passwd *pw = getpwuid(info->st_uid);
    struct group *gr = getgrgid(info->st_gid);
    char timebuf[80];

    printf((S_ISDIR(info->st_mode)) ? "d" : "-");
    printf((info->st_mode & S_IRUSR) ? "r" : "-");
    printf((info->st_mode & S_IWUSR) ? "w" : "-");
    printf((info->st_mode & S_IXUSR) ? "x" : "-");
    printf((info->st_mode & S_IRGRP) ? "r" : "-");
    printf((info->st_mode & S_IWGRP) ? "w" : "-");
    printf((info->st_mode & S_IXGRP) ? "x" : "-");
    printf((info->st_mode & S_IROTH) ? "r" : "-");
    printf((info->st_mode & S_IWOTH) ? "w" : "-");
    printf((info->st_mode & S_IXOTH) ? "x" : "-");
    printf(" %lu ", info->st_nlink);
    printf("%s %s ", pw->pw_name, gr->gr_name);
    printf("%ld ", (long)info->st_size);
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&info->st_mtime));
    printf("%s ", timebuf);
    printf("%s ", filename);
    printf("\n");
}

void redirectionExecute(char *command, struct PWD *pwd, struct commandTime *timePrompt, Container *backgroundProcess, Container *activities)
{
    // piping
    int l = strlen(command);
    int pipeExist = 0;
    for (int i = 0; i < l; i++)
    {
        if (command[i] == '|')
            pipeExist = 1;
    }

    if (pipeExist)
    {
        char *seperatedCommands[INPUT_LIMIT];
        int numberOfCommands = 0;
        char *delim = "|";
        char *token = strtok(command, delim);
        while (token != NULL)
        {
            seperatedCommands[numberOfCommands] = (char *)malloc(sizeof(char) * (strlen(token) + 1));
            strcpy(seperatedCommands[numberOfCommands], token);
            numberOfCommands++;
            token = strtok(NULL, delim);
        }

        char buf[OUTPUT_LIMIT];
        int bufSize = 0;
        for (int i = 0; i < numberOfCommands; i++)
        {
            int pipefd[2];
            int pipefd2[2];
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                return;
            }
            if (pipe(pipefd2) == -1)
            {
                perror("pipe");
                return;
            }

            if (bufSize > 0)
            {
                write(pipefd[1], buf, bufSize);
            }
            int cPid = fork();
            if (cPid == -1)
            {
                perror("fork");
                return;
            }

            if (cPid == 0)
            {
                if (i > 0 && dup2(pipefd[0], STDIN_FILENO) == -1)
                {
                    perror("dup2");
                    close(pipefd[0]);
                    close(pipefd2[1]);
                    exit(1);
                }
                if (i < numberOfCommands - 1 && dup2(pipefd2[1], STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    close(pipefd[0]);
                    close(pipefd2[1]);
                    exit(1);
                }
                close(pipefd[1]);
                close(pipefd2[0]);
                close(pipefd[0]);
                close(pipefd2[1]);
                execute(seperatedCommands[i], pwd, backgroundProcess, timePrompt, activities);
                exit(0);
            }
            close(pipefd2[1]);
            close(pipefd[0]);
            close(pipefd[1]);

            waitpid(cPid, NULL, 0);
            bufSize = read(pipefd2[0], buf, OUTPUT_LIMIT);
            close(pipefd[0]);
        }
        return;
    }

    // pipe taken care
    if (command[0] == '<' || command[0] == '>')
    {
        fprintf(stderr, ANSI_COLOR_RED "Invalid Usage\n" ANSI_COLOR_RESET);
        return;
    }

    char delim[] = " \t\n";
    char *outFile = strrchr(command, '>');
    int saveOUT = dup(STDOUT_FILENO);
    if (outFile != NULL)
    {
        // printf("hii");
        if (outFile[-2] == ' ')
            outFile[-2] = '\0';
        else if (outFile[-1] == ' ')
            outFile[-1] = '\0';
        // printf("%s",inpFile+2);
        int fd;
        if (outFile[-1] == '>')
            fd = open(parsePath(outFile + 2, pwd->homeDirectory, NULL, pwd), O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            fd = open(parsePath(outFile + 2, pwd->homeDirectory, NULL, pwd), O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (fd == -1)
        {
            perror("open");
            return;
        }
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("dup2");

            return;
        }
        close(fd);
    }

    char *seperatedCommands[INPUT_LIMIT];
    int numberOfCommands = 0;
    char *token = strtok(command, delim);
    while (token != NULL)
    {
        seperatedCommands[numberOfCommands] = (char *)malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(seperatedCommands[numberOfCommands], token);
        numberOfCommands++;
        token = strtok(NULL, delim);
    }

    char commandToExec[INPUT_LIMIT];
    strcpy(commandToExec, "");
    for (int i = 0; i <= numberOfCommands; i++)
    {
        if (i == numberOfCommands || strcmp(seperatedCommands[i], ";") == 0)
        {
            timePrompt->timeToShow = 0;

            commandToExec[strlen(commandToExec) - 1] = '\0';
            actualExecute(commandToExec, pwd, timePrompt, backgroundProcess, activities);
            strcpy(commandToExec, "");
        }
        else if (i <= numberOfCommands - 1 && (strcmp(seperatedCommands[i], "<") == 0))
        {
            int fileToRedirect;
            if (strcmp(seperatedCommands[i], "<") == 0)
                fileToRedirect = STDIN_FILENO;
            else
                fileToRedirect = STDOUT_FILENO;

            int terminalOut = dup(fileToRedirect);
            int fd;
            if (i <= numberOfCommands - 2)
            {
                if (strcmp(seperatedCommands[i], ">") == 0)
                    fd = open(parsePath(seperatedCommands[i + 1], pwd->homeDirectory, NULL, pwd), O_WRONLY | O_TRUNC | O_CREAT, 0644);
                else if (strcmp(seperatedCommands[i], ">>") == 0)
                    fd = open(parsePath(seperatedCommands[i + 1], pwd->homeDirectory, NULL, pwd), O_WRONLY | O_APPEND | O_CREAT, 0644);
                else
                    fd = open(parsePath(seperatedCommands[i + 1], pwd->homeDirectory, NULL, pwd), O_RDONLY);
            }
            if (fd == -1)
            {
                perror(ANSI_COLOR_RED"open: No such input file found!"ANSI_COLOR_RESET);
                freeStringArr(seperatedCommands, numberOfCommands);

                return;
            }

            if (dup2(fd, fileToRedirect) == -1)
            {
                perror("dup2");
                freeStringArr(seperatedCommands, numberOfCommands);

                return;
            }
            close(fd);

            timePrompt->timeToShow = 0;
            commandToExec[strlen(commandToExec) - 1] = '\0';
            // printf(".%s.",commandToExec);
            actualExecute(commandToExec, pwd, timePrompt, backgroundProcess, activities);
            strcpy(commandToExec, "");

            fflush(stdout);

            if (dup2(terminalOut, fileToRedirect) == -1)
            {
                perror("dup2");
                freeStringArr(seperatedCommands, numberOfCommands);

                return;
            }
            i += 2;
        }
        else
        {
            timePrompt->timeToShow = 0;

            strcat(commandToExec, seperatedCommands[i]);
            strcat(commandToExec, " ");
        }
    }
    if (outFile != NULL)
    {
        if (dup2(saveOUT, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            freeStringArr(seperatedCommands, numberOfCommands);
            return;
        }
    }
    freeStringArr(seperatedCommands, numberOfCommands);
}

int activityExecute(char *command, struct PWD *pwd, Container *backgroundProcess, struct commandTime *timePrompt, Container *activities)
{
    char *activityOp[OUTPUT_LIMIT];
    int numActivity = 0;
    for (int i = 0; i < activities->numContent; i++)
    {
        int stat;
        char buffer[OUTPUT_LIMIT * 2];
        char status[OUTPUT_LIMIT];
        if (waitpid(activities->idContainer[i], &stat, WNOHANG) != 0)
        {
            remContent(activities, activities->idContainer[i]);
            continue;
        }

        char fileName[OUTPUT_LIMIT];
        sprintf(fileName, "/proc/%d/stat", activities->idContainer[i]);
        FILE *f = fopen(fileName, "r");
        if (f == NULL)
            continue;
        char statFile[OUTPUT_LIMIT];
        fgets(statFile, OUTPUT_LIMIT, f);
        int count = 0;
        for (int i = 0; i < strlen(statFile); i++)
        {
            if (statFile[i] == ' ')
                count++;
            if (count == 2)
            {
                if (statFile[i + 1] == 'T')
                    strcpy(status, "Stopped");
                else
                    strcpy(status, "Running");
                break;
            }
        }

        sprintf(buffer, "%d : %s - %s", activities->idContainer[i], activities->stringContainer[i], status);
        activityOp[numActivity++] = strdup(buffer);
        qsort(activityOp, numActivity, sizeof(activityOp[0]), compareStrings);
    }
    for (int i = 0; i < numActivity; i++)
    {
        printf("%s\n", activityOp[i]);
    }
    return 0;
}

int compareStrings(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

int pingExecute(char *command, struct PWD *pwd, Container *backgroundProcess, struct commandTime *timePrompt, Container *activities)
{
    char *argPing = strchr(command, ' ');
    if (argPing == NULL)
    {
        perror("ping: Invalid Syntax");
        return 1;
    }
    argPing++;
    char *argPing2 = strchr(argPing, ' ');
    if (argPing2 == NULL)
    {
        perror("ping: Invalid Syntax");
        return 1;
    }
    argPing2[0] = '\0';
    argPing2++;

    int pid = atoi(argPing);
    int signal = atoi(argPing2) % 32;

    if (kill(pid, 0) == -1)
    {
        perror("No such process found");
        return 1;
    }
    if (kill(pid, signal) == 0)
    {
        printf("Sent signal %d to process with pid %d\n", signal, pid);
    }
    else
    {
        perror("Failed to send signal");
        return 1;
    }
}

int fgbgExecute(char *command, struct PWD *pwd, Container *backgroundProcess, struct commandTime *timePrompt, Container *activities)
{
    int pid = atoi(command + 3);
    int isT = 0;
    char commandName[INPUT_LIMIT];

    char fileName[OUTPUT_LIMIT];
    sprintf(fileName, "/proc/%d/stat", pid);
    FILE *f = fopen(fileName, "r");
    if (f == NULL)
    {
        printf("No such process found\n");
        return 1;
    }
    char statFile[OUTPUT_LIMIT];
    fgets(statFile, OUTPUT_LIMIT, f);
    int count = 0;
    for (int i = 0; i < strlen(statFile); i++)
    {
        if (statFile[i] == ' ')
            count++;
        if (count == 1)
        {
            strcpy(commandName, (statFile + i));
            char *sp = strchr(commandName, ' ');
            sp[0] = '\0';
        }
        if (count == 2)
        {
            if (statFile[i + 1] == 'T')
                isT = 1;
            break;
        }
    }

    if (isT)
    {
        if (kill(pid, SIGCONT) != 0)
        {
            perror("Failed to send signal");
            return 1;
        }
    }
    if (command[0] == 'f')
    {
        waitpid(pid, &isT, 0);
    }
    else
    {
        int isInbg = 0;
        for (int i = 0; i < backgroundProcess->numContent; i++)
        {
            if (backgroundProcess->idContainer[i] == pid)
            {
                isInbg = 1;
                break;
            }
        }
        if (!isInbg)
        {
            addContent(backgroundProcess, pid, commandName);
            printf("\n[%d] %d\n", backgroundProcess->numContent, pid);
        }
    }
    return 0;
}

void freeStringArr(char **s, int l)
{
    for (int i = 0; i < l; i++)
    {
        free(s[i]);
    }
    // free(s);
}
