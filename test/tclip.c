#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void print_string(const char* c, uint32_t len) {
    for(int i = 0; i < len; i++)
        printf("%c", c[i]);

    printf(" |LEN=%d| ", len);
}


int main() {
	char buff[1025] = "abzx";
	
	system("service up /service/clipboard -dev /dev/clipboard");
	sleep(2);
	
	int id = copy_to_clipboard("ABZX", 4);
	int ret = paste_from_clipboard(id, buff, 3);
	assert(ret < 0);
	
	ret = paste_from_clipboard(id, buff, 4);
	assert(ret == 0);
	
	ret = copy_to_clipboard(buff, 1025);
	assert(ret < 0);
	
	print_string(buff, 4);
	
	system("service down clipboard");
}
