#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DBG

#ifdef DBG 
	#define dprintf(fmt_args...) printf(fmt_args);
#else
	#define dprintf(fmt_args...) /* nothing */
#endif


typedef struct {
	char *id, *name, *precise_name, *layer, *state, *country;
} Location;



int N;
Location *locations;

void load_input(char *input_file) {
	FILE *input = fopen(input_file, "r");

	fscanf(input, "%d\n", &N);

	locations = (Location *)malloc(N * sizeof(Location));

	char buffer[128];

	for (int i = 0; i < N; ++i) {
		
		// ID
		fgets(buffer, 255, input);
		buffer[strlen(buffer) - 1] = 0;
		locations[i].id = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
		memcpy(locations[i].id, buffer, strlen(buffer));

		// NAME
		fgets(buffer, 255, input);
		buffer[strlen(buffer) - 1] = 0;
		locations[i].name = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
		memcpy(locations[i].name, buffer, strlen(buffer));

		// PRECISE NAME
		fgets(buffer, 255, input);
		buffer[strlen(buffer) - 1] = 0;
		locations[i].precise_name = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
		memcpy(locations[i].precise_name, buffer, strlen(buffer));

		// LAYER
		fgets(buffer, 255, input);
		buffer[strlen(buffer) - 1] = 0;
		locations[i].layer = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
		memcpy(locations[i].layer, buffer, strlen(buffer));

		// STATE
		fgets(buffer, 255, input);
		buffer[strlen(buffer) - 1] = 0;
		locations[i].state = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
		memcpy(locations[i].state, buffer, strlen(buffer));

		// COUNTRY
		fgets(buffer, 255, input);
		buffer[strlen(buffer) - 1] = 0;
		locations[i].country = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
		memcpy(locations[i].country, buffer, strlen(buffer));
	}

	for (int i = 0; i < N; ++i) {
		printf("%s %s %s %s %s %s\n", 
			locations[i].id, 		
			locations[i].name, 		
			locations[i].precise_name, 		
			locations[i].layer, 		
			locations[i].state, 			
			locations[i].country
		);
	}

	fclose(input);
}

inline void query_loop() {
	while (true) {
		char string[100];
		scanf("%s", string);
		printf("%s\n", string);
		fflush(stdout);
	}
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