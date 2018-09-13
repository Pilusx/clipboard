//
// Created by bartek on 8/29/18.
//

#ifndef _COPY_PASTE_CLIPBOARD_H_
#define _COPY_PASTE_CLIPBOARD_H_

#include "clipboard.h"

int32_t copy_to_clipboard(const char *buffer, uint32_t buffer_len);

uint32_t paste_from_clipboard(uint32_t id, char *buffer, uint32_t* buffer_len);

#endif // _COPY_PASTE_CLIPBOARD_H_
