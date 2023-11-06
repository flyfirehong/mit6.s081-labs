#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    int p[2];
    pipe(p);
    char s[10];
    int pid;
    pid = fork();
    if(pid > 0){
        //close(p[0]);
        //char* tmp = "ping";
        write(p[1], "ping", 5);
        wait(0);//如果不使用wait会导致程序无法正常结束
        read(p[0], s, 5);
        char* tmp = "pong";
        if(strcmp(tmp,s) == 0){
            printf("%d: received pong\n", getpid());
        }
    }else if(pid == 0){
        //close(p[1]);
        read(p[0], s, 5);
        int n = getpid();
        char* tmp = "ping";
        if(strcmp(s,tmp) == 0) {
            printf("%d: received ping\n", n);
            write(p[1], "pong", 5);
        }
    }else{
        exit(1);
    }
    exit(0);
}