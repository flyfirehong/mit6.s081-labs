#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* path, char* target){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    if(st.type != T_DIR){
        fprintf(2, "find: %s is not a directory\n", path);
        close(fd);
        return;
    }
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("find: path too long\n");
        close(fd);
        return;
    }
    //把path拷贝到buf中，在要查找的路径后面加上/
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    //读取目录中每个文件的信息并检查
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        //暂不清楚是用来判断啥的，但是不加会递归有问题
        if(de.inum == 0) continue;
        //对于.和..直接跳过
        if(!strcmp(de.name, ".") || !strcmp(de.name, "..")){
            continue;
        }
        //把文件名拷贝给p,也等同于设置了新的路径
        memmove(p, de.name, DIRSIZ);
        //设置字符串结束位置
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        if(st.type == T_DIR){
            find(buf, target);
        }
        if(st.type == T_FILE && !strcmp(de.name, target)){
            printf("%s\n", buf);
        }
    }
    close(fd);
}

int main(int argc, char* argv[])
{
    if(argc != 3){
        fprintf(2, "Format like: find . b\n");
        exit(1);
    }else{
        find(argv[1], argv[2]);
    }
    exit(0);
}