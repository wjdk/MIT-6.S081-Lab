#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p1[2],p2[2];
    char buf[1];
    pipe(p1);//父->子
    pipe(p2);//子->父
    
    int pid=fork();
    
    if(pid>0){//父进程
        close(p1[0]);
        close(p2[1]);
        write(p1[1]," ",1);
        read(p2[0],buf,1);
        printf("%d: received pong\n",getpid());
        close(p1[1]);
        close(p2[0]);
        exit(0);
    }
    else if(pid==0){//子进程
        close(p1[1]);
        close(p2[0]);
        read(p1[0],buf,1);
        printf("%d: received ping\n",getpid());
        write(p2[1]," ",1);
        close(p1[0]);
        close(p2[1]);
        exit(0);
    }
}
