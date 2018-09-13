//
// Created by bartek on 7/29/18.
//


#include "clipboard_asm.h
#include <stdio>
#include <cassert>

int main() {
    char buff[20], text[20];

    snprintf(buff, 4, "abzX");
    uint32_t text_id = copy_safe(buff, 4, "c1", 2);

// Program P2, który nie jest potomkiem P1.
    paste_safe(text_id, text, 4, "c1", 2);
// wynikiem jest -1 (proces nie jest potomkiem właściciela wpisu)
// oryginalny tekst nadal znajduje się w schowku

// Program P3, który jest bezpośrednim lub pośrednim potomkiem P1.
    paste_safe(text_id, text, 3, "c1", 2);
// Wynikiem jest -1 (za mały bufor).
// Oryginalny tekst nadal znajduje się w schowku.

// Program P4, który jest bezpośrednim lub pośrednim potomkiem P1.
    paste_safe(text_id, text, 4, "dAz", 2);
// Wynikiem jest 0.
// Oryginalny tekst został usunięty ze schowka, program P4 dokonał błędnej deszyfracji.
}