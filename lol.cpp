#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
	int giga = 3;

	for (int i = 0; i < giga; ++i) {
		// alocate 1 MB 1024 times
		for (int j = 0; j < 1024; ++j) {
			memset(malloc(1 << 20), 0xf3f3, 1 << 20);
			usleep(100);
		}
	}

	return 0;
}