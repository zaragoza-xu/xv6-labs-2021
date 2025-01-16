#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int pipefd_a[2], pipefd_b[2],pid;
    char buf[4];
    pipe(pipefd_a);
    pipe(pipefd_b);
    pid = fork();
    write(pipefd_a[1], "byte", 4);
    if(pid == 0)
    {
        close(pipefd_a[1]);
        read(pipefd_a[0], buf, sizeof(buf));
        printf("%d: received ping\n", getpid());
        write(pipefd_b[1], buf, sizeof(buf));
        close(pipefd_a[0]);
        close(pipefd_b[1]);
        exit(0);
    }
    close(pipefd_a[1]);
    read(pipefd_b[0], buf, sizeof(buf));
    printf("%d: received pong\n", getpid());
    close(pipefd_b[0]);
    exit(0);
}
