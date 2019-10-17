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
    struct shmcont *shm;
    struct test *t1;
    struct rwlock *l1;
    pgSize = getpagesize ();

    shm = (struct shmcont*)get_shm (SHM_KEY, pgSize);
    if (shm == (struct shmcont*)-1) {
        perror ("get shm");
        exit (-1);
    }
    
    t1 = &(shm->shm_test);
    l1 = &(shm->shm_rwlock);

    while (1) {
        rw_rd (l1);
        printf ("%d, %d, %d, %d\n", t1->a, t1->b, t1->c, t1->d);
        sleep(1);
        rw_rd_end (l1);
    }
    return 0;
}
