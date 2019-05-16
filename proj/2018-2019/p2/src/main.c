#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "node.h"


/* Variáveis globais */
#define BUFFER_SIZE 1604 /* 1024 + 512 + 64 + 2 */
char buffer[BUFFER_SIZE];

/* Obtém comandos */
char getcmd() {
	char cmd;
	fgets(buffer, BUFFER_SIZE, stdin);
	buffer[strlen(buffer)-1] = 0; /* Remover o '\n' */
	if (buffer[0] == 0) return 0;
	cmd = strtok(buffer, " ")[0];
	return cmd;
}

/* Main */
int main(void) {
	bool loop = true;

	List list, *l = &list;
	l->first = NULL;

	while (loop) {
		char *name, *email, *phone;
		char cmd = getcmd();

		switch (cmd) {
		case 'a': /* Adiciona um novo contacto. */
			name = strtok(NULL, " ");
			email = strtok(NULL, " ");
			phone = strtok(NULL, " ");
			list_add_node(l, name, email, phone);
			break;
		case 'l': /* Lista todos os contactos pela ordem em que foram introduzidos. */
			list_print(l);
			break;
		case 'p': /* Procura um contacto. */
			list_print_node(l, strtok(NULL, " "));
			break;
		case 'r': /* Apaga um contacto. */
			list_remove_node(l, strtok(NULL, " "));
			break;
		case 'e': /* Altera o endereço do email de um contacto. */
			name = strtok(NULL, " ");
			phone = strtok(NULL, " ");
			list_change_email(l, name, phone);
			break;
		case 'c': /* Conta o número de ocorrências de um domínio de email. */
			list_count_occurrences(l, strtok(NULL, " "));
			break;
		case 'x': /* Sai do programa */
			loop = false;
			break;
		}
	}

	list_destroy(l);

	return 0;
}
