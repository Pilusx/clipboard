

#define MODE_CIPHER 0
#define MODE_DECIPHER 1
#define MODE_COMPRESS 2
#define MODE_DECOMPRESS 3

#define MAX(x, y) ((x) < (y) ? (y) : (x))

#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "copy_paste_clipboard.h"

void include_char(uint32_t * len, uint32_t* counter, uint32_t pos) {
    if(*counter == 0 && pos > 0) {
        (*len)++;
    }
    else {
        *len += *counter;
        *counter = 0;
    }
}

extern void print_string(const char* c, uint32_t len);

uint32_t expected_decompressed_size(const char * buffer, uint32_t buffer_len) {
    uint32_t len = 0;
    uint32_t current = 0;
    uint32_t pos;
    char c;

    for(pos = 0 ; pos < buffer_len; pos++) {
        c = buffer[pos];
        if(!isdigit(c)) {
            include_char(&len, &current, pos);
        }
        else {
            current = 10 * current + c - '0';
        }
    }

    include_char(&len, &current, pos);
    return len;
}



uint32_t cipher(char * buffer, uint32_t buffer_len, const char *key, uint32_t key_len, uint32_t mode);

uint32_t copy_safely(const char *buffer, uint32_t *buffer_len,
                     const char *key, uint32_t key_len);
int32_t paste_safely(uint32_t id, char *buffer, uint32_t buffer_len,
                     const char *key, uint32_t key_len);



void assert_equal_str(const char * t1, const char * t2, uint32_t len) {
    printf("CMP? :");
    print_string(t1, len);
    print_string(t2, len);
    printf("\n");

    for(int i = 0; i < len; i++)
        assert(t1[i] == t2[i]);
}

void test_cipher(char * test, uint32_t test_len,
                 char * key, uint32_t key_len,
                 char * res, uint32_t res_len,
                 uint32_t mode) {

#define copy(s) char * copy_## s = malloc(sizeof(char) * s ##_len); memcpy(copy_## s, s, s ##_len);
#define clean(s) char * copy_## s = malloc(sizeof(char) * s ##_len); memset(copy_## s, 0, s ## _len);

    copy(test);
    cipher(copy_test, test_len, key, key_len, mode);
    assert_equal_str(copy_test, res, res_len);

    free(copy_test);
}


#ifdef TEST_COMPRESS
void test_compress(char * test, uint32_t test_len,
                   char * res, uint32_t res_len,
                   uint32_t mode) {


    clean(res);
    if(mode == MODE_DECOMPRESS) {
        assert(expected_decompressed_size(test, test_len) == res_len);
        decompress(test, test_len, copy_res);
    } else {
        assert(compress(test,test_len, copy_res) == res_len);
    }

    assert_equal_str(copy_res, res, res_len);
    free(copy_res);
}
#define TEST_COMPRESS_DUAL(from, fl, to, tl) \
    test_compress(from, fl, to, tl, MODE_COMPRESS); \
    test_compress(to, tl, from, fl, MODE_DECOMPRESS);
    
#endif //TEST_COMPRESS

#define TEST_CIPHER_DUAL(from, fl, key, kl, to, tl) \
    test_cipher(from, fl, key, kl, to, tl, MODE_CIPHER); \
    test_cipher(to, tl, key, kl, from, fl, MODE_DECIPHER);




void cipher_compress_test() {
    //char buff[16];
    //uint32_t len = itoa_inverse(buff, 131412);
    //push_char(buff, &len, 'a');
    //print_string(buff, len);

    TEST_CIPHER_DUAL("abCzX", 5, "de35", 4, "dfLcB", 5);
    TEST_CIPHER_DUAL("zzmoZ", 5, "12ab", 4, "lznaZ", 5);
    //TEST_COMPRESS_DUAL("aabbZZZc", 8,  	"a2b2Z3c", 7);
    //TEST_COMPRESS_DUAL("zzZmwww", 7,  	"z2Zmw3", 6);
    
    

    printf("COMPRESSING & CIPHERING OK!\n");
}


#define ASSERT(x, exp) if (x != exp) printf("%d != %d \n", x, exp), assert(x == exp)
void clipboard_test() {
    char buff[20] = "abzX", text[20];
    uint32_t buff_size = 4;

    clipboard_init();

    uint32_t text_id = copy_safely(buff, &buff_size, "c3", 2);
    assert(text_id != -1);

    printf("TEXT_ID: %d\n", text_id);
    int32_t res;

    clipboard_log();

    res = paste_safely(text_id, text, 1, "c3", 2);   // Wynikiem jest -1 (za mały bufor).
    ASSERT(res, -1);

    clipboard_log();

    res = paste_safely(text_id, text, 6, "c3", 2);   // Wynikiem jest 0, text zawiera "cebA\0\0".
    ASSERT(res, 0);
    assert_equal_str(buff, text, 4);

    clipboard_log();

    res = paste_safely(text_id, text, 4, "c3", 2);   // Wynikiem jest -1 (wpis już został usunięty).
    ASSERT(res, -1);

    clipboard_log();
    printf("RETURN VALUES OK!\n");
}


int main() {
    cipher_compress_test();
    clipboard_test();
}

