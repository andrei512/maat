#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DBG

#ifdef DBG 
	#define dprintf(fmt_args...) printf(fmt_args);fflush(stdout);
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

// ====================================================================================================



typedef struct NodeInfo {
	int index;
	NodeInfo *next;
} NodeInfo;

// O(1)
NodeInfo *newNodeInfo(int index) {
	NodeInfo *info = (NodeInfo *)malloc(sizeof(NodeInfo));

	info->index = index;
	info->next = NULL;

	return info;
}

#include "char_map.h"
typedef struct TrieNode {
	char character;
	char n_sons;
	TrieNode *sons;
	NodeInfo *info;
} TrieNode;

TrieNode *root = NULL;

// O(1)
TrieNode *newNode(char character) {
	TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));

	node->character = character;
	node->n_sons = 0;
	node->sons = NULL;
	node->info = NULL;	

	return node;
}

// O(1)
inline void add_index_to_node_info(TrieNode *node, int index) {	
	// if no info create the first item form the linked list
	if (node->info == NULL) {
		node->info = newNodeInfo(index);
	} else {
		// create a new list tha will point to the begining of the previous list
		NodeInfo *info = newNodeInfo(index);		
		info->next = node->info;
		node->info = info;
	}
}

inline int add_edge_to_character(TrieNode *node, char character) {
	// get the old list
	TrieNode *old_sons = node->sons;
	char new_n_sons = node->n_sons + 1;
	int son_index = node->n_sons;

	// create a new one and copy the old information and add the new node
	TrieNode *new_sons = (TrieNode *)malloc(new_n_sons * sizeof(TrieNode));
	memcpy(new_sons, old_sons, node->n_sons * sizeof(TrieNode));
	memcpy(new_sons+son_index, newNode(character), sizeof(TrieNode));

	// apply changes on current node
	node->n_sons = new_n_sons;
	node->sons = new_sons;		

	return son_index;	
}

inline int son_index_for_character(TrieNode *node, char character) {
	for (int i = 0; i < node->n_sons; ++i) {
		TrieNode son = node->sons[i];
		if (son.character == character) {
			return i;
		}
	}
	return -1;
}

inline int get_or_add_son_for_character(TrieNode *node, char character) {
	int son_index = son_index_for_character(node, character);

	if (son_index == -1) {
		son_index = add_edge_to_character(node, character);
	}

	return son_index;
}

// O(|string| * SIGMAish) but more like O(|string|) when taking in consideration all of the insertions
void add_to_trie(TrieNode *node, char *string, int index) {
	if (strlen(string) > 0) {	
		char first_character = string[0];
		int son_index = get_or_add_son_for_character(node, first_character);
		
		add_to_trie(&(node->sons[son_index]), string+1, index);
	} else {
		add_index_to_node_info(node, index);
	}
}

// O(|string| ^ MAX_VARIATION)
const int MAX_VARIATION = 3;
void add_to_trie_with_deletition_variants(TrieNode *node, char *string, int index, int variation) {
	if (strlen(string) > 0) {	
		if (variation < MAX_VARIATION) {
			int deletiton_index = get_or_add_son_for_character(node, DELETITION_MARKER);
			add_to_trie_with_deletition_variants(&(node->sons[deletiton_index]), string+1, index, variation + 1);	
		}

		char first_character = string[0];
		int son_index = get_or_add_son_for_character(node, first_character);
		
		add_to_trie_with_deletition_variants(&(node->sons[son_index]), string+1, index, variation);
	} else {
		add_index_to_node_info(node, index);
	}
}

bool valid_string(char *string) {
	if (strlen(string) <= 0) {
		return false;
	}

	int length = strlen(string);
	for (int i = 0; i < length; ++i) {		
		if (char_map[(int)string[i]] == -1) {
			return false;
		}
	}
	return true;
}

// O(N * |string|)
void build_trie() {
	root = newNode(0); // beacuse money is the root of all evil

	for (int i = 0; i < N; ++i) {	
		if (valid_string(locations[i].name) == false) {
			dprintf("id = %s - |%s| is invalid!\n", locations[i].id, locations[i].name);
			continue;
		}
		add_to_trie_with_deletition_variants(root, locations[i].name, i, 0);
	}
}

// ====================================================================================================

#include "char_dist.h"

int matrix[128][128];
int distance(char *s1, char *s2) {
    int x, y, s1len, s2len;
    
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

int N_RET;
char *names[128];
FILE *fifo_pipe;

bool print_info(NodeInfo *info) {
	if (N_RET < 20) {
		++N_RET;

		fprintf(fifo_pipe, "%s***", locations[info->index].name);

		return true;
	}
	return false;
}

void iterate_trie(TrieNode *node, char *prefix) {
	// dprintf("iterate_trie - node = %u prefix = %s\n", node, prefix);
	if (prefix != NULL and strlen(prefix) > 0) {
		if (node->sons != NULL) {		
			char first_character = prefix[0];

			int son_index = -1;
			for (int i = 0; i < node->n_sons; ++i) {
				TrieNode son = node->sons[i];
				if (son.character == first_character) {
					son_index = i;
					break;
				}
			}

			if (son_index != -1) {
				char *next_prefix = strlen(prefix) > 1 ? prefix + 1 : NULL;
				iterate_trie(&(node->sons[son_index]), next_prefix);
			}
		}
	} else {
		if (node->info != NULL) {
			NodeInfo *it = node->info;
			while (it) {
				if (print_info(it) == false) {
					return ; 
				}
				it = it->next;
			}
		}

		if (node->sons != NULL) {
			for (int i = 0; i < node->n_sons; ++i) {
				iterate_trie(&(node->sons[i]), NULL);	
			}
		}
	}
}

void solve_for(char *string) {	
	double start = clock();

	fifo_pipe = fopen("fifo_pipe", "w");

	N_RET = 0;
	iterate_trie(root, string);

	fclose(fifo_pipe);

	double finish = clock();



	double dt = (finish - start) / CLOCKS_PER_SEC;
	printf("for %s - %.4f seconds\n", string, dt);		

	fflush(stdout);
}

inline void query_loop() {
	char string[100];
	while (true) {
		// while (fgets(string, 100, stdin) == NULL);
		fgets(string, 100, stdin);

		// remove \n
		string[strlen(string) - 1] = 0;
		
		if (strlen(string) > 0) {
			solve_for(string);		
		}
	}
}

void debug_trie(TrieNode *node, char *prefix) {
	dprintf("%s -> %lu\n", prefix, (unsigned long)node);

	for (int i = 0; i < node->n_sons; ++i) {
		char *new_prefix = (char *)malloc(strlen(prefix) + 2);

		TrieNode son = node->sons[i];

		sprintf(new_prefix, "%s%c", prefix, son.character);

		debug_trie(&son, new_prefix);
	}
}

int main(int argc, char **args) {
	if (argc < 2) {
		printf("Please specify an input file\n");
		return 1; 
	}

	load_input(args[1]);
	build_trie();

	// debug_trie(root, "");

	query_loop();

	return 0;
}
