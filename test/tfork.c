#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

#define N 20

void print(int start) {
	printf("N=%d, PID_START=%d\n", N, start);
	for(int i = 0; i < N; i++) {
		printf("PID %-4d :", start + i); 
		for(int j = 0; j < N; j++) {
			if((start + j)%10 == 0) printf("|");
			printf("%d", check_if_ancestor(start+j, start+i));
		}
		printf("\n");
	}
	
}


int main() {
	pid_t pid = getpid();
	pid_t parent_pid = getpid();
	printf("pid=%d\n", pid);
	
	for(int i = 0; i < 4; i++) {
		switch (pid = fork()) {
			case -1:
				break;
			case 0:
				fprintf(stderr, "%d", getpid()); 
				sleep(3);
				break;
			
			default:
				if(i > 2 && getpid() == parent_pid) print(parent_pid);
				sleep(3);
				break;
		}
	}
}
