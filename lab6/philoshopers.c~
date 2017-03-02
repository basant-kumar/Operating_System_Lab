#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/time.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<semaphore.h>
#include<sys/sem.h>

char state[3][10];
//sem_t chopstick[5];
pthread_mutex_t chopstick[5]=PTHREAD_MUTEX_INITIALIZER;

void *fun(int j){
	printf("hungry: %d\n",j);	
	
	if(j%2==0){
		pthread_mutex_lock(&chopstick[j]);//sleep(5);
		pthread_mutex_lock(&chopstick[(j+1)%5]);
	}else{
		pthread_mutex_lock(&chopstick[(j+1)%5]);//sleep(5);
		pthread_mutex_lock(&chopstick[j]);
	}
	//sleep(10);
	
	printf("Started Eating: %d\n",j);
	//eating
	sleep(5);
	printf("Finish Eating: %d\n",j);
	pthread_mutex_unlock(&chopstick[j]);
	pthread_mutex_unlock(&chopstick[(j+1)%5]);
	printf("thinking: %d\n"); 
}

int main(int argc,char *argv[]){
	if(argc!=2){fprintf(stderr,"Enter number of threads\n"); exit(1);}
	int size,i;
	size=atoi(argv[1]);
	//printf("size is %d\n",size);
	pthread_t thread_id[size];
	
	printf("Everyone is thinking\n");
	for(i=0;i<5;i++){
		strcpy(state[i],"thinking");
		pthread_mutex_init(&chopstick[i], NULL);
	}	
	
	for(i=0;i<size;i++){
			sleep(2);
			pthread_create(&thread_id[i],NULL,&fun,i);
			
		}
	for(i=0;i<size;i++){
		pthread_join(thread_id[i],NULL);
	}
return 0;
}
