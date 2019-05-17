#include <stdlib.h>
#include <string.h>

#include "hash.h"

int hash(const char *str) {
	int val = 0, a = 127;
	while (*(str++) != 0)
		val = (val*a + *str) % HASHTABLE_SIZE;
	return val;
}

void hashtable_initialize() {
}

HashNode *hashtable_find_node(const char *name) {
	HashNode *hn;
	int idx = hash(name);
	for (hn = AllNodes[idx]; hn != NULL && strcmp(hn->n->name, name) != 0; hn = hn->next);
	return hn;
}

void hashtable_add_node(Node *n) {
	HashNode *hn;
	int idx = hash(n->name);

	if (AllNodes[idx] == NULL) {
		AllNodes[idx] = calloc(1, sizeof(HashNode));
		hn = AllNodes[idx];
	} else {
		for (hn = AllNodes[idx]; hn->next != NULL; hn = hn->next);
		hn->next = calloc(1, sizeof(HashNode));
		hn = hn->next;
	}

	hn->n = n;
}

Node *hashtable_remove_node(const char *name) {
	HashNode *hn, *next;
	Node *n;
	int idx = hash(name);

	for (hn = AllNodes[idx], next = hn; next != NULL && strcmp(next->n->name, name) != 0; hn = next, next = next->next);
	if (next == NULL) return NULL;
	if (hn == next) AllNodes[idx] = hn->next;
	hn->next = next->next;

	n = next->n;

	free(next);
	return n;
}

void hashtable_destroy() {
	int idx;
	for (idx = 0; idx < HASHTABLE_SIZE; idx++) {
		HashNode *hn = AllNodes[idx];
		while (hn != NULL) {
			HashNode *tmp = hn->next;
			free(hn); /* E depois o nó da HashTable */
			hn = tmp;
		}
	}
}
