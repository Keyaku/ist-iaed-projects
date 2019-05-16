#include <stdlib.h>

#include "hash.h"

int hash(const char *str) {
	int val = 0, a = 127;
	while (*(str++) != 0)
		val = (val*a + *str) % HASHTABLE_SIZE;
	return val;
}

void hashtable_initialize() {
	int idx;
	for (idx = 0; idx < HASHTABLE_SIZE; idx++) {
		AllNodes[idx] = calloc(1, sizeof(HashNode));
	}
}

HashNode *hashtable_find_node(Node *n) {
	HashNode *hn;
	int idx = hash(n->name);
	for (hn = AllNodes[idx]; hn != NULL && hn->n != n; hn = hn->next);
	return NULL;
}

void hashtable_add_node(Node *n) {
	HashNode *hn = hashtable_find_node(n);
	/* TODO */
}

void hashtable_remove_node(Node *n) {
	HashNode *hn = hashtable_find_node(n);
	/* TODO */
}

void hashtable_destroy() {
	int idx;
	for (idx = 0; idx < HASHTABLE_SIZE; idx++) {
		free(AllNodes[idx]);
	}
}
