#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    char buf[512] = {0}, *p, *child_argv[MAXARG];
    int pid;
    p = buf;
    for(int i = 1; i < argc; i ++)
        child_argv[i - 1] = argv[i];
    while(read(0, p, 1) != 0)
    {
        while(*(p ++) != '\n')
        {
            read(0, p, 1);
        }
        *(-- p) = 0;
        child_argv[argc - 1] = buf;
        if((pid = fork()) == 0)
        {
            exec(argv[1], child_argv);
        }
        wait(0);
        p = buf;
    }
    
    exit(0);
}
