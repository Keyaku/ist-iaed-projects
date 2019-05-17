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

bool hashtable_add_node(Node *n) {
	HashNode *hn;
	int idx = hash(n->name);

	if (AllNodes[idx] == NULL) {
		AllNodes[idx] = calloc(1, sizeof(HashNode));
		hn = AllNodes[idx];
	} else {
		for (hn = AllNodes[idx]; hn->next != NULL; hn = hn->next) {
			if (strcmp(hn->n->name, n->name) == 0) return false;
		}
		if (strcmp(hn->n->name, n->name) == 0) return false;
		hn->next = calloc(1, sizeof(HashNode));
		hn = hn->next;
	}

	hn->n = n;
	return true;
}

void hashtable_remove_node(Node *n) {
	HashNode *hn, *next;
	int idx = hash(n->name);

	for (hn = AllNodes[idx], next = hn; next->n != n; hn = next, next = next->next);
	if (hn == next) AllNodes[idx] = hn->next;
	hn->next = next->next;

	free(next);
}

void hashtable_destroy() {
	int idx;
	for (idx = 0; idx < HASHTABLE_SIZE; idx++) {
		HashNode *hn = AllNodes[idx];
		while (hn != NULL) {
			HashNode *tmp = hn->next;
			free(hn);
			hn = tmp;
		}
	}
}
