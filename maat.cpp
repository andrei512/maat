#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <vector>
#include <algorithm>

using namespace std;

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
	int index;
} TrieNode;

TrieNode *root = NULL;

typedef struct ActiveState {
	TrieNode *node;
	char cursor;
	char incoordonation;
} ActiveState;


#ifdef DBG
int N_NODES = 0;
#endif

ActiveState *new_active_state(TrieNode *node, char cursor, char incoordonation) {
	ActiveState *active_state = (ActiveState *)malloc(sizeof(ActiveState));

	active_state->node = node;
	active_state->cursor = cursor;
	active_state->incoordonation = incoordonation;

	return active_state;
}

// O(1)
TrieNode *newNode(char character) {	
	TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));

	node->character = character;
	node->n_sons = 0;
	node->sons = NULL;
	node->info = NULL;	

	#ifdef DBG
	++N_NODES;
	#endif


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

// O(n_sons)
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

// O(n_sons)
inline int son_index_for_character(TrieNode *node, char character) {
	for (int i = 0; i < node->n_sons; ++i) {
		TrieNode son = node->sons[i];
		if (son.character == character) {
			return i;
		}
	}
	return -1;
}

// O(n_sons)
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
const int MAX_VARIATION = 2;
void add_to_trie_with_deletition_variants(TrieNode *node, char *string, int index, int variation) {
	node->index = index;
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
	#ifdef DBG
	N_NODES = 0;
	#endif

	root = newNode(-1); 

	for (int i = 0; i < N; ++i) {	
		if (valid_string(locations[i].name) == false) {
			dprintf("id = %s - |%s| is invalid!\n", locations[i].id, locations[i].name);
			continue;
		}
		add_to_trie_with_deletition_variants(root, locations[i].name, i, 0);
	}

	#ifdef DBG
	dprintf("the trie has %d nodes\n", N_NODES);
	#endif
}

// ====================================================================================================
// IncNGTrie - algorithms

// ActiveState vector and iterator
typedef vector<ActiveState> va;
typedef vector<ActiveState>::iterator vait;

bool compare_state(const ActiveState &a, const ActiveState &b) {
	if (a.incoordonation == b.incoordonation) {
		return a.cursor > b.cursor;
	}
    return a.incoordonation < b.incoordonation;
}

// int vector and iterator
typedef vector<int> vi;
typedef vector<int>::iterator viit;

char *string_from_state(ActiveState state) {
	sprintf(buffer, "<%p - \"%s\", %d, %d>", state.node, locations[state.node->index].name, (int)state.cursor, (int)state.incoordonation);
	return (char *)memcpy(malloc(strlen(buffer) + 1), buffer, strlen(buffer) + 1);
}

va expand_state(ActiveState active_state) {
	va A = va();

	// create expansion with deletition
	for (char dq = 0; dq <= MAX_VARIATION - active_state.incoordonation; ++dq) {
		A.push_back(*new_active_state(active_state.node, 
									  active_state.cursor + dq, 
									  active_state.incoordonation + dq));
	}

	TrieNode *node = active_state.node;

	char ds = 1;
	int son_index = -1;
	while (active_state.incoordonation + ds <= MAX_VARIATION and 
		   (son_index = son_index_for_character(node, DELETITION_MARKER)) != -1) {
		for (int dq = 0; dq <= MAX_VARIATION - active_state.incoordonation; ++dq) {
			int new_incoordonation = dq > ds ?: ds;
			A.push_back(*new_active_state(&(node->sons[son_index]), 
										  active_state.cursor + dq, 
										  active_state.incoordonation + new_incoordonation));
		}
		node = &(node->sons[son_index]);
		++ds;
	}
	
	return A;	
}

vi indexes_from_node(TrieNode *node) {
	if (node->n_sons == 0) {
		vi ret = vi();
		
		for (NodeInfo *it = node->info; it != NULL; it=it->next) {
			ret.push_back(it->index);
		}

		sort(ret.begin(), ret.end());
		ret.erase(unique(ret.begin(), ret.end()), ret.end());
		
		return ret;
	} else {
		vi ret = vi();

		for (int i = 0; i < node->n_sons; ++i) {
			vi indexes_from_son = indexes_from_node(&(node->sons[i]));
			ret.insert(ret.end(), indexes_from_son.begin(), indexes_from_son.end());
		}

		sort(ret.begin(), ret.end());
		ret.erase(unique(ret.begin(), ret.end()), ret.end());

		return ret;
	}
}



vi inclemental_search(char *query) {
	int query_length = strlen(query);

	if (query_length == 0) {
		return vi();
	}

	va A = expand_state(*new_active_state(root, 0, 0));

	for (int v = 0; v < query_length; ++v) {
		va A_ = va();
		for (vait it = A.begin(); it < A.end(); ++it) {		
			ActiveState state = *it;
			if (state.cursor > v) {
				A_.push_back(state);
			} 
			int son_index = son_index_for_character(state.node, query[v]);
			if (son_index != -1) {
				TrieNode *n_ = &(state.node->sons[son_index]);				
				va expanded_state = expand_state(*new_active_state(n_, state.cursor + 1, state.incoordonation));
				A_.insert(A_.end(), expanded_state.begin(), expanded_state.end());
			}
		}
		A = A_;
	}	

	// a bit of magik :)
	// sort(A.begin(), A.end(), compare_state);

	for (vait it = A.begin(); it != A.end(); ++it) {
		dprintf("%s\n", string_from_state(*it));
	}

	vi ret = vi();

	for (vait it = A.begin(); it != A.end(); ++it) {
		ActiveState state = *it;

		vi indexes_from_state = indexes_from_node(state.node);

		ret.insert(ret.end(), indexes_from_state.begin(), indexes_from_state.end());

		sort(ret.begin(), ret.end());
		ret.erase(unique(ret.begin(), ret.end()), ret.end());
	}

	return ret;
}

// ====================================================================================================


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

FILE *fifo_pipe;

void solve_for(char *string) {	
	double start = clock();

	fifo_pipe = fopen("fifo_pipe", "w");

	vi indexes = inclemental_search(string);
	for (viit it = indexes.begin(); it != indexes.end(); ++it) {
		fprintf(fifo_pipe, "%s***", locations[*it].name);
	}

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

void demo_query() {
	char query[128];
	sprintf(query, "mew yo");

	vi indexes = inclemental_search(query);
	for (viit it = indexes.begin(); it != indexes.end(); ++it) {
		dprintf("%s\n", locations[*it].name);
	}
}

int main(int argc, char **args) {

	if (argc < 2) {
		printf("Please specify an input file\n");
		return 1; 
	}

	load_input(args[1]);
	build_trie();

	// demo_query();

	query_loop();

	return 0;
}
