#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<errno.h>

int main(){
	int cpid;
	
	cpid=fork();
	if(cpid<0){
		fprintf(stderr,"error in fork()\n");
	}
	if(cpid==0){//child process
		char *cmd[]={"/bin/cat","temp.txt",(char*)0};
		execv("/bin/cat",cmd);
		printf("hello from the child process\n");
	}
	else{//parent process
		if(wait(NULL)==-1){
			perror("wait");
		}
		printf("child executed\n");
	}

return 0;
}
