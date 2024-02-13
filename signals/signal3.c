#include <poll.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void term_handler(int signo, siginfo_t *si, void *uctx) {

}

void intr_handler(int signo, siginfo_t *si, void *uctx) {

}

int main() {
    int i, n;
    struct sigaction sa_term, sa_intr;
    sigset_t ss, ss2;

    memset(&sa_term, 0, sizeof(struct sigaction));
    memset(&sa_intr, 0, sizeof(struct sigaction));

    sa_term.sa_sigaction = &term_handler;
    sa_intr.sa_sigaction = &intr_handler;

    sigaction(SIGINT, &sa_term, NULL);
    sigaction(SIGTERM, &sa_intr, NULL);

    sigfillset(&ss);
    sigdelset(&ss, SIGTERM);
    sigdelset(&ss, SIGINT);

    sigsuspend(&ss);

    sigpending(&ss2);

    printf("Pending signals:\n");
    for (size_t i = 1; i < 32; i++) {
        char *s;

        if (sigismember(&ss2, i)) {
            psignal(i, s);
            printf("\t%s\n", s);
        }
    }
}