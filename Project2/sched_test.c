#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>

#include <sys/syscall.h>
#include <sys/types.h>

#define NUM 40000

void* run(int job){
	for(int i = 0; i < NUM; i++)
		for(int j = 0; j < NUM; j++)
			;

	printf("Thread %ld has finished Job %d.\n", pthread_self(), job);

	return (void*) 0;
}

int main(int argc, char *argv[]){
	int s = 0;
	
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(0, &mask);
	s = sched_setaffinity(0, sizeof(mask), &mask);

	int max_thread = 1;

	if(argc > 1)
		max_thread = atoi(argv[1]);
	if(argc > 2){
		if(strcmp(argv[2], "SCHED_FIFO") == 0){
			struct sched_param param;
			param.sched_priority = 1;
			s = sched_setscheduler(0, SCHED_FIFO, &param);
			printf("Policy : SCHED_FIFO\n");
		}	
		else if(strcmp(argv[2], "SCHED_RR") == 0){
			struct sched_param param;
			param.sched_priority = 1;
			s = sched_setscheduler(0, SCHED_RR, &param);
			printf("Policy : SCHED_RR\n");
		}	
	}
	else{
		printf("Policy : Time Sharing\n");
	}
	
	pthread_t tid[max_thread];
	
	for(int i = 0; i < max_thread; i++){
		printf("Job %d start.\n", i);
		int err = pthread_create(&tid[i], NULL, (void*) run, (void*) i);
		assert(err == 0);
	}

	for(int i = 0; i < max_thread; i++)
		pthread_join(tid[i], NULL);

	return 0;
}
