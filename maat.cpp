#include <stdio.h>

#define DBG

#ifdef DBG 
	#define dprintf(fmt_args...) printf(fmt_args);
#else
	#define dprintf(fmt_args...) /* nothing */
#endif

void load_input(char *input_file) {
	FILE *input = fopen(input_file, "r");

	char line[255];

	// Get column names and ignore them
	
	// Get all the lines

	fclose(input);
}

inline void query_loop() {
	// while (true) {
	// 	char string[100];
	// 	scanf("%s", string);
	// 	printf("%s\n", string);
	// 	fflush(stdout);
	// }
}

int main(int argc, char **args) {
	if (argc < 2) {
		printf("Please specify an input file\n");
		return 1; 
	}

	load_input(args[1]);

	// query_loop();
	return 0;
}