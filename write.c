#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "syn.h"
#include "def.h"

int main(void)
{
    int pgSize;
    struct  shmcont *shm;
    struct test *t1;
    struct rwlock *l1;
    //int mut;
    pgSize = getpagesize ();
    shm = (struct shmcont*)creat_shm (SHM_KEY, pgSize);
    
    if (shm == (struct shmcont*)-1) {
        perror ("creat shm");
        exit (-1);
    }

    t1 = &(shm->shm_test);
    l1 = &(shm->shm_rwlock);
    
    if (init_rwlock (l1, RL_KEY, WL_KEY, PW_KEY) == -1) {
        perror ("Creat lock");
    }

    while (1) {
        rw_wrt (l1);   
        t1->a = 1;
        t1->b = 2;
        t1->c = 3;
        t1->d = 4;
        puts ("wrote1");
        rw_wrt_end (l1);

        //sleep (1);
        rw_wrt (l1);
        t1->a *= 2;
        t1->b *= 2;
        t1->c *= 2;
        t1->d *= 2;
        puts ("wrote2");
        rw_wrt_end (l1);
    }
    return 0;
}
