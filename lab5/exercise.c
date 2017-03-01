#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/time.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>

#define max 1000000
struct timeval startread,startcalc,readtime,finish,calctime,overalltime;
char buf1[max][50],buf[max];
int count=0,idx=1;
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;


void *thread_fun(void *bkm){
	int size=atoi(buf1[0]); //printf("size is %d\n",size);

	while(idx < size){//printf("string is %s\n",buf1[idx]);
		if(atoi(buf1[idx])==3){	
			pthread_mutex_lock(&lock);	 
			count++; //printf("inside\n");
			pthread_mutex_unlock(&lock);
		}
		idx++;
	}
}

int main(int argc,char *argv[]){
	if(argc!=2){
		fprintf(stderr,"Enter Max number of threads\n");
		exit(0);
	}

	int k,i,j,size;
	size=atoi(argv[1]);
	int fp1=open("data.txt",O_WRONLY|O_CREAT,0644);
	if(fp1==-1){ perror("file cannot open");}
	for(k=0;k<size;k++){
		count=0;idx=1;
		pthread_t thread_id[size];
		if (pthread_mutex_init(&lock, NULL) != 0){
			printf("\n mutex init failed\n");
			return 1;
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
		//printf("final result is %d\n",count);
		//printf("read time is %d\n",readtime.tv_usec);
		//printf("calc time is %d\n",calctime.tv_usec);
		//printf("overall time is %d\n",overalltime.tv_usec);
		//free(buf1);
		pthread_mutex_destroy(&lock);
		//writing output to data.txt file to plot
		char threads[10],time[10],temp[20];
		sprintf(threads,"%d",k);
		sprintf(time,"%d",overalltime.tv_usec);
		strcpy(temp,threads); strcat(temp," "); strcat(temp,time); strcat(temp," \n");
		int sss=strlen(temp);
		char temp1[sss];
		strcpy(temp1,temp);		
		write(fp1,temp1,sizeof(temp1));
		
	}
	close(fp1);
	system("gnuplot > load 'plot'");
return 0;
}
