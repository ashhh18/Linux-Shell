#include "main.h"

int curForeground = -1;

int main()
{
    signal(SIGINT, ctrlCHandler);
    signal(SIGTSTP, ctrlZHandler);

    struct PWD pwd;
    initPWD(&pwd);
    Container *backgroundProcess = initContainer();
    Container *activities = initContainer();
    struct commandTime *timePrompt = initTime();
    // Keep accepting commands
    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(&pwd, timePrompt);
        char input[4096];
        if (fgets(input, 4096, stdin) == NULL)
        {
            kill(-1 * getpid(), SIGKILL);
        }

        checkCompletion(backgroundProcess, &pwd);
        execute(input, &pwd, backgroundProcess, timePrompt, activities);
        checkCompletion(backgroundProcess, &pwd);
    }
}

void ctrlCHandler(int a)
{
    int pid = curForeground;
    if (curForeground == -1)
        return;
    if (kill(pid, 0) == -1)
    {
        perror("No such process found");
        return;
    }
    if (kill(pid, 2) == 0)
    {
    }
    else
    {
        perror("Failed to send signal");
        return;
    }
}

void ctrlZHandler(int a)
{
    int pid = curForeground;
    if (curForeground == -1)
        return;
    if (kill(pid, 0) == -1)
    {
        perror("No such process found");
        return;
    }
    if (kill(pid, SIGTSTP) == 0)
    {
    }
    else
    {
        perror("Failed to send signal");
        return;
    }
}
