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
    struct test *t1;
    int mut;
    pgSize = getpagesize ();

    t1 = (struct test*)get_shm (SHM_KEY, pgSize);
    if (t1 == (struct test*)-1) {
        perror ("get shm");
        exit (-1);
    }
    mut = get_mut (SEM_KEY);
    if (mut == -1) {
        perror ("get mut");
        exit (-1);
    }

    while (1) {
        mut_lock (mut);
        printf ("%d, %d, %d, %d\n", t1->a, t1->b, t1->c, t1->d);
        sleep(1);
        mut_rel (mut);
    }
    return 0;
}
