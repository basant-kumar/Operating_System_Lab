#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<readline/readline.h>
#include<readline/history.h>

#define HISTORY_MAX 20  //max history list size
#define BUFSIZE 1024
#define DELIMITER " \t\r\n"

#define RESET   "\033[0m"
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
int kill_pid=0;
struct history *head,*tail;

struct history{
	int id;
	char data[50];
	struct history *next;
};

void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}


char* prompt(char *ppt){
	char username[30],cwd[100];//,ppt[256];
	getlogin_r(username,sizeof(username));
	getcwd(cwd,sizeof(cwd));
	strcat(username,"@");
	strcat(cwd,">> ");
	//printf(BOLDRED"%s"RESET BOLDCYAN"%s"RESET,username,cwd);
	strcpy(ppt,BOLDRED);strcat(ppt,username);strcat(ppt,RESET);strcat(ppt,BOLDCYAN);strcat(ppt,cwd);strcat(ppt,RESET);
	
}
void welcome_msg(void){
	char welcome[100]="########## Welcome to BShell ##########\t\n";
	char name[100]="Created by Basant Kumar Meena\n";
	char msg[100]="Use 'help' command for assistance\n";
	
	printf(BOLDGREEN"%s"RESET BOLDBLUE"%s"RESET BOLDGREEN "%s\n"RESET,welcome,name,msg);
}

void clear_history1(){
	struct history *p;
	p=head;
	head=head->next;
	free(p);
	
}

char **parse_cmd(char *cmd){
	int i=0,bufsize=BUFSIZE;
	char **arr=calloc(bufsize , sizeof(char));
	char *element;
	if(!arr){
		fprintf(stderr,"Memory Allocation Error in Array\n");
		exit(0);
	}
	element=strtok(cmd,DELIMITER);
	while(element!=NULL){
		arr[i++]=element;
		if(i<=bufsize){
			bufsize+=BUFSIZE;	
			arr=realloc(arr,bufsize*sizeof(char));
			if(!arr){
				fprintf(stderr,"Memory Allocation Error second time for Array\n");
				exit(0);
			}		
		}
		element=strtok(NULL,DELIMITER);
	}
	return arr;
}

//built in commands................
int execute_help(void);
int execute_cd(char **argv);
int execute_jobs(char **argv,int flag);
int execute_history();
int execute_kill(char **argv);
int execute_exit(char **argv);
int execute_else(char **argv);
int background(char **argv,int size);



int execute_cmd(char **argv){
	
	int size=0;
	while(1){
		if(argv[size]==NULL){
			break;
		}
		size++;
	}
	if(argv[0]==NULL){
		return 1;
	}
	else if(strcmp(argv[size-1],"&")==0){
		//printf("background cmd\n");
		return background(argv,size);
	}
	else if(strcmp(argv[0],"help")==0){
		return execute_help();
	}
	else if(strcmp(argv[0],"cd")==0){
		return execute_cd(argv);
	}
	else if(strcmp(argv[0],"jobs")==0){
		return execute_jobs(argv,0);
	}
	else if(strcmp(argv[0],"kill")==0){
		argv[2]='\0';
		return execute_kill(argv);
	}
	else if(strcmp(argv[0],"history")==0){
		return execute_history();
	}
	else if(strcmp(argv[0],"exit")==0){
		return execute_exit(argv);
	}
	else{
		return execute_else(argv);
	}

}

int execute_help(){
	printf("Following commands are built In.\n");
	printf("1: cd\n2: help\n3: history\n4: jobs\n5: kill\n6: exit\n");
	printf("To execute command use following pattern\n");
	printf("cmd <arguments> and hit enter.\n");
	printf("For more info about commands see Manual using man <cmd> command\n");
	return 1;
}

int execute_cd(char **argv){
	if(argv[1]==NULL){
		fprintf(stderr,"Error: Directory name is needed for \"cd\" command.\n");
	}else{
		if(chdir(argv[1])!=0){
			perror("chdir error");
		}
	}
return 1;
}

int execute_jobs(char **argv,int flag){
	int i=0,j,pid=getpid();
	char cmd[100],*buf[50];
	char path1[100];
	sprintf(cmd,"ps --ppid %d",pid);
	FILE *fp=popen(cmd,"r");
	while (fgets(path1, 100, fp) != NULL){
		buf[i]=(char*)malloc(100);
		strcpy(buf[i],path1);    		
		//printf("%s", path1);
		i++;
	}
	if(flag==1){
		if(i<=2){
			pclose(fp);return 1;
		}
		printf(BOLDYELLOW"WARNING: "RESET" %d process running on background, kill them first\n\n",i-2);
	}
	if(i<=2){
		pclose(fp);
		return 1;
	}
	
	for(j=0;j<i-1;j++){
		printf("%s\n",buf[j]);
		free(buf[j]);
	}
	pclose(fp);
	if(flag==1) return 0;
	return 1;
}

int execute_kill(char **argv){
	if(argv[1]==NULL){
		fprintf(stderr,"process id is needed\n");
	}
	else{
		if(argv[1][0]!='%'){
			fprintf(stderr,"Give pid in form of kill %<pid>\n");
		}
		else{
			char *pid=calloc(30,sizeof(char));
			pid=strtok(argv[1],"%"); 
			if(pid==NULL){
				printf("Give pid in form of kill %<pid>\n");
				return 1;
			}
			if(kill(atoi(pid),SIGKILL)==-1){
				perror("kill");
			}
			//free(pid);
		}
		
	}
	return 1;
}

int execute_history(){
	struct history *p;
	p=head;
	while(p!=NULL){
		printf("%d: %s\n",p->id,p->data);
		p=p->next;
	}
	return 1;
}

int execute_exit(char **argv){
	
	int z=execute_jobs(argv,1);
	if(z==1){
		kill(kill_pid,SIGINT);
	}
	return 1;
}

int execute_else(char **argv){
	int status;
	pid_t pid,wpid;
	pid=fork();
	if(pid==0){
		//printf("%s %s %s\n",argv[0],argv[1],argv[2]);
		if(execvp(argv[0],argv)==-1){
			fprintf(stderr,"BShell: No such command found\n");
		}
		exit(0);
	}
	else if(pid<0){
		perror("Error in forking");
	}else{
		do{
			wpid=waitpid(pid,&status,WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}

int execute_redirect(char **argv,char *cmd){
	char infile[100],outfile[100];
	int i,status,flag=0;
	if(strchr(cmd,'<')!=NULL && strchr(cmd,'>')!=NULL){ 
		i=0;
		while(argv[i]!=NULL){
			if(strcmp(argv[i],"<")==0){
				strcpy(infile,argv[i+1]);argv[i]='\0';  i++;continue;
			}
			if(strcmp(argv[i],">")==0){
				strcpy(outfile,argv[i+1]);argv[i]='\0'; i++;continue;
			}
			i++;
		}
	}
	else if(strchr(cmd,'<')!=NULL){ 
		i=0;
		while(argv[i]!=NULL){
			if(strcmp(argv[i],"<")==0){
				strcpy(infile,argv[i+1]); argv[i]='\0';i++; break;
			}
			i++;
		}
	}
	else if(strchr(cmd,'>')!=NULL){
		i=0;
		while(argv[i]!=NULL){
			if(strcmp(argv[i],">")==0){
				strcpy(outfile,argv[i+1]); argv[i]='\0';i++; break;
			}
			i++;
		}
	}
	FILE *in,*out; 
	int indup,outdup;
	indup=dup(0);
	outdup=dup(1);
	if(in!=NULL){
		in=fopen(infile,"r");
		dup2(fileno(in),STDIN_FILENO);
		close(fileno(in));
	}
	if(out!=NULL){
		out=fopen(outfile,"w");
		dup2(fileno(out),STDOUT_FILENO);
		close(fileno(out));
	}

	pid_t pid,wpid;
	pid=fork();
	if(pid==0){
		//printf("%s %s %s\n",argv[0],argv[1],argv[2]);
		if(execvp(argv[0],argv)==-1){
			fprintf(stderr,"BShell: No such command found\n");
		}
		exit(0);
	}
	else if(pid<0){
		perror("Error in forking");
	}else{
		do{
			wpid=waitpid(pid,&status,WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
		if(in!=NULL) fclose(in);
		if(out!=NULL) fclose(out);
		dup2(indup,0);
		dup2(outdup,1);
		close(indup);
		close(outdup);
	}	
	return 1;
}

int history_again(char **argv){
	struct history *p;
	p=head;
	char cmd[100];
	int num,size,i;
	size=strlen(argv[0]);
	for(i=1;i<size;i++){
		cmd[i-1]=argv[0][i];
	}
	cmd[size-1]='\0';
	num=atoi(cmd);

	if(num==-1){
		strcpy(cmd,tail->data);
		//printf("cmd is %s\n",cmd);
		
	}else{
		while(p!=NULL){
			if(p->id==num){
				strcpy(cmd,p->data); //printf("cmd is %s\n",cmd);
				break;
			}
			p=p->next;
		}
		
	}
	return execute_cmd((parse_cmd(cmd))); 
}


int background(char **argv,int size){
	char cmd[100];
	int pid,i;
        
	argv[size-1]=NULL;
	strcpy(cmd,argv[0]);
	for(i=1;i<size-1;i++){
		strcat(cmd," ");
		strcat(cmd,argv[i]);
		
	}
	pid=fork();
	if(pid<0){fprintf(stderr,"fork error\n");}
	if(pid==0){
		if(execvp(argv[0],argv)==-1){
			fprintf(stderr,"BShell: No such command found\n");	
		}
	}
	printf(BOLDYELLOW"[%d] : %s\n"RESET,pid,cmd);
	return 1;
}


int main(){
	system("clear");
	welcome_msg();
	struct sigaction sa;
	int status,count=1,cnt=1;
	char *cmd2;
	struct history *hist;
	tail=NULL;
	head=NULL;
	
	do{	hist=(struct history*)malloc(sizeof(struct history));
		sa.sa_handler = sigchld_handler; // reap all dead processes
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART;
		if (sigaction(SIGCHLD, &sa, NULL) == -1) {
			perror("sigaction");
			exit(1);
		}
		char *cmd,**argv,cmd1[256];
		int i=0;
		char ppt[256];
		prompt(ppt);
		cmd=readline(ppt);
		if(strlen(cmd)<=0) continue;
		bzero(cmd1,sizeof(cmd1));
		strcpy(cmd1,cmd);
		int z1=strlen(cmd1);
		cmd1[z1]='\0';
		add_history(cmd);	
		argv=parse_cmd(cmd);
		if(strchr(argv[0],'!')!=NULL){//printf("inside\n");
			status=history_again(argv);
			continue;
		}else{
			hist->id=cnt++;
			strcpy(hist->data,cmd1);
			hist->next=NULL;
			if(tail==NULL){
				tail=hist;
				head=hist;
			}
			tail->next=hist;
			tail=hist;
		}
		
		if(count==HISTORY_MAX+1){
			clear_history1();
			count--;
		}
		
		if(strchr(cmd1,'<')!=NULL || strchr(cmd1,'>')!=NULL ){
			status=execute_redirect(argv,cmd1);
		}else{
			status=execute_cmd(argv);
		}
		free(argv);
		count++;

	}while(status);
	

return 0;
}
