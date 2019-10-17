#ifndef _DEF_H_
#define _DEF_H_

#include "syn.h"

struct test {
    int a;
    int b;
    int c;
    int d;
};

struct shmcont {
    struct test shm_test;
    struct rwlock shm_rwlock;
};

#endif
