char *data;
#include "function.h"

//char data[256][ROWS]={0};
int main() {
	data = malloc(ROWS*256);
	//debug();
    setup();
	if (data == NULL) {
		quit();
		printf("error calling malloc with size of %d\n", ROWS*256);
		exit(-1);
	}
    for (int ascii=0;ascii<255;ascii++) {
		if (ui(ascii) != 0) {
			;
		} else {
			goto label_finished;
		}
	}
	label_finished:
	quit();
	return 0;
}