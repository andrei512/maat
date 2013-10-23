#include <stdio.h>

#define DBG_

#ifdef DBG 
	#define dprintf(fmt, args...) printf(fmt, args);
#else
	#define dprintf(fmt, args...) /* nothing */
#endif

char **read_csv_line(char *line) {
	char *lp = line;

	char buff[255], bp;

	while (*lp) {
		dprintf("%c\n", *lp);

		++lp;
	}
}

void load_input(char *input_file) {
	FILE *input = fopen(input_file, "r");

	char line[255];

	// Get columns	
	fgets(line, sizeof(line), input);
	
	read_csv_line(line);

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