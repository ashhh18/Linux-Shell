#include "neonate.h"

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

/**
 * stdout and stdin are buffered we disable buffering on that
 * After entering in raw mode we read characters one by one
 * Up arrow keys and down arrow keys are represented by 3 byte escape codes
 * starting with ascii number 27 i.e. ESC key
 * This way we interpret arrow keys
 * Tabs are usually handled by the term, but here we are simulating tabs for the sake of simplicity
 * Backspace move the cursor one control character to the left
 * @return
 */
int neonExecute(char *command, struct PWD *pwd, Container *backgroundProcess, struct commandTime *timePrompt, Container *activities)
{
    char *cpCommand = strdup(command);
    char *lastSpace = strrchr(cpCommand, ' ');
    if (lastSpace == NULL)
    {
        printf(ANSI_COLOR_RED "Invalid Syntax\n" ANSI_COLOR_RESET);
        return 1;
    }
    lastSpace[0] = '\0';
    if (strcmp(cpCommand, "neonate -n") != 0)
    {
        printf(ANSI_COLOR_RED "Invalid Syntax\n" ANSI_COLOR_RESET);
        return 1;
    }

    int sec = atoi(strrchr(command, ' '));
    int pid = fork();

    // print in child
    if (pid == 0)
    {
        struct timeval start_time, end_time;
        gettimeofday(&start_time, NULL);
        int latestPid = 0;
        while (1)
        {
            // rest
            gettimeofday(&end_time, NULL);
            if (((end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0) > sec)
            {
                FILE *f = fopen("/proc/loadavg", "r");
                char buf[OUTPUT_LIMIT];
                int bite = fread(buf, 1, OUTPUT_LIMIT, f);
                buf[bite] = '\0';
                char *lastSpace = strrchr(buf, ' ');
                lastSpace++;
                latestPid = atoi(lastSpace);

                printf("%d\n", latestPid);
                gettimeofday(&start_time, NULL);
            }
        }
        exit(0);
    }
    // printf("..%d..",pid);
    // addContent(activities,pid,command);

    char *inp = malloc(sizeof(char) * 100);
    char c;
    while (1)
    {
        setbuf(stdout, NULL);
        enableRawMode();
        memset(inp, '\0', 100);
        int pt = 0;
        while (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (iscntrl(c))
            {
                if (c == 10)
                    break;
                else if (c == 27)
                {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2)
                    { // length of escape code
                        printf("\rarrow key: %s", buf);
                    }
                }
                else if (c == 127)
                { // backspace
                    if (pt > 0)
                    {
                        if (inp[pt - 1] == 9)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        printf("\b \b");
                    }
                }
                else if (c == 9)
                { // TAB character
                    inp[pt++] = c;
                    for (int i = 0; i < 8; i++)
                    { // TABS should be 8 spaces
                        printf(" ");
                    }
                }
                else if (c == 4)
                {
                    exit(0);
                }
            }
            else if (c == 'x')
            {
                if (kill(pid, SIGKILL) != 0)
                {
                    perror("kill: killing child failed");
                    return 1;
                }
                disableRawMode();
                return 0;
            }
        }
        disableRawMode();
    }
    return 0;
}