#include<stdio.h>
#include<fcntl.h>
#include<string.h>

int main(){

	int fp=open("a.txt",O_RDONLY);
	char buf[256];
	read(fp,buf,sizeof(buf));
	int size=strlen(buf);
	buf[size-5]='\0';
	printf("buf is %s\n",buf);
	close(fp);
return 0;
}
