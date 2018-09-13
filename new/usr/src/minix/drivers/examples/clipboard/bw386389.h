#ifndef _BW386389_H_
#define _BW386389_H_

#include <stdint.h>
#include <sys/ioctl.h>
#include "stack.h"
#include "clipboard_protocol.h"

#define OK 0

typedef struct Entry {
    uint32_t buffer_len;
    uint32_t decompressed_len;
    pid_t source_pid;
    pid_t locking_pid;
    char* buffer;
    char in_use;
} entry_t;


typedef struct Clipboard {
    entry_t entries[BUF_MAX_COUNT];
    struct Stack available_entries;
} clipboard_t;


#define entry(id) CLIPBOARD.entries[(id)]
#define entry_len(id) entry(id).buffer_len
#define entry_buffer(id) entry(id).buffer
#define EMPTY 0
#define USED 1
#define LOCKED 2
#define slot(id) (entry((id)).in_use)
#define busy(id) (slot(id) == USED)
#define lock(id) {slot(id) = LOCKED;}
#define unlock(id) {slot(id) = USED;}
#define locked(id) (slot(id) == LOCKED)
#define lock_by(id, pid) {lock(id); entry((id)).locking_pid = (pid);}

#endif // _BW386389_H_
