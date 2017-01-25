#include<stdio.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/time.h>

int main(int argc,char *argv[]){
	struct addrinfo hints,*res,*p;
	int status,sockfd,yes=1,numbytes;
	//char buf[max_size];
	if(argc != 4){
		printf("Give IP Address, Port Number of server and Filename\n");
		printf("\ne.g.->$ ./client 192.168.186.128 8888 f1.txt\n");
		exit(1);
	}
	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype=SOCK_STREAM;

	if((status=getaddrinfo(argv[1],argv[2],&hints,&res))==-1){
		fprintf(stderr,"getaddrinfo error:%s\n",gai_strerror(status));
		return 1;
	}

	for(p=res;p!=NULL;p=p->ai_next){
		if((sockfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol))==-1){
			perror("client:socket");
			continue;
		}
		//to handle Address already in Use error
		if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
			perror("server:setsockopt");
			exit(1);
		}
		if(connect(sockfd,p->ai_addr,p->ai_addrlen)==-1){
			close(sockfd);			
			perror("client:connect");
			continue;
		}
		break;
	}
	if(p==NULL){
		printf("client:cannot connect to server\n");
		exit(1);
	}
	printf("connecting to server\n");
	if((numbytes=send(sockfd,argv[3],sizeof(argv[3]),0))==-1){
		perror("client:send");
		exit(1);
	}
	int filesize;
	if(recv(sockfd,&filesize,sizeof(int),0)==-1){
		perror("client:recv");
		exit(1);
	}
	if(filesize==0){
		printf("filename does not exists on server\n");
		exit(0);
	}else{
		printf("Good to go\n");
	}
	char buf[filesize];
	//printf("sent bytes :%d\n",numbytes);
	FILE *fp;
	fp=fopen(argv[3],"w");
	if(fp==NULL){
		perror("file did not open:");
		exit(1);
	}
	

	if((numbytes=recv(sockfd,buf,sizeof(buf)-1,0))==-1){
		perror("client:receive");
		exit(1);
	}
	fwrite(buf,1,numbytes,fp);
	printf("file transfer is completed\n");
	
	close(sockfd);

return 0;
}
