#include <pthread.h>

#define MAX_THREAD_LIST 1000

typedef struct thread_node {
	struct thread_node *next;
	pthread_t tid;
} T_NODE;

typedef struct SSU_Sem {
	int value;
	int length;
	// int last;
	// pthread_t thread_list[MAX_THREAD_LIST];
	T_NODE *thread_list;
	T_NODE *first;
} SSU_Sem;

void SSU_Sem_init(SSU_Sem *, int);
void SSU_Sem_up(SSU_Sem *);
void SSU_Sem_down(SSU_Sem *);
void SSU_Sem_free(SSU_Sem *, int); // free semaphore
