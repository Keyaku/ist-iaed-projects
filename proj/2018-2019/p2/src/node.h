#ifndef __NODE_H__
#define __NODE_H__

/* Estrutura Node */
typedef struct node Node;
struct node {
	char *name, *email, *domain, *phone;
	Node *prev, *next;
};

/* Estrutura List, que contém o primeiro nó */
typedef struct list {
	Node *first, *last;
} List;


/* Protótipos */
bool node_destroy(Node *n);

List *list_new();
void list_add_node(List *l, char *name, char *email, char *phone);
void list_remove_node(List *l, char *name);
void list_print(List *l);
void list_print_node(char *name);
void list_change_email(char *name, char *email);
void list_count_occurrences(List *l, char *domain);
void list_destroy(List *l);


#endif
