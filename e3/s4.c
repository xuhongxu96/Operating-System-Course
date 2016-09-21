#include <stdio.h>
#include <unistd.h>

const int N = 10;

void output() {
    for (int i = 0; i < N; ++i) {
        printf("CHILD-%d: %d\n", getpid(), i);
        usleep(1000);
    }
}

int main() {
    pid_t fpid;
    printf("PARENT: a, pid: %d\n", getpid());

    fpid = fork();
    if (fpid == 0) {
        output();
    } else {
        fpid = fork();
        if (fpid == 0) {
            output();
        }
    }
}
