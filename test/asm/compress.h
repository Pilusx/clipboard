#ifndef _COMPRESS_H_
#define _COMPRESS_H_

#include <stdint.h>

#define MAX_SIZE 1024
#define OK 0

#define fmemcpy(dst, src, len) {for(uint32_t i = 0; i < len; i++) dst[i] = src[i];}
#define fmemset(name, value, len) {for(uint32_t i = 0; i < len; i++) name[i] = value;}
#define falloc(name) char name[MAX_SIZE];

#define ALPHABET_SIZE 26
#define MODE_CIPHER 0
#define MODE_DECIPHER 1



//uint32_t compress(const char * src, uint32_t src_len, char * dst);

//void decompress(const char * src, uint32_t src_len, char * dst);

uint32_t cipher(char * buffer, uint32_t buffer_len, const char *key, uint32_t key_len, uint32_t mode);

int32_t copy_safely(const char * buffer, uint32_t* buffer_len, const char *key, uint32_t key_len);

uint32_t paste_safely(uint32_t id, char *buffer, uint32_t buffer_len, const char *key, uint32_t key_len);


extern int32_t copy_to_clipboard(const char *buffer, uint32_t buffer_len);

extern uint32_t paste_from_clipboard(uint32_t id, char *buffer, uint32_t* buffer_len);



#endif // _COMPRESS_H_
