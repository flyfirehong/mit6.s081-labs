#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int p[]){
    int i;
    read(p[0], &i, sizeof(int));
    printf("prime %d\n", i);
    close(p[1]);//不关管道写端，会导致死循环输出0
    if(i == 31) exit(0);

    int pp[2];
    pipe(pp);
    int pid = fork();
    if(pid > 0){
        int digit;
        close(pp[0]);
        while(1){
            if(read(p[0], &digit, sizeof(int)) < 1) {
                break;
            }
            if(digit%i != 0) write(pp[1], &digit, sizeof(int));
        }
        close(p[0]);
        close(pp[1]);
        wait(0);
    }else if(pid == 0){
        primes(pp);
    }else{
        exit(1);
    }
    exit(0);
}

int main(){
    int p[2];
    pipe(p);
    int pid = fork();
    if(pid > 0){
        close(p[0]);
        int i = 2;
        printf("prime %d\n", i);
        int j;
        for(j = i+1; j<=35; j++){
            if(j%i != 0){
                write(p[1], &j, sizeof(int));
            }
        }
        close(p[1]);
        wait(0);
    }else if(pid == 0){
        primes(p);
    }else{
        fprintf(2, "process create error!\n");
        exit(1);
    }
    exit(0);
}