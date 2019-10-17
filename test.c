#include <sys/sem.h>
#include "syn.h"

int main(void)
{
    struct a{
        int b;
    } c;
    c.b = creat_mut (SEM_KEY);
    return 0;
}
