#include "clipboard.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void print_string(const char* c, uint32_t len) {
    for(int i = 0; i < len; i++)
        printf("%c", c[i]);

    printf(" |LEN=%d| ", len);
}


int32_t (*clipboard_calls[CLIPBOARD_CALL_NUM])(void* msg) = {
        clipboard_read,
        clipboard_write,
};

clipboard_t CLIPBOARD;

void use_entry(uint32_t id, const char * buffer, uint32_t buffer_len) {
    assert(!busy(id));
    entry_buffer(id) = (char*) malloc(sizeof(char) * buffer_len);
    memcpy(entry_buffer(id), buffer, buffer_len);
    entry(id).buffer_len = buffer_len;
    slot(id) = USED;
}

void delete_entry(uint32_t id) {
    assert(busy(id));
    free(entry_buffer(id));
    slot(id) = EMPTY;
}


int32_t clipboard_write(void * msg) { // const char* buffer, uint32_t buffer_len)
    clipboard_write_msg_ptr m = (clipboard_write_msg_ptr) msg;
    const char * buffer = m->buffer;
    uint32_t buffer_len = m->buffer_len;

    if(is_empty(&CLIPBOARD.available_entries)) { return -1; }

    int id = top(&CLIPBOARD.available_entries);
    pop(&CLIPBOARD.available_entries);

    if(busy(id)) { return -1; }

    use_entry(id, buffer, buffer_len);
    entry(id).decompressed_len = m->decompressed_len;

    return id;
}

int32_t clipboard_read(void *msg) { // uint32_t id, char* buffer, uint32_t buffer_len)
    clipboard_read_msg_ptr m = (clipboard_read_msg_ptr) msg;
    uint32_t id = m->id;
    char* buffer = m->buffer;
    uint32_t buffer_len = m->buffer_len;


    if(!busy(id)) { return -1; }
    if(buffer_len < entry_len(id)) { return -1; }

    memcpy(buffer, entry_buffer(id), entry_len(id));
    delete_entry(id);
    return OK;
}


int32_t clipboard_ioctl(int32_t request, void* msg) {
    if(request < CLIPBOARD_CALL_NUM) {
        return clipboard_calls[request](msg);
    }
    return -1;
}

void clipboard_log() {
    static int k = 0;

    printf("LOG %d \n", k++);
    for(int i = 0; i < BUF_MAX_COUNT; i++) {
        if(busy(i)) {
            printf("%d (LE=%d, DE=%d):  ", i, entry_len(i), entry(i).decompressed_len);
            print_string(entry_buffer(i), entry_len(i));
            printf("\n");
        }
    }
}

void clipboard_init() {
    init(&CLIPBOARD.available_entries);
    for(int i = 0; i < BUF_MAX_COUNT; i++) {
        slot(i) = EMPTY;
    }
}
