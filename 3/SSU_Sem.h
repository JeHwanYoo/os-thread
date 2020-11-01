#include <pthread.h>

#define MAX_THREAD_LIST 1000

typedef struct SSU_Sem {
	int value;
	int last;
	pthread_t thread_list[MAX_THREAD_LIST];
} SSU_Sem;

void SSU_Sem_init(SSU_Sem *, int);
void SSU_Sem_up(SSU_Sem *);
void SSU_Sem_down(SSU_Sem *);
