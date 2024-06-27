#include "iman.h"

int iManExecute(char *command, struct PWD *pwd, Container *backgroundProcess, struct commandTime *timePrompt, Container *activities)
{
    char *sp = strchr(command, ' ');
    if (sp == NULL || strcmp(strchr(command, ' '), strrchr(command, ' ')) != 0)
    {
        printf(ANSI_COLOR_RED "Invalid usage of iMan\n" ANSI_COLOR_RESET);
        return 1;
    }
    char *cmd = strdup(sp + 1);
    char host[] = "man.he.net";
    int port = 80;

    struct hostent *hostInfo;
    struct sockaddr_in *serverAddress = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    hostInfo = gethostbyname(host);
    if (hostInfo == NULL)
    {
        printf(ANSI_COLOR_RED "Error in gethost\n" ANSI_COLOR_RESET);
        return 1;
    }

    // socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf(ANSI_COLOR_RED "Error in socket\n" ANSI_COLOR_RESET);
        return 1;
    }

    serverAddress->sin_family = AF_INET;
    serverAddress->sin_port = htons(port);
    serverAddress->sin_addr = *((struct in_addr *)hostInfo->h_addr);

    if (connect(sockfd, (struct sockaddr *)serverAddress, sizeof(*serverAddress)) == -1)
    {
        printf(ANSI_COLOR_RED "Error in connecting to the server\n" ANSI_COLOR_RESET);
        close(sockfd);
        return 1;
    }

    char request[INPUT_LIMIT];
    sprintf(request, "GET /?topic=%s&section=all HTTP/1.1\r\nHost: %s\r\n\r\n", cmd, host);

    if (send(sockfd, request, INPUT_LIMIT, 0) == -1)
    {
        perror(ANSI_COLOR_RED "send: Error sending GET" ANSI_COLOR_RESET);
        close(sockfd);
        return 1;
    }

    char buffer[OUTPUT_LIMIT];
    int bytes;

    int toPrint = 0;
    int done = 0;
    while ((bytes = recv(sockfd, buffer, OUTPUT_LIMIT - 1, 0)) > 0)
    {
        buffer[bytes] = '\0';
        int n = strlen(buffer);
        for (int i = 0; i < n; i++)
        {
            if (strncmp(buffer + i, "NAME\n", 5) == 0)
                toPrint = 1;
            if (strncmp(buffer + i, "AUTHOR\n", 7) == 0)
            {
                done = 1;
                break;
            }
            if (toPrint)
                printf("%c", buffer[i]);
        }
        if (done)
            break;
    }
    if (!toPrint)
        fprintf(stderr, ANSI_COLOR_RED "ERROR\n\tNo such command\n" ANSI_COLOR_RESET);

    if (bytes == -1)
    {
        perror("Error receiving HTTP response");
    }

    // Close the socket when done
    close(sockfd);
    return 0;
}