#include <lib.h>
#include <sys/types.h>
#include <unistd.h>

uint8_t check_if_ancestor(pid_t pid1, pid_t pid2) {
    message m;
    m.m1_i1 = pid1;
    m.m1_i2 = pid2;

    return _syscall(PM_PROC_NR, PM_CHECKANCESTOR, &m);
}
