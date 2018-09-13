#include <stdint.h>
#include "compress.h"

extern void print_string(const char*, uint32_t );


typedef uint32_t flag_t;

#define NUMBER_MIN '0'
#define NUMBER_MAX '9'
#define LOWER_MIN 'a'
#define LOWER_MAX 'z'
#define UPPER_MIN 'A'
#define UPPER_MAX 'Z'

inline uint32_t isdigit(char x) {
    return (NUMBER_MIN <= (x) && (x) <= NUMBER_MAX);
}

inline uint32_t islower(char x) {
    return (LOWER_MIN <= (x) && (x) <= LOWER_MAX);
}

inline uint32_t isupper(char x) {
    return (UPPER_MIN <= (x) && (x) <= UPPER_MAX);
}

void push_char(char * dst, uint32_t * pos, char c) {
    dst[(*pos)++] = c;
}

uint32_t itoa_inverse(char * buffer, uint32_t val) {
    uint32_t i = 0;
    while(val > 0) {
        buffer[i] = '0' + val%10;
        val /= 10;
        i++;
    }
    return i;
}

uint32_t push_int(char * buffer, uint32_t* pos, uint32_t val) {
    char tmp[16];
    uint32_t res = itoa_inverse(tmp, val);
    int32_t i;

    for(i = res - 1; i >= 0; i--) {
        push_char(buffer, pos, tmp[i]);
    }
    return res;
}

void push_char_count(char * dst, uint32_t* pos, char c, uint32_t count) {
    if(count == 0) return;
    push_char(dst, pos, c);
    if(count == 1) return;
    pos += push_int(dst, pos, count);
}

/** returns length of dst **/
uint32_t compress(const char * src, uint32_t src_len, char * dst) {
    char last_char = 0;
    uint32_t count = 0;
    uint32_t pos = 0;
    char c;

    for (uint32_t i = 0; i < src_len; i++) {
        c = src[i];
        if (c == last_char) { count++; }
        else {
            push_char_count(dst, &pos, last_char, count);

            last_char = c;
            count = 1;
        }
    }
    push_char_count(dst, &pos, last_char, count);
    return pos;
 }


#define PARSING 0
#define NOT_PARSING 1


 void push_char_multiple(char * buffer, uint32_t* pos, char c, char * count) {
    while(*count > 0) {
        push_char(buffer, pos, c);
        (*count)--;
    }
}

 void push_decompressed(char * dst, uint32_t* pos, char last_char, uint32_t * count, uint32_t* parsing_state) {
    while(*count > 1) {
        push_char(dst, pos, last_char); (*count)--;
    }
    *count = 0;
    *parsing_state = NOT_PARSING;
}

 void decompress(const char * src, uint32_t src_len, char * dst) {
    uint32_t count = 0;
    uint32_t pos = 0;
    char c;
    char last_char = 0;
    flag_t parsing_state = NOT_PARSING;

    for(uint32_t i = 0; i < src_len; i++) {
        c = src[i];
        if(!isdigit(c)) {
            if(pos > 0) {
                push_decompressed(dst, &pos, last_char, &count, &parsing_state);
            }
            last_char = c;
            push_char(dst, &pos, c);
        }
        else {
            if(parsing_state == NOT_PARSING) {
                parsing_state = PARSING;
                count = 0;
            }
            else {
                count = 10 * count;
            }
            count = (count + c - '0')%ALPHABET_SIZE;
        }
    }
    push_decompressed(dst, &pos, last_char, &count, &parsing_state);
}

 void finish_push_number(char * res, uint32_t * pos, char value, uint32_t* parsing_state) {
    if(*parsing_state == PARSING) {
        push_char(res, pos,  value);
        *parsing_state = NOT_PARSING;
    };
}

 void normalize_key(char* key, uint32_t* key_len) {
    char * res = key;
    uint32_t count = 0;
    uint32_t i, j = 0;
    flag_t parsing_state = NOT_PARSING;
    char c;

    for(i = 0; i < *key_len; i++) {
        c = key[i];
        if(isdigit(c)) {
            if(parsing_state == PARSING) { count = 10 * count; }
            else {
                count = 0;
                parsing_state = PARSING;
            }
            count = (count +  c - '0') % ALPHABET_SIZE;
        }
        else {
            finish_push_number(res, &j, count, &parsing_state);

            if(islower(c)) {push_char(res, &j,  c - 'a'); }
            else { push_char(res, &j, c - 'A');}
        }
    }

    finish_push_number(res, &j, count, &parsing_state);
    *key_len = j;
}

 char char_base(char c) {
    if(isdigit(c)) return NUMBER_MIN;
    if(islower(c)) return LOWER_MIN;
    if(isupper(c)) return UPPER_MIN;
    return 0;
}



// expects normalized key
 uint32_t cipher(char * buffer, uint32_t buffer_len, const char *key, uint32_t key_len, uint32_t mode) {
    if(key_len > MAX_SIZE) { return -1; }

    falloc(norm_key);
    uint32_t norm_key_len = key_len;
    uint32_t k = 0, i;
    char base;

    fmemcpy(norm_key, key, key_len);
    normalize_key(norm_key, &norm_key_len);

    //print_string(buffer, buffer_len);

    for(i = 0; i < buffer_len; i++) {
        base = char_base(buffer[i]);
        buffer[i] -= base;

        if(mode == MODE_DECIPHER) { buffer[i] += ALPHABET_SIZE - norm_key[k]; }
        else { buffer[i] += norm_key[k]; }

        buffer[i] %= ALPHABET_SIZE;
        buffer[i] += base;
        k = (k+1)%norm_key_len;
    }

    //print_string(buffer, buffer_len);

    return OK;
}


extern int32_t copy_to_clipboard(const char *buffer, uint32_t buffer_len);

extern uint32_t paste_from_clipboard(uint32_t id, char *buffer, uint32_t* buffer_len);




int32_t copy_safely(const char * buffer, uint32_t* buffer_len, const char *key, uint32_t key_len) {
    int32_t compressed_size, res;
	falloc(tmp);
	
    if(*buffer_len > MAX_SIZE) return -1;
    fmemcpy(tmp, buffer, *buffer_len);

    cipher(tmp, *buffer_len, key, key_len, MODE_CIPHER);
    compressed_size = compress(tmp, *buffer_len, tmp);
    //print_string(tmp, *buffer_len);
    res = copy_to_clipboard(tmp, compressed_size);

    *buffer_len = compressed_size;
    return res;
}


#define slen(i, s, bound) uint32_t i = 0; while(i < bound && s[i] != 0) {i++;}
uint32_t paste_safely(uint32_t id, char *buffer, uint32_t buffer_len, const char *key, uint32_t key_len) {
    int32_t res;
    uint32_t buffer_len_tmp = buffer_len;
    falloc(tmp);

    res = paste_from_clipboard(id, tmp, &buffer_len_tmp);
    if(res != 0) return res;

    slen(compressed_len, tmp, buffer_len);
    //print_string(tmp, compressed_len);

    fmemset(buffer, 0, buffer_len);
    decompress(tmp, compressed_len, buffer);
    slen(decompressed_len, buffer, buffer_len);

    cipher(buffer, decompressed_len, key, key_len, MODE_DECIPHER);

    //print_string(buffer, decompressed_len);

    return res;
}
