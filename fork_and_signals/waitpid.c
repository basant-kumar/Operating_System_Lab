#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<stdlib.h>

int main(){
	pid_t pid,pid1;
	int status;
	pid=fork();
	pid1=fork();
	if(pid<0){
		printf("error in fork()\n");
		exit(1);
	}
	if(pid1<0){
			printf("error in 2fork2()\n");
			exit(1);
		}

	if(pid==0){		
		//this is first child
		int t=10;
		while(t--){
			printf("*** this is from the child those pid is:%d and ppid is:%d ***\n",getpid(),getppid());
		}
			
	}
	if(pid1==0){
		//this is second child
		int t=10;
		while(t--){
			printf("@@@ this is from the child those pid is:%d and ppid is:%d @@@\n",getpid(),getppid());
		}
	}	
	else{
		//this is parent
		waitpid(pid,&status,0);
		waitpid(pid1,&status,0);
		printf("i am going to sleep for 5 sec\n");
		sleep(5);
		int t=10;
		while(t--){
			printf("#### this is from the parent those pid is:%d and ppid is:%d ###\n",getpid(),getppid());
		}
	}
return 0;
}
