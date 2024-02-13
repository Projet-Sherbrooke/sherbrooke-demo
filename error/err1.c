#include <stdio.h>
#include <string.h>
#include <errno.h>

int main() {

    errno = EINVAL;
    perror("There was an error");

    errno = ENOMEM;
    printf("Error # %d: %s\n", errno, strerror(errno));

    errno = ENOENT;
    char buf[255];
    strerror_r(errno, buf, sizeof(buf));
    printf("Error # %d: %s\n", errno, buf);
}
