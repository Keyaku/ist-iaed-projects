#ifndef __NODE_H__
#define __NODE_H__

/* Estrutura Node */
typedef struct node Node;
struct node {
	char name[1024],
		email[512],
		phone[64];
	Node *prev, *next;
};

/* Estrutura List, que contém o primeiro nó */
typedef struct list {
	Node *first;
} List;


/* Protótipos */
Node *node_new(char *name, char *email, char *phone);

void list_add_node(List *l, Node *to_add);
void list_print(List *l);
void list_print_node(List *l, char *name);
void list_remove_node(List *l, char *name);
void list_change_email(List *l, char *name, char *email);
void list_count_occurrences(List *l, char *domain);
void list_destroy(List *l);


#endif