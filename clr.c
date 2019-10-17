#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "def.h"
#include "syn.h"

int main(void)
{
    long pgsize = sysconf (_SC_PAGESIZE);
    del_mut (SEM_KEY);
    del_shm (SHM_KEY, pgsize);
    del_mut (RL_KEY);
    del_mut (WL_KEY);
    del_mut (PW_KEY);
    return 0;
}
