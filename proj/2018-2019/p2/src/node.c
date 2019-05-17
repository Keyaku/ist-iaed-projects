#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "node.h"
#include "hash.h"

#define unavailable() printf("Nome inexistente.\n");
#define conflict() printf("Nome existente.\n");

/* Funções Node */
Node *node_new(char *name, char *email, char *phone) {
	Node *n = malloc(sizeof(Node));
	if (name != NULL) n->name = strndup(name, 1024);
	if (email != NULL) {
		char *at = strchr(email, '@');
		n->email = strndup(email, strlen(email) - strlen(at));
		n->domain = strndup(at+1, strlen(at));
	}
	if (phone != NULL) n->phone = strndup(phone, 64);
	n->prev = n->next = NULL;
	return n;
}

Node *node_find(char *name) {
	HashNode *hn = hashtable_find_node(name);
	return hn != NULL ? hn->n : NULL;
}

bool node_change(Node *n, char *email, char *phone) {
	if (n == NULL) return false;
	if (email) {
		char *at = strchr(email, '@');
		free(n->email); free(n->domain);
		n->email = strndup(email, strlen(email) - strlen(at));
		n->domain = strndup(at+1, strlen(at));
	}
	if (phone) {
		free(n->phone);
		n->phone = strndup(phone, 64);
	}
	return true;
}

bool node_print(Node *n) {
	if (n == NULL) return false;
	printf("%s %s@%s %s\n", n->name, n->email, n->domain, n->phone);
	return true;
}

bool node_destroy(Node *n) {
	if (n == NULL) return false;

	if (n->prev != NULL) n->prev->next = n->next;
	if (n->next != NULL) n->next->prev = n->prev;

	if (n->name) free(n->name);
	if (n->email) free(n->email);
	if (n->domain) free(n->domain);
	if (n->phone) free(n->phone);

	n->name = n->email = n->phone = NULL;
	n->next = n->prev = NULL;
	free(n);
	return true;
}

/* Funções List */
List *list_new() {
	List *l = calloc(1, sizeof(List));
	hashtable_initialize();
	return l;
}

void list_add_node(List *l, char *name, char *email, char *phone) {
	Node *n, *to_add;

	/* Adiciona o nó na HashTable (se possível) */
	if (node_find(name) != NULL) {
		conflict();
		return;
	}
	to_add = node_new(name, email, phone);
	hashtable_add_node(to_add);

	if (l->first == NULL) {
		l->first = to_add;
	}
	else {
		/* Adicona o nó na lista */
		n = l->last;
		n->next = to_add;
		to_add->prev = n;
	}

	l->last = to_add;
}

void list_remove_node(List *l, char *name) {
	Node *n = hashtable_remove_node(name); /* Remover o nó da HashTable */
	if (n != NULL) {
		if (n == l->first) l->first = n->next;
		if (n == l->last) l->last = n->prev;
		node_destroy(n); /* Apagar o nó da memória */
	}
	else unavailable();
}

void list_print(List *l) {
	Node *n;
	for (n = l->first; n != NULL; n = n->next) {
		node_print(n);
	}
}

void list_print_node(char *name) {
	Node *n = node_find(name);
	n != NULL ? node_print(n) : unavailable();
}

void list_change_email(char *name, char *email) {
	Node *n = node_find(name);
	n != NULL ? node_change(n, email, NULL) : unavailable();
}

void list_count_occurrences(List *l, char *domain) {
	int count = 0;
	Node *n;

	for (n = l->first; n != NULL; n = n->next) {
		if (strcmp(n->domain, domain) == 0) count++;
	}

	printf("%s:%d\n", domain, count);
}

void list_destroy(List *l) {
	while (l->first != NULL) {
		Node *n = l->first->next;
		node_destroy(l->first);
		l->first = n;
	}
	hashtable_destroy();
	free(l);
}
