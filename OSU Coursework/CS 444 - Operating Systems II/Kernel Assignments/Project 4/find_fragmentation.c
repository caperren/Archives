#include <stdio.h>
#include <linux/unistd.h>

#define __sys_slob_free 400
#define __sys_slob_used 401

int main (void){
	float fragmentation_percent;
	long used = syscall(__sys_slob_used);
	long free = syscall(__sys_slob_free);
        
	fragmentation_percent = (float) free / (float) used;
	printf("Amount free: %ld.\n", free);
	printf("Amount used: %ld.\n", used);
	printf("Amount frag: %.2f%%.\n", fragmentation_percent);
        
	return 0;
}
