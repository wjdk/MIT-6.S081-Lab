#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    char * command=argv[1];
    char parm[MAXARG][105];
    for(int i=1;i<argc;i++){
        strcpy(parm[i-1],argv[i]);
    }
    int parm_cnt=argc-1;//[0-parm_cnt-1]为原有参数
    while(1){
        parm_cnt=argc-1;
        // printf("test: %s %d\n",parm[0],parm_cnt);
        for(int i=parm_cnt;i<MAXARG;i++) memset(parm[i],0,sizeof parm[i]);//清空!
        char buf[105],*p=buf;
        int tmp;
        int stend=0;
        int flag=0;
        while((tmp=read(0,p,1))>0 && (*p!='\n')){
            if(*p!=' '){
                parm[parm_cnt][stend++]=*p;
                flag=1;
            }
            else if(flag){
                parm[parm_cnt][stend]='\0';
                parm_cnt++;
                stend=0;
                flag=0;
            }
            p++;
        }
        parm_cnt++;
        stend=0;
        if(!flag){
            break;
        }
        char *parm_tmp[MAXARG];
        for(int i=0;i<parm_cnt;i++) parm_tmp[i]=parm[i];
        for(int i=parm_cnt;i<MAXARG;i++) parm_tmp[i]=0;
        int pid=fork();
        if(pid==0){
            exec(command,parm_tmp);
            exit(0);
        }
        else{
            wait(0);
        }
    }
    exit(0);
}
