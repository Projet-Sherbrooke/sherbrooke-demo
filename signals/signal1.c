#include <poll.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void term_handler(int signo) {}

void intr_handler(int signo) {}

int main() {
    int n;

    signal(SIGTERM, &term_handler);
    signal(SIGINT, &intr_handler);

    /* Busy looping */
    while (1);

    perror("poll");
}