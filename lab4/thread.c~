#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;
int c;

void *fun1(void *arg){
	//int c=atoi(c);
	printf("fun1 started\n");
	//pthread_mutex_lock(&m);
	c=c+1; printf("inside fun1 %d\n",c);
	//pthread_mutex_unlock(&m);
	printf("fun1 ended\n");
	return NULL;
}
void *fun2(void *arg){
	//int c=atoi(c);
	printf("fun2 started\n");
	//pthread_mutex_lock(&m);
	c=c-1; printf("inside fun2 %d\n",c);
	//pthread_mutex_unlock(&m);
	printf("fun2 ended\n");
	return NULL;
}
void *fun3(void *arg){
	//int c=atoi(c);
	printf("fun3 started\n");
	//pthread_mutex_lock(&m);
	c=c*2; printf("inside fun3 %d\n",c);
	//pthread_mutex_unlock(&m);
	printf("fun3 ended\n");
	return NULL;
}
void *fun4(void *arg){
	//int c=atoi(c);
	printf("fun4 started\n");
	//pthread_mutex_lock(&m);
	c=c/2; printf("inside fun4 %d\n",c);
	//pthread_mutex_unlock(&m);
	printf("fun4 ended\n");
	return NULL;
}


int main(int argc,char *argv[]){
	c=10;	
	pthread_t tid1,tid2,tid3,tid4;
	//pthread_attr_t attr;

	if (pthread_mutex_init(&m, NULL) != 0)
	    {
		printf("\n mutex init failed\n");
		return 1;
	    }

	
	//if(argc!=3){
	//	fprintf(stderr,"enter two values for calculation\n");
	//	exit(0);
	//}
	//if(atoi(argv[1]<0 || atoi(argv[2])<0){
	//	fprintf(stderr,"need positive numbers\n");
	//	exit(0);
	//}
	pthread_create(&tid1,NULL,&fun1,NULL);
	pthread_create(&tid2,NULL,&fun2,NULL);
	pthread_create(&tid3,NULL,&fun3,NULL);
	pthread_create(&tid4,NULL,&fun4,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);
	pthread_join(tid4,NULL);
	printf("final value of c is :%d\n",c);
	pthread_mutex_destroy(&m);
	

return 0;
}
