#ifndef _CLIPBOARD_PROTOCOL_H_
#define _CLIPBOARD_PROTOCOL_H_

#include <stdint.h>

#ifdef DEBUG
#define log_msg(...) {printf("[%s] ", __FUNCTION__); printf(__VA_ARGS__);}
#else
#define log_msg(...) {}
#endif

#define LOCK_SIZE	(sizeof(struct clipboard_lock_msg))
#define GET_SIZE	(sizeof(struct clipboard_get_msg))
#define SET_SIZE	(sizeof(struct clipboard_set_msg))
#define ENTRY_LEN_LIMIT 1024
#define BUF_MAX_COUNT 100

#define IS_PID_QUERY(size) (size < GET_SIZE ? 1 : 0)
#define IS_CORRECT_ID(id) (size < BUF_MAX_COUNT ? 1 : 0)
#define ENCODE_ID(id) (id)
#define DECODE_ID(size) (size)

#define ENCODE_PID(pid) (GET_SIZE + (pid))
#define DECODE_PID(size)  ((size) - GET_SIZE)


struct clipboard_lock_msg {
    uint32_t pid;
    uint32_t id;
    uint32_t buffer_len;
};
typedef struct clipboard_lock_msg* clipboard_lock_msg_ptr;

struct clipboard_get_msg {
    uint32_t buffer_len;
    uint32_t decompressed_len;
    char buffer[ENTRY_LEN_LIMIT];
};
typedef struct clipboard_get_msg* clipboard_get_msg_ptr;

struct clipboard_set_msg {
    uint32_t pid;
    uint32_t buffer_len;
    uint32_t decompressed_len;
    char buffer[ENTRY_LEN_LIMIT];
};
typedef struct clipboard_set_msg* clipboard_set_msg_ptr;

struct clipboard_write_msg {
    union {
        struct clipboard_lock_msg lock;
        struct clipboard_set_msg set;
    } u;
};
typedef struct clipboard_write_msg* clipboard_write_msg_ptr;


#endif // __CLIPBOARD_PROTOCOL_H_
