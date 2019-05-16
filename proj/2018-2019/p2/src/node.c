#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "node.h"

#define unavailable() printf("Nome inexistente.\n");
#define conflict() printf("Nome existente.\n");

/* Estrutura Node */
typedef struct node Node;
struct node {
	char name[1024],
		email[512],
		phone[64];
	Node *prev, *next;
};

/* Estrutura de hashing */
#define HASHTABLE_SIZE 501

/* Funções Node */
Node *node_new(char *name, char *email, char *phone) {
	Node *n = malloc(sizeof(Node));
	if (name != NULL) strcpy(n->name, name);
	if (email != NULL) strcpy(n->email, email);
	if (phone != NULL) strcpy(n->phone, phone);
	n->prev = n->next = NULL;
	return n;
}

Node *node_find(Node *n, char *name) {
	for (; n != NULL && strcmp(n->name, name) != 0; n = n->next);
	return n;
}

bool node_change(Node *n, char *email, char *phone) {
	if (n == NULL) return false;
	if (email) strcpy(n->email, email);
	if (phone) strcpy(n->phone, phone);
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
	free(tmp);
	return n != NULL;
}

/* Funções List */
void list_add_node(List *l, char *name, char *email, char *phone) {
	Node *tmp, *n = l->first;
	Node *to_add = node_new(name, email, phone);

	if (n == NULL) {
		l->first = to_add;
		return;
	}

	for (tmp = n; tmp != NULL; n = tmp, tmp = tmp->next) {
		if (strcmp(tmp->name, to_add->name) == 0) {
			conflict();
			free(to_add);
			return;
		}
	}
	n->next = to_add;
	to_add->prev = n;
}

void list_print(List *l) {
	Node *n;
	for (n = l->first; n != NULL; n = n->next) {
		node_print(n);
	}
}

void list_print_node(List *l, char *name) {
	Node *n = node_find(l->first, name);
	n != NULL ? node_print(n) : unavailable();
}

void list_remove_node(List *l, char *name) {
	Node *n = node_find(l->first, name);
	if (l->first == n && n != NULL) l->first = n->next;
	n != NULL ? node_destroy(n) : unavailable();
}

void list_change_email(List *l, char *name, char *email) {
	Node *n = node_find(l->first, name);
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
	if (n != NULL) {
		while (node_destroy(n->next));
		node_destroy(n);
		l->first = NULL;
	}
}
