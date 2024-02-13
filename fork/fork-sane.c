#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int nbfork, n;

    if (argc < 1) {
        fprintf(stderr, "Missing argument\n");
        exit(1);
    }

    nbfork = atoi(argv[1]);

    for (int i = 0; i < nbfork; i++) {
        pid_t child_pid = fork();
        int s, n;

        n = rand_r(&s);

        if (child_pid < 0) {
            fprintf(stderr, "Oops, fork() failed somehow\n");
            exit(1);
        }

        if (!child_pid) {
            /* This is the children entry point. */
            exit(n);
        }
    }

    do {
        int s;

        n = wait(&s);
        if (n < 0) break;

        if (WIFEXITED(s))
            fprintf(stdout, "Process %d is gone, status code %d\n", n, WEXITSTATUS(s));
        else
            fprintf(stdout, "Process %d is somehow gone!\n", n);
    } while (1);

    pause();
}