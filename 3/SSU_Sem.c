#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include "SSU_Sem.h"

void signal_handler() {}

void SSU_Sem_init(SSU_Sem *s, int value) {
	s = malloc(sizeof(SSU_Sem)); 
	s->value = value;
	s->length = 0;
	s->thread_list = NULL;
	s->first = NULL;
}

void SSU_Sem_down(SSU_Sem *s) {
	int i;
	struct sigaction sig_act;
	sigset_t sig_set;
	T_NODE *tmp;
	s->value--;
	if (s->value < 0) {
		tmp = s->thread_list;
		s->thread_list = malloc(sizeof(T_NODE));
		s->thread_list->tid = pthread_self();
		if (s->length++ == 0) {
			s->first = s->thread_list;
		} else {
			tmp->next = s->thread_list;
		}
		sigemptyset(&sig_act.sa_mask);
		sig_act.sa_flags = 0;
		sig_act.sa_handler = signal_handler;
		sigaction(SIGUSR1, &sig_act, NULL);
		sigsuspend(&sig_act.sa_mask);
	}
}

void SSU_Sem_up(SSU_Sem *s) {
	T_NODE *tmp;
	if (s->first != NULL) {
		pthread_kill(s->first->tid, SIGUSR1);
		tmp = s->first;
		s->first = s->first->next;
		free(tmp);
		s->length--;
	}
}

/**
 *	Free Semaphore
 *	if parameter 'free_sempahore' == 0 then s will not be free
 **/
void SSU_Sem_free(SSU_Sem *s, int free_semaphore) {
	T_NODE *next, *tmp_next, *tmp_s;
	next = s->first;
	while (next != NULL) {
		tmp_next = next;
		next = next->next;
		free(tmp_next);
	}
	s->thread_list = NULL;
	s->first = NULL;
	if (free_semaphore) {
		free(s);
		s = NULL;
	}
}
