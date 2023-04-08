char *data;
#include "function.h"

FILE *memory;

int main() {
	data = malloc(ROWS*256);

	memory=fopen("font.mem", "rb");
	fgets(data, 4096, memory);
	fclose(memory);
	write_file();

	return 0;
}
