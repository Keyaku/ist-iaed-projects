#ifndef __PROJECTO_HASH_TABLE_H__
#define __PROJECTO_HASH_TABLE_H__

#include "node.h"

/* Estrutura de hashing */
#define HASHTABLE_SIZE 501

typedef struct hash_node HashNode;
struct hash_node {
	Node *n;
	HashNode *next;
};

HashNode *AllNodes[HASHTABLE_SIZE];

/* Protótipos */
void hashtable_initialize();
HashNode *hashtable_find_node(const char *name);
void hashtable_add_node(Node *n);
void hashtable_remove_node(Node *n);
void hashtable_destroy();

#endif
