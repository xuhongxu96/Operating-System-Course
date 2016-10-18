#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024 
#define KEY 75

struct msgbuf {
    long mtype;
    char mtext[BUFFER_SIZE];
};

void server() {

    int msqid = msgget(KEY, IPC_CREAT | 0660);
    if (msqid == -1) {
        printf("msgget err: %d %s\n", errno, strerror(errno));
        return;
    }

    printf("msqid: %d\n", msqid);

    struct msgbuf buffer;
    int ret = 0;

    for (long i = 10; i >= 1; --i) {
        ret = msgrcv(msqid, (void *) &buffer, BUFFER_SIZE, i, MSG_NOERROR);
        if (ret == -1) {
            printf("(Server) err: %d %s\n", errno, strerror(errno));
            break;
        }
        printf("(Server) received %ld: %s\n", buffer.mtype, buffer.mtext);

        buffer.mtype = 10 + i;
        sprintf(buffer.mtext, "Hi, I'm msg %ld", 10 + i);
        ret = msgsnd(msqid, (const void *) &buffer, sizeof(buffer.mtext), 0);
        if (ret == -1) {
            printf("(Server) err: %d %s\n", errno, strerror(errno));
            break;
        } else {
            printf("(Server) sent: %s\n", buffer.mtext);
        }
    }

    printf("(Server) exit\n");

}

void client() {

    int msqid = msgget(KEY, IPC_CREAT | 0660);
    int ret = 0;
    struct msgbuf buffer;

    if (msqid == -1) {
        printf("msgget err: %d %s\n", errno, strerror(errno));
        return;
    }

    for (long i = 10; i >= 1; --i) {
        buffer.mtype = i;
        sprintf(buffer.mtext, "Hello, I'm msg %ld", i);
        int ret = msgsnd(msqid, (const void *) &buffer, sizeof(buffer.mtext), 0);
        if (ret == -1) {
            printf("(Client) err: %d %s\n", errno, strerror(errno));
            break;
        } else {
            printf("(Client) sent: %s\n", buffer.mtext);
        }

        ret = msgrcv(msqid, (void *) &buffer, BUFFER_SIZE, 10 + i, MSG_NOERROR);
        if (ret == -1) {
            printf("(Client) err: %d %s\n", errno, strerror(errno));
            break;
        }
        printf("(Client) received %ld: %s\n", buffer.mtype, buffer.mtext);
    }

    printf("(Client) exit\n");

}

int main() {

    pid_t server_pid, client_pid;


    server_pid = fork();
    if (!server_pid) {
        // server child
        server();
    } else {
        // parent
        client_pid = fork();
        if (!client_pid) {
            // client child
            client();
        } else {
            wait(0);
            wait(0);

            printf("(Parent) exit\n");
        }
    }


    return 0;
}
