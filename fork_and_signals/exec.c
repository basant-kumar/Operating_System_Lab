#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
	int ret;
	//char *env[] = { "HOME=/usr/home", "LOGNAME=home", (char *)0 };
	ret=execl("/bin/ls","ls","-l",(char*)0);
	printf("one down two to go\n\n\n");
	execlp("ls","ls","-l",(char*)0);
	printf("two down one to go\n\n\n");
	
	
return 0;
}		
