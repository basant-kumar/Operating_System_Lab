#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/time.h>
#include<fcntl.h>
#include<unistd.h>

struct timeval startread,startcalc,readtime,finish,calctime,overalltime;
char buf1[256][50],buf[1024],idx=1;
int count=0;
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;


void *thread_fun(void *bkm){
	int size=atoi(buf1[0]);
	pthread_mutex_lock(&lock);
	while(idx < size){//printf("string is %s\n",buf1[idx]);
		if(strcmp(buf1[idx],"3")==0){		 
			count++; //printf("inside\n");
		}
		idx++;
	}
	pthread_mutex_unlock(&lock);
}

int main(int argc,char *argv[]){
	int i,j,size;
	size=atoi(argv[1]);
	pthread_t thread_id[size];
	if (pthread_mutex_init(&lock, NULL) != 0){
		printf("\n mutex init failed\n");
		return 1;
	}
	if(argc!=2){
		fprintf(stderr,"Enter number of thread ?\n");
		exit(0);
	}
	gettimeofday(&startread, NULL);

	int fp=open("a.txt",O_RDONLY);
	read(fp,buf,sizeof(buf));
	//printf("buf is %s\n",buf);
	char *ele;
	ele=strtok(buf,"\n");
	j=0;
	while(ele!=NULL){
		strcpy(buf1[j++],ele);
		ele=strtok(NULL,"\n");
	}
	close(fp);
	
	
	gettimeofday(&startcalc, NULL);
	//count the number of 3s in the input data
	for(i=0;i<size;i++){
		pthread_create(&thread_id[i],NULL,&thread_fun,NULL);
	}
	for(i=0;i<size;i++){
		pthread_join(thread_id[i],NULL);
	}

	gettimeofday(&finish, NULL);
	timersub(&startcalc, &startread, &readtime);
	timersub(&finish, &startcalc, &calctime);
	timersub(&finish, &startread, &overalltime);
	//output result
	printf("final result is %d\n",count);
	printf("read time is %d\n",readtime.tv_usec);
	printf("calc time is %d\n",calctime.tv_usec);
	printf("overall time is %d\n",overalltime.tv_usec);
	//free(buf1);
	pthread_mutex_destroy(&lock);

return 0;
}
