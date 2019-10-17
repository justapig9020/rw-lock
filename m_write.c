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
    t1 = (struct test*)creat_shm (SHM_KEY, pgSize);
    if (t1 == (struct test*)-1) {
        perror ("Creat shm");
        exit (-1);
    }

    mut = creat_mut (SEM_KEY);
    if (mut == -1) {
        perror ("Creat mut");
    }

    while (1) {
        mut_lock (mut);
        t1->a = 1;
        t1->b = 2;
        t1->c = 3;
        t1->d = 4;
        puts ("wrote1");
        mut_rel (mut);
        mut_lock (mut);
        t1->a *= 2;
        t1->b *= 2;
        t1->c *= 2;
        t1->d *= 2;
        puts ("wrote2");
        mut_rel (mut);
    }
    return 0;
}
