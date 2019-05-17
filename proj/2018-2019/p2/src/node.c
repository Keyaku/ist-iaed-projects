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
	if (email != NULL) n->email = strndup(email, 512);
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
		free(n->email);
		n->email = strndup(email, 512);
	}
	if (phone) {
		free(n->phone);
		n->phone = strndup(phone, 64);
	}
	return true;
}

bool node_print(Node *n) {
	if (n == NULL) return false;
	printf("%s %s %s\n", n->name, n->email, n->phone);
	return true;
}

bool node_destroy(Node *n) {
	Node *tmp = n;
	if (n == NULL) return false;
	if (n->prev != NULL) {
		n = n->prev;
		n->next = n->next->next;
		if (n->next) n->next->prev = n;
	}
	else if (n->next != NULL) {
		n = n->next;
		n->prev = NULL;
	} else {
		n = NULL;
	}

	if (tmp->name) free(tmp->name);
	if (tmp->email) free(tmp->email);
	if (tmp->phone) free(tmp->phone);

	tmp->name = tmp->email = tmp->phone = NULL;
	free(tmp);
	return n != NULL;
}

/* Funções List */
List *list_new() {
	List *l = calloc(1, sizeof(List));
	hashtable_initialize();
	return l;
}

void list_add_node(List *l, char *name, char *email, char *phone) {
	Node *n, *to_add = node_new(name, email, phone);

	if (l->first == NULL) {
		l->first = to_add;
	}
	else {
		if (hashtable_find_node(name) != NULL) {
			conflict();
			free(to_add);
			return;
		}

		/* Adicona o nó na lista */
		n = l->last;
		n->next = to_add;
		to_add->prev = n;
	}

	/* Adiciona o nó na HashTable */
	hashtable_add_node(to_add);
	l->last = to_add;
}

void list_remove_node(List *l, char *name) {
	Node *n = node_find(name);
	if (n != NULL) {
		if (n == l->first) l->first = n->next;
		if (n == l->last) l->last = n->prev;

		hashtable_remove_node(n); /* Remover o nó da HashTable */
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
		if (strcmp(strchr(n->email, '@')+1, domain) == 0) count++;
	}

	printf("%s:%d\n", domain, count);
}

void list_destroy(List *l) {
	Node *n = l->first;
	hashtable_destroy();
	if (n != NULL) {
		while (node_destroy(n->next));
		node_destroy(n);
		l->first = l->last = NULL;
	}
	free(l);
}
