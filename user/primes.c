#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primesieve(int p[]){//从管道p[]读入数据
    close(p[1]);
    int nums[36],cnt=0;
    int prime,now;
    read(p[0],&prime,sizeof(int));
    printf("prime %d\n",prime);
    while(read(p[0],&now,sizeof(int))!=0){
        if(now%prime!=0) nums[++cnt]=now;
    }
    close(p[0]);
    if(cnt==0) return ;
    //cnt>0
    int pr[2];
    pipe(pr);//创建一个新管道将数据输出到下一个进程
    int pid=fork();
    if(pid==0){//子进程
        primesieve(pr);
        exit(0);
    }
    close(pr[0]);
    for(int i=1;i<=cnt;i++){
        write(pr[1],&nums[i],sizeof(int));
    }
    close(pr[1]);
    wait(0);//这条不要忘了，否则子进程可能成为僵尸进程
}
int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    int pid=fork();
    if(pid>0){
        close(p[0]);
        for(int i=2;i<=35;i++){
            write(p[1],&i,sizeof(int));
        }//第一个进程将数字2到35输入管道
        close(p[1]);
        wait(0);
        exit(0);
    }
    else if(pid==0){
        printf("primes\n");
        primesieve(p);
        exit(0);
    }
}
