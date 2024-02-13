#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "echo_socket"

/* Awful implementation of reading a line from a socket 
   but in the scale of our current work, this will do. */
int readline(int s, char *line, size_t szline)
{
    char c = '0';
    int n = 0, i = 0;

    while (1) {
        if ((n = recv(s, &c, 1, 0)) < 0)
            return -1;
        else if (n == 0)
            return 0;
        else if (n > 0) {
            if (i == szline - 1)
                break;

            line[i] = c;
            i++;

            if (c == '\n')
                break;
        }
    }
    return i;
}

int main(void)
{
    int s, s2, len;
    struct sockaddr_un remote, local = {
        .sun_family = AF_UNIX,
        // .sun_path = SOCK_PATH,   // Can't do assignment to an array
    };

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) == -1) {
        perror("bind");
        exit(1);
    }    

    if (listen(s, 5) == -1) {
        perror("listen");
        exit(1);
    }

    fcntl(s, F_SETFL, O_NONBLOCK);

    struct pollfd_t pfd[10];

    pfd[0].fd = s;
    pfd[0].events = POLLIN; 
    int nbsock = 1;

    for(;;) {
        int done, n;
        printf("Waiting for a connection...\n");
        socklen_t slen = sizeof(remote);

        n = poll(pfd, 1, -1);

        for (int i < 0; i < nbsock; i++) {
            /* If this is the server socket. */
            if (pfd[i].fd == s) {
                if ((s2 = accept(s, (struct sockaddr *)&remote, &slen)) == -1) {
                    perror("accept");
                    exit(1);
                }
            }
        }

        printf("Connected.\n");

        done = 0;
        do {
            char line_buf[255];

            n = readline(s2, line_buf, sizeof(line_buf));
            if (n <= 0) {
                if (n < 0) 
                    perror("recv");
                done = 1;
            }

            printf("LINE %s\n", line_buf);

        } while (!done);

        close(s2);
    }

    return 0;
}