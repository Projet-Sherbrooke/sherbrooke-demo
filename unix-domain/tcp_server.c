#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define SOCK_PATH "echo_socket"

int handle_cmd(char *cmd, size_t sz) {
    return 0;   
}

/* Awful implementation of reading a line from a socket 
   but in the scale of our current work, this will do. */
int readline(int s, char *line, size_t szline) {
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

int main(void) {
    int s, s2, len, yes=1;
    struct sockaddr_in remote, local = {
        .sin_family = AF_INET,
        .sin_port = htons(1234),
        .sin_addr.s_addr = INADDR_ANY
    };

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* Prevents an error about the address being already in use */
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    /* Attach the socket we created to the address/port we want. */
    if (bind(s, (struct sockaddr *)&local, sizeof(struct sockaddr_in)) == -1) {
        perror("bind");
        exit(1);
    }

    /* Puts the socket in listening mode. */
    if (listen(s, 5) == -1) {
        perror("listen");
        exit(1);
    }

    for(;;) {
        int done, n;
        printf("Waiting for a connection...\n");
        socklen_t slen = sizeof(remote);
        if ((s2 = accept(s, (struct sockaddr *)&remote, &slen)) == -1) {
                perror("accept");
                exit(1);
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

            if (handle_cmd(line_buf, n) < 0) {
                fprintf(stderr, "Invalid command: %s", line_buf);
                done = 1;
            } else {
                char ok[] = "OK\n";

                send(s2, ok, sizeof(ok), 0);
            }
        } while (!done);

        close(s2);
    }

    return 0;
}
