#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void func(int *fd_l)
{
    int i, p;
    int fd_r[2];
    close(fd_l[1]);
    if(read(fd_l[0], &p, sizeof(int)) <= 0)
    {
        return ;
    }
        
    printf("prime %d\n", p);
    pipe(fd_r);
    if(fork() == 0)
    {
        func(fd_r);
        return ;
    }
    close(fd_r[0]);
    while(read(fd_l[0], &i, sizeof(int)) > 0)
    {
        if(i % p != 0)
        {
            write(fd_r[1], &i, sizeof(int));
        }
    }
    close(fd_l[0]);
    close(fd_r[1]);
    wait(0);
}

int main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);
    for(int i = 2; i <= 35; i ++)
    {
        write(fd[1], &i, sizeof(int));
    }
    if(fork() == 0)
    {
        func(fd);
        exit(0);
    }
    close(fd[0]);
    close(fd[1]);
    wait(0);
    exit(0);
}
