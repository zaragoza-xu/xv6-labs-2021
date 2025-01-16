#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *obj)
{
    char buf[512];
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        exit(1);
    }

    if(fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit(1);
    }

    if(st.type != T_DIR)
    {
        fprintf(2, "find: %s is not a directory\n", path);
        close(fd);
        exit(1);
    }
    strcpy(buf, path);
    char *p = buf + strlen(buf);
    *(p++) = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if(de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0)
        {
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        if(st.type == T_FILE)
        {
            int flag = 0;
            if(!strcmp(p, obj))
            {
                flag = 1;
            }
            if(flag)
                printf("%s\n", buf);
        }
        if(st.type == T_DIR && strcmp(de.name, ".") && strcmp(de.name, ".."))
        {
            find(buf, obj);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("find: incorrect number of argument\n");
        exit(1);
    }
    
    find(argv[1], argv[2]);

    exit(0);
}
