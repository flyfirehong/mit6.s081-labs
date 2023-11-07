#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
//#define  MAXSIZE 512

int main(int argc, char* argv[])
{
    sleep(10);
    char buf[MAXARG];
    read(0, buf, MAXARG);
    //printf("%s\n", buf);
    char* argvs[MAXARG];
    int ct = 0;
    for(int i=1; i<argc; i++){
        argvs[ct++] = argv[i];
    }
    char *p = buf;
    //每遇到一个换行符，就开一个子进程把这个换行符之前的参数执行掉
    for(int i=0;i<MAXARG;i++){
        if(buf[i] == '\n'){
            //换行符设置为0，因为字符串是读到0结束的
            buf[i] = 0;
            int pid = fork();
            if(pid == 0){
                argvs[ct++] = p;
                exec(argvs[0], argvs);
                exit(0);
            }else if(pid > 0){
                wait(0);
                //让p指向换行符后面
                p = &buf[i+1];
            }
        }
    }
    exit(0);
}