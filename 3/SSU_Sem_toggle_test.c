#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include "SSU_Sem.h"

#define NUM_THREADS 3
#define NUM_ITER 10

SSU_Sem sem[NUM_THREADS];

int count = NUM_THREADS;
int next;

void *justprint(void *data)
{
	int thread_id = *((int *)data);

	for(int i = 0; i < NUM_ITER; i++)
	{
		count--;
		SSU_Sem_down(&sem[thread_id]);
		printf("This is thread %d\n", thread_id);
		if (thread_id + 1 != NUM_THREADS) {
			SSU_Sem_up(&sem[thread_id + 1]);
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{

	pthread_t mythreads[NUM_THREADS];
	int mythread_id[NUM_THREADS];
	int i, j;

	for(i = 0; i < NUM_THREADS; i++) {
		SSU_Sem_init(&sem[i], 0);
	}

	for(i = 0; i < NUM_THREADS; i++)
	{
		mythread_id[i] = i;
		pthread_create(&mythreads[i], NULL, justprint, (void *)&mythread_id[i]);
	}

	i = 0;
	while (1) {
		usleep(1000);
		if (i != NUM_ITER && count == 0) 
		{
			count = NUM_THREADS;
			i++;
			SSU_Sem_up(&sem[0]);
		}
		else if (i == NUM_ITER)
		{
			break;
		}
	}

	for(i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(mythreads[i], NULL);
	}

	return 0;
}
