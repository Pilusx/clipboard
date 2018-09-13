//
// Created by bartek on 7/29/18.
//

#ifndef SO_7_CLIPBOARD_DEV_H
#define SO_7_CLIPBOARD_DEV_H

#include <stdint.h>
#include "stack.h"

#define OK 0
#define CLIPBOARD_READ 0
#define CLIPBOARD_WRITE 1
#define CLIPBOARD_CALL_NUM 2
#define BUF_MAX_COUNT 100

extern int32_t (*clipboard_calls[CLIPBOARD_CALL_NUM])(void* msg);

typedef struct Entry {
    uint32_t buffer_len;
    char* buffer;
    uint32_t decompressed_len;
    char in_use;
} entry_t;
#define entry(id) CLIPBOARD.entries[(id)]
#define entry_len(id) entry(id).buffer_len
#define entry_buffer(id) entry(id).buffer

#define USED 1
#define EMPTY 0
#define slot(id) (entry((id)).in_use)
#define busy(id) (slot(id) == USED)



typedef struct clipboard_read_msg {
    char *buffer;
    uint32_t buffer_len;
    uint32_t id;
    uint32_t decompressed_len;
}* clipboard_read_msg_ptr;

typedef struct clipboard_write_msg {
    const char *buffer;
    uint32_t buffer_len;
    uint32_t decompressed_len;
}* clipboard_write_msg_ptr;

typedef struct Clipboard {
    entry_t entries[BUF_MAX_COUNT];
    struct Stack available_entries;
} clipboard_t;


void use_entry(uint32_t id, const char * buffer, uint32_t buffer_len);

void delete_entry(uint32_t id);




int32_t clipboard_ioctl(int32_t request, void* msg);

void clipboard_log();

void clipboard_init();

int32_t clipboard_write(void *msg);
int32_t clipboard_read(void *msg);


#endif //SO_7_CLIPBOARD_DEV_H
