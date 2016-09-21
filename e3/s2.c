#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t fpid;
    printf("PARENT: a, pid: %d", getpid());

    fpid = fork();
    if (fpid == 0) {
        printf("CHILD-1: b, pid: %d\n", getpid());
    } else {
        fpid = fork();
        if (fpid == 0) {
            printf("CHILD-2: c, pid: %d\n", getpid());
        }
    }
}
