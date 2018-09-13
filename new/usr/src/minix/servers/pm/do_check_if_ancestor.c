#include "pm.h"
#include "mproc.h"

#define pid(i) (mproc[(i)].mp_pid)
#define parent(i) (mproc[(i)].mp_parent)
#define active(i) (mproc[(i)].mp_flags & IN_USE)

int do_checkifancestor(void) {
    pid_t pid1 = m_in.m1_i1;
    pid_t pid2 = m_in.m1_i2;

    if(pid1 == pid2) {return 1;}

    int current_idx = 0;
    int found = 0;
    int last_idx = 0;

    for(int i = 0; i < NR_PROCS; i++) {
        if(active(i) && pid(i) == pid2) {
            current_idx = i;
            found = 1;
            break;
        }
    }

    if(found == 0) { return 0;}

    do {
        last_idx = current_idx;
        current_idx = parent(last_idx);

        if(pid(current_idx) == pid1) { return 1; }
    } while(pid(current_idx) != 1);

    return 0;
}
