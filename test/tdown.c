#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void print_string(const char* c, uint32_t len) {
    for(int i = 0; i < len; i++)
        printf("%c", c[i]);

    printf(" |LEN=%d| ", len);
}


int main() {
	char buff[1025] = "abzx";
	
	system("service down clipboard");
	sleep(2);
	
	system("service up /service/clipboard -dev /dev/clipboard");
	sleep(2);
	
	int id = copy_to_clipboard("ABZX", 4);

	system("service update /service/clipboard");
	
	sleep(10);
	
	printf("%d", getpid());
	paste_from_clipboard(id, buff, 1025);
}
