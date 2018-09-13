#include "copy_paste_clipboard.h"
#include "clipboard.h"


extern uint32_t expected_decompressed_size(const char*, uint32_t);

int32_t copy_to_clipboard(const char *buffer, uint32_t buffer_len) {
    struct clipboard_write_msg msg;
    msg.buffer_len = buffer_len;
    msg.buffer = buffer;
    msg.decompressed_len = expected_decompressed_size(buffer, buffer_len);
    //printf("DEC=%d\n", msg.decompressed_len);

    return clipboard_ioctl(CLIPBOARD_WRITE, &msg);
}

uint32_t paste_from_clipboard(uint32_t id, char *buffer, uint32_t* buffer_len) {
    struct clipboard_read_msg msg;
    msg.buffer = buffer;
    msg.id = id;
    msg.buffer_len = *buffer_len;
    //printf("BL=%d\n", msg.buffer_len);

    uint32_t res = clipboard_ioctl(CLIPBOARD_READ, &msg);
    *buffer_len = msg.buffer_len;
    return res;
}
