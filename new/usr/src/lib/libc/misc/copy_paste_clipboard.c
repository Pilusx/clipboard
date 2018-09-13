#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "clipboard_protocol.h"

extern uint8_t check_if_ancestor(pid_t pid1, pid_t pid2);

void include_char(uint32_t* len, uint32_t* counter, uint32_t pos) {
    if (*counter == 0 && pos > 0) {
        (*len)++;
    } else {
        *len += *counter;
        *counter = 0;
    }
}

uint32_t expected_decompressed_size(const char* buffer, uint32_t buffer_len) {
    uint32_t len = 0;
    uint32_t current = 0;
    uint32_t pos = 0;

    for (; pos < buffer_len; pos++) {
        char c = buffer[pos];
        if (!isdigit(c)) {
            include_char(&len, &current, pos);
        } else {
            current = 10 * current + c - '0';
        }
    }

    include_char(&len, &current, pos);
    return len;
}

uint32_t paste_from_clipboard(uint32_t id, char* buffer, uint32_t buffer_len) {
    int ret;
    int fd;
    int locking_pid;
    int source_pid;
    struct clipboard_lock_msg ml;
    struct clipboard_get_msg mg;

    if ((fd = open("/dev/clipboard", O_RDWR)) < 0) {
        log_msg("open failed %d\n", fd);
        return -1;
    }

    if (id >= BUF_MAX_COUNT) {
        log_msg("Wrong id=%d, expected < %d\n", id, BUF_MAX_COUNT);
        return -1;
    }

    if ((source_pid = read(fd, &mg, ENCODE_ID(id))) < 0) {
        log_msg("Wrong id?\n");
        return -1;
    }
    locking_pid = getpid();

    if (!check_if_ancestor(source_pid, locking_pid)) {
        log_msg("You don't have permission to lock this entry\n");
        return -1;
    }

    ml.pid = locking_pid;
    ml.id = id;
    ml.buffer_len = buffer_len;
    if ((ret = write(fd, &ml, LOCK_SIZE)) < 0) return ret;

    memset(&mg, 0, GET_SIZE);
    if ((ret = read(fd, &mg, ENCODE_PID(ml.pid))) < 0) return ret;

    memcpy(buffer, mg.buffer, mg.buffer_len);
    close(fd);

    return ret;
}

uint32_t copy_to_clipboard(char* buffer, uint32_t buffer_len) {
    int fd;
    int ret;
    struct clipboard_set_msg ms;

    if (buffer_len > ENTRY_LEN_LIMIT) {
        return -1;
    }

    if ((fd = open("/dev/clipboard", O_WRONLY)) < 0) {
        log_msg("open failed %d\n", fd);
        return -1;
    }

    ms.pid = getpid();
    ms.buffer_len = buffer_len;
    ms.decompressed_len = expected_decompressed_size(buffer, buffer_len);

    memcpy(ms.buffer, buffer, buffer_len);
    ret = write(fd, &ms, SET_SIZE);

    close(fd);

    return ret;
}
