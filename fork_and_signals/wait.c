#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<stdlib.h>

int main(){
	pid_t pid;
	pid=fork();
	if(pid<0){
		printf("error in fork()\n");
		exit(1);
	}
	if(pid==0){
		//this is child
		int t=100;
		while(t--){
			printf("*** this is from the child ***\n");
		}
	}
	if(pid>0){
		//this is parent
		if(wait(NULL)==-1){
			printf("error in wait()\n");
		}
		int t=100;
		while(t--){
			printf("#### this is from the parent ###\n");
		}
	}
return 0;
}
