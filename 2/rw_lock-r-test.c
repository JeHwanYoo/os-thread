#include "rw_lock.h"

void init_rwlock(struct rw_lock * rw)
{
	rw = malloc(sizeof(struct rw_lock));
	rw->lock = 0;
	rw->read_length = 0;
}

void r_lock(struct rw_lock * rw)
{
	while (rw->lock == -1) {}
	rw->lock = 1;
	rw->read_length++;
}

void r_unlock(struct rw_lock * rw)
{
	rw->read_length--;
	if (rw->read_length == 0) {
		rw->lock = 0;
	}
}

void w_lock(struct rw_lock * rw)
{
	while (rw->lock != 0) {}
	rw->lock = -1;
}

void w_unlock(struct rw_lock * rw)
{
	rw->lock = 0;
}
