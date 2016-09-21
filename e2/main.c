#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t fpid;
    printf("a\n");

    fpid = fork();
    if (fpid == 0) {
        printf("b\n");
    } else {
        fpid = fork();
        if (fpid == 0) {
            printf("c\n");
        }

    }
}
