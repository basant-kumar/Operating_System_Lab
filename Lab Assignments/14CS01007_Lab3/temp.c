#include<stdio.h>
#include<string.h>

int main(){
	char str[100];
	read(0,str,sizeof(str));
	if(strchr(str,'<')!=NULL){
		printf("mil gaya\n");
	}else{ printf("nhi mila\n");}
return 0;
}
