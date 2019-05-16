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
	for (hn = AllNodes[idx]; hn != NULL && hn->n != NULL; hn = hn->next)
		if (strcmp(hn->n->name, name) == 0) break;
	return hn;
}

void hashtable_add_node(Node *n) {
	HashNode *hn = hashtable_find_node(n->name);
	if (hn == NULL) { /* Se o nó não estiver na HashTable, adicioná-lo */
		int idx = hash(n->name);
		for (hn = AllNodes[idx]; hn != NULL && hn->next != NULL; hn = hn->next);
		if (hn == NULL) {
			AllNodes[idx] = calloc(1, sizeof(HashNode));
			hn = AllNodes[idx];
		}
		else {
			hn->next = calloc(1, sizeof(HashNode));
			hn = hn->next;
		}
		hn->n = n;
	}
}

void hashtable_remove_node(Node *n) {
	HashNode *hn, *tmp;
	int idx = hash(n->name);
	for (tmp = AllNodes[idx], hn = tmp; tmp->n != n; hn = tmp, tmp = tmp->next);
	hn->next = tmp->next;
	if (hn == tmp) AllNodes[idx] = NULL;
	free(tmp);
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
