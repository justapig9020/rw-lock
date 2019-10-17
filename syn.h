#ifndef _SYN_H_
#define _SYN_H_

#include <sys/types.h>

#define mut_t int
#define SHM_KEY (key_t)666
#define SEM_KEY (key_t)667
#define RL_KEY (key_t)668
#define WL_KEY (key_t)669
#define PW_KEY (key_t)670

struct rwlock {
    int rNum;
    mut_t rlock;
    mut_t wlock;
    mut_t pwait;
};

void *creat_shm (key_t, int);
void *get_shm (key_t, int);
int del_shm (key_t, int);

mut_t creat_mut (key_t);
mut_t get_mut (key_t);
int mut_lock (int); // use semaphore
int mut_rel (int); // release semaphore
int del_mut (int);

int init_rwlock (struct rwlock*, key_t, key_t, key_t);
int rw_rd (struct rwlock*);
int rw_wrt (struct rwlock*);
int rw_rd_end (struct rwlock*);
int rw_wrt_end (struct rwlock*);
int del_rwlock (struct rwlock*);
#endif
