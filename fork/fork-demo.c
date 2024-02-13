#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd;
    pid_t child_pid;
    
    unlink("test.txt");
    fd = open("test.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    if ((child_pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    if (child_pid > 0) {
        char parent_txt[] = "I'm the parent\n";

        /* Parent */
        lseek(fd, 0, SEEK_END);
        write(fd, parent_txt, sizeof(parent_txt));
        close(fd);

	    pause();

    } else {
        char child_txt[] = "I'm the child\n";

        /* Child */
        lseek(fd, 0, SEEK_END);
        write(fd, child_txt, sizeof(child_txt));
        close(fd);
    }
}
