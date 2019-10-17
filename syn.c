#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include "syn.h"

#define DEBUG 0

#if DEBUG
    #define debug(x, ...) printf (x, __VA_ARGS__) 
#else
    #define debug(x, ...)
#endif


union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
							    (Linux-specific) */
};

void *creat_shm (key_t key, int size)
{
    int id;
    void *addr;
    id = shmget (key, size, 0644 | IPC_CREAT | IPC_EXCL);
    if (id == -1)
        return (void *)-1;
    addr = shmat (id, NULL, 0);
    memset (addr, size, 0);
    return addr;
}

void *get_shm (key_t key, int size)
{
    int id;
    id = shmget (key, size, 0644 | IPC_CREAT);
    if (id == -1)
        return (void *)-1;
    return shmat (id, NULL, 0);
}

int del_shm (key_t key, int size)
{
    int shmid;
    shmid = shmget (key, size, 0644);
    if (shmid == -1)
        return -1;

    if (shmctl (shmid, IPC_RMID, 0) == -1)
        return -1;
    return 0;
}

int creat_mut (key_t key)
{
    int id;
    union semun sem_union;
    id = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
    if (id == -1)
        return -1;
    sem_union.val = 1;
    if (semctl(id, 0, SETVAL, sem_union) == -1) {
        return -1;
    }
    return id;
}

int get_mut (key_t key)
{
    return semget(key, 1, 0666 | IPC_CREAT);
}

int mut_lock (int mid)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;
    sem_b.sem_flg = SEM_UNDO;

    debug ("locking");
    if (semop(mid, &sem_b, 1) == -1) {
        perror("semaphore_p lock failed");
        return -1;
    }
    debug ("locked");
    return 0;
}

int mut_rel (int mid)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;
    sem_b.sem_flg = SEM_UNDO;

    debug ("releasing");
    if (semop(mid, &sem_b, 1) == -1) {
        perror("semaphore_p rel failed");
        return -1;
    }
    debug ("released");
    return 0;
}

int mut_wait (int mid)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;
    sem_b.sem_flg = 0;

    debug ("waiting");
    if (semop(mid, &sem_b, 1) == -1) {
        perror("semaphore_p wait failed");
        return -1;
    }
    debug ("got it");
    return 0;
}

int del_mut (key_t key)
{
    int id;
	union semun sem_union;

    id = get_mut (key);
    if (id == -1)
        return -1;
    
    if (semctl(id, 0, IPC_RMID, sem_union) == -1) 
        return -1;
    
    return 0;
}

int init_rwlock (struct rwlock *lock, key_t k1, key_t k2, key_t k3)
{
    lock->rNum = 0;
    lock->rlock = creat_mut (k1);
    if (lock->rlock == -1)
        return -1;
    lock->wlock = creat_mut (k2);
    if (lock->wlock == -1)
        return -1;
    lock->pwait = creat_mut (k3);
    if (lock->pwait == -1)
        return -1;
    return 0;
}

int rw_rd (struct rwlock *lock)
{
    //mut_wait (lock->pwait);
    mut_lock (lock->pwait);
    mut_rel (lock->pwait);
    mut_lock (lock->rlock);
    lock->rNum++;
    if (lock->rNum == 1) {
        mut_lock (lock->wlock);
    }
    mut_rel (lock->rlock);
    return 0;
}

int rw_wrt (struct rwlock *lock)
{
    mut_lock (lock->pwait);
    mut_lock (lock->wlock);
    mut_rel (lock->wlock);
    //mut_wait (lock->wlock);
    return 0;
}

int rw_rd_end (struct rwlock *lock)
{
    mut_lock (lock->rlock);
    lock->rNum--;
    if (lock->rNum == 0) {
        mut_rel (lock->wlock);
    }
    mut_rel (lock->rlock);
    return 0;
}

int rw_wrt_end (struct rwlock *lock)
{
    mut_rel (lock->pwait);
    return 0;
}

int del_rwlock (struct rwlock *lock)
{
    del_mut (lock->rlock);
    del_mut (lock->wlock);
    del_mut (lock->pwait);
    return 0;
}
