#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

char buffer[128];


void load_input(char *input_file) {
	FILE *input = fopen(input_file, "r");

	fscanf(input, "%d\n", &N);

	locations = (Location *)malloc(N * sizeof(Location));

	for (int i = 0; i < N; ++i) {
		
		// ID
		fgets(buffer, 128, input);
		buffer[strlen(buffer) - 1] = 0;
		locations[i].id = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
		memcpy(locations[i].id, buffer, strlen(buffer));

		// NAME
		fgets(buffer, 128, input);
		buffer[strlen(buffer) - 1] = 0;
		locations[i].name = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
		memcpy(locations[i].name, buffer, strlen(buffer));

		// PRECISE NAME
		fgets(buffer, 128, input);
		buffer[strlen(buffer) - 1] = 0;
		locations[i].precise_name = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
		memcpy(locations[i].precise_name, buffer, strlen(buffer));

		// LAYER
		fgets(buffer, 128, input);
		buffer[strlen(buffer) - 1] = 0;
		locations[i].layer = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
		memcpy(locations[i].layer, buffer, strlen(buffer));

		// STATE
		fgets(buffer, 128, input);
		buffer[strlen(buffer) - 1] = 0;
		locations[i].state = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
		memcpy(locations[i].state, buffer, strlen(buffer));

		// COUNTRY
		fgets(buffer, 128, input);
		buffer[strlen(buffer) - 1] = 0;
		locations[i].country = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
		memcpy(locations[i].country, buffer, strlen(buffer));
	}

	fclose(input);

	// for (int i = 0; i < N; ++i) {
	// 	dprintf("%s %s %s %s %s %s\n", 
	// 		locations[i].id, 		
	// 		locations[i].name, 		
	// 		locations[i].precise_name, 		
	// 		locations[i].layer, 		
	// 		locations[i].state, 			
	// 		locations[i].country
	// 	);
	// }
}

#include "char_dist.h"

unsigned int matrix[128][128];
int distance(char *s1, char *s2) {
    unsigned int x, y, s1len, s2len;
    
    s1len = strlen(s1);
    s2len = strlen(s2);
    
    
    matrix[0][0] = 0;

    for (x = 1; x <= s2len; x++) {
        matrix[x][0] = matrix[x-1][0] + 1;    	
    }

    for (y = 1; y <= s1len; y++) {
        matrix[0][y] = matrix[0][y-1] + 1;    	
    }

    for (x = 1; x <= s2len; ++x) {
        for (y = 1; y <= s1len; ++y) {
            matrix[x][y] = matrix[x-1][y] + 10; // ignore a character from the first string

            int tmp = matrix[x][y-1] + 10; // ignore a character from the second string
            if (tmp < matrix[x][y]) {
            	matrix[x][y] = tmp;
            }

            tmp = matrix[x-1][y-1] + cost_for(s1[y], s2[x]); // add the cost of matching based on keyboard

            if (tmp < matrix[x][y]) {
            	matrix[x][y] = tmp;
            }
        }
    }
 
    return matrix[s2len][s1len];
}

void add_to_max_heap(int index, int dist, int *heap, int *heap_p, int size, int &count) {
	++count;

	heap[count] = index;
	heap_p[count] = dist;

	int idx = count;

	while (idx > 1 and heap_p[idx>>1] < heap_p[idx]) {
		register int tmp = heap[idx];
		heap[idx] = heap[idx>>1];
		heap[idx>>1] = tmp;

		tmp = heap_p[idx];
		heap_p[idx] = heap_p[idx>>1];
		heap_p[idx>>1] = tmp;

		idx >>= 1;
	}	
}

void pop_max_heap(int *heap, int *heap_p, int size, int &count) {
	if (count > 1) {
		// add the last element on top of the heap and remove the max element
		// TO DO: swap last element with first in order to obtain heapsort 
		heap[1] = heap[count];
		heap_p[1] = heap_p[count];
		--count;

		// go down until balanced
		int idx = 1;
		while (true) {
			int max_p_idx = idx;
			if (idx * 2 <= count and
				heap_p[idx * 2] > heap_p[max_p_idx]) {
				max_p_idx = idx * 2;
			}

			if (idx * 2 + 1 <= count and
				heap_p[idx * 2 + 1] > heap_p[max_p_idx]) {
				max_p_idx = idx * 2 + 1;
			}

			if (max_p_idx == idx) {
				break ;
			} else {
				register int tmp = heap[idx];
				heap[idx] = heap[max_p_idx];
				heap[max_p_idx] = tmp;

				tmp = heap_p[idx];
				heap_p[idx] = heap_p[max_p_idx];
				heap_p[max_p_idx] = tmp;

				idx = max_p_idx;
			}
		}
	} else {
		count = 0;
	}
}

char *string_from_location(char *buff, Location location) {	
	sprintf(buff, "%s %s %s %s %s %s", 
		location.id, 		
		location.name, 		
		location.precise_name, 		
		location.layer, 		
		location.state, 			
		location.country
	);
	return buff;
}

void solve_for(char *string) {	
	double start = clock();

	int heap_size = 40, count = 0;
	int heap[2 * heap_size + 1];	
	int heap_p[2 * heap_size + 1];

	for (int i = 1; i < N; ++i) {
		int dist = distance(string, locations[i].name);
		
		add_to_max_heap(i, dist, heap, heap_p, heap_size, count);
		if (i > heap_size) {
			pop_max_heap(heap, heap_p, heap_size, count);
		}
	}

	double finish = clock();

	// TO DO: sort the heap

	for (int i = 1; i < count; ++i) {
		printf("%d - %d - %s\n", heap_p[i], heap[i], string_from_location(buffer, locations[heap[i]]));		
	}

	fflush(stdout);

	double dt = (finish - start) / CLOCKS_PER_SEC;

	printf("%.4f seconds\n", dt);		
}

inline void query_loop() {
	char string[100];
	while (true) {
		fgets(string, 100, stdin);
		// remove \n
		string[strlen(string) - 1] = 0;
		
		solve_for(string);		
	}
}

int main(int argc, char **args) {
	if (argc < 2) {
		printf("Please specify an input file\n");
		return 1; 
	}

	load_input(args[1]);

	query_loop();

	return 0;
}