//
// Created by bartek on 7/29/18.
//

#ifndef SO_7_UNISTD_H
#define SO_7_UNISTD_H

#include "clipboard.h"

typedef int pid_t;

uint8_t check_if_ancestor(pid_t pid1, pid_t pid2) {
    return 1;
}

int32_t copy_to_clipboard(const char *buffer, uint32_t buffer_len) {
    // system_call(pm, ...)
    struct clipboard_write_msg m;
    m.buffer = buffer;
    m.buffer_len = buffer_len;
    m.decompressed_len = expected_decompressed_size(buffer, buffer_len);

    return clipboard_ioctl(CLIPBOARD_WRITE, &m);
}

uint32_t paste_from_clipboard(uint32_t id, char *buffer, uint32_t * buffer_len) {
    struct clipboard_read_msg m;
    m.buffer = buffer;
    m.buffer_len = *buffer_len;
    m.id = id;

    uint32_t res = clipboard_ioctl(CLIPBOARD_READ, &m);
    *buffer_len = m.buffer_len;
    return res;
}


#endif //SO_7_UNISTD_H
