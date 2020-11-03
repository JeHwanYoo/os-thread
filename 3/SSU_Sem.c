#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include "SSU_Sem.h"

void signal_handler() {}

void SSU_Sem_init(SSU_Sem *s, int value) {
	s = malloc(sizeof(SSU_Sem)); 
	s->value = value;
	s->last = -1;
	memset(s->thread_list, 0, MAX_THREAD_LIST);
}

void SSU_Sem_down(SSU_Sem *s) {
	s->value--;
	int i;
	struct sigaction sig_act;
	sigset_t sig_set;
	if (s->value < 0) {
		s->thread_list[++s->last] = pthread_self();
		sigemptyset(&sig_act.sa_mask);
		sig_act.sa_flags = 0;
		sig_act.sa_handler = signal_handler;
		sigaction(SIGUSR1, &sig_act, NULL);
		sigsuspend(&sig_act.sa_mask);
	}
}

void SSU_Sem_up(SSU_Sem *s) {
	s->value++;
	pthread_kill(s->thread_list[s->last], SIGUSR1);
	s->thread_list[s->last] = 0; // pop 
	s->last--;
}

