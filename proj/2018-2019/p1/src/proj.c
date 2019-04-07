#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_BUFFER 256
#define TAMANHO 63
#define NUM_PESSOAS 4
#define NUM_SALAS 10
#define NUM_EVENTOS 100


void adiciona_participante();
void remove_participante();


typedef unsigned char bool;
enum { false = 0, true = 1 };

typedef struct evento{

	char descricao[TAMANHO];
	char pessoa[NUM_PESSOAS][TAMANHO];
	int dia, mes, ano;
	int hora, min;
	int duracao;
	int sala;

} Evento;

Evento Todos[NUM_SALAS][NUM_EVENTOS]; /* Matriz de Eventos */
int ContaTodos[NUM_SALAS]; /* Número de Eventos registados por sala */
int SalaDescoberta; /* Variável auxiliar de procura_todas_salas() */

/************************************ Auxiliares ************************************/
/* Variados */
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

/* Comparadores */
int cmp_evento(const void *a, const void *b) {
	Evento ev1 = *(Evento*) a, ev2 = *(Evento*) b;
	int diff = (ev2.dia - ev1.dia) + (ev2.mes - ev1.mes) + (ev2.ano - ev1.ano);
	diff += (ev2.hora * 60 + ev2.min + ev2.duracao) - (ev1.hora * 60 + ev1.min + ev1.duracao);

	if (diff == 0)
		diff = (ev2.sala - ev1.sala);

	return diff;
}

bool evento_livre(Evento ev1, Evento ev2) {
	ev1.sala = ev1.sala; /* Para fazer desaparecer um warning */
	return ev2.sala == 0;
}
bool evento_ocupado(Evento ev1, Evento ev2) {
	/* Caso haja um evento exatamente no mesmo dia e na mesma sala */
	int diff = cmp_evento((void*)&ev1, (void*)&ev2);
	diff = max(diff, -diff); /* Acertar a polaridade */
	return (0 <= diff && diff < ev1.duracao && 0 <= diff && diff < ev2.duracao);
}
bool evento_sobreposto(Evento ev1, Evento ev2) {
	int i, j;
	/* Caso haja uma mesma pessoa */
	for (i = 0; i < NUM_PESSOAS; i++) {
	for (j = 0; j < NUM_PESSOAS; j++)
		if (evento_ocupado(ev1, ev2) && strcmp(ev1.pessoa[i], ev2.pessoa[j]) == 0){
			return true;
		}
	}
	return false;
}
bool evento_pela_descricao(Evento ev1, Evento ev2) {
	return strcmp(ev1.descricao, ev2.descricao) == 0;
}

/* Funções de procura */
int procura_evento(Evento ev, int sala, bool (*fn)(Evento, Evento)) {
	int i;
	Evento *lista = Todos[sala];

	for (i = 0; i < ContaTodos[sala]; i++){
		if (fn(ev, lista[i]) == true) break;
	}

	return i;
}

int procura_todas_salas(Evento ev, bool (*fn)(Evento, Evento)) {
	int i;

	for (i = 0; i < NUM_SALAS; i++) {
		int res = procura_evento(ev, i, fn);
		if (res < ContaTodos[i]) {
			SalaDescoberta = i;
			return res;
		}
	}

	SalaDescoberta = -1;
	return -1;
}

bool pode_agendar_evento(Evento ev) {
	int sala = ev.sala-1, ocupado, sobreposto;

	ocupado = procura_evento(ev, sala, evento_ocupado);
	sobreposto = procura_todas_salas(ev, evento_sobreposto);

	if (ocupado < ContaTodos[sala] || sobreposto != -1) {
		if (ocupado < ContaTodos[sala])
			printf("Impossivel agendar evento %s. Sala%d ocupada.\n", ev.descricao, ev.sala);
		else if (sobreposto != -1) {
			for (sala = 0; sala < NUM_SALAS; sala++) {
				Evento ev_sobreposto = Todos[sala][sobreposto];
				int i, j;
				for (i = 0; i < NUM_PESSOAS; i++) {
				for (j = 0; j < NUM_PESSOAS; j++)
					if (evento_ocupado(ev, ev_sobreposto) && strcmp(ev.pessoa[i], ev_sobreposto.pessoa[j]) == 0){
						printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", ev.descricao, ev.pessoa[i]);
					}
				}
			}
		}
		return false;
	}

	return true;
}

/********************************* Adiciona um Evento *********************************/
void adiciona_evento_aux(Evento ev, int res) {
	int sala = ev.sala-1;
	res = procura_evento(ev, sala, evento_livre);

	Todos[sala][res] = ev;
	ContaTodos[sala]++;
	qsort(Todos[sala], ContaTodos[sala], sizeof(Evento), cmp_evento);
}

void adiciona_evento(char *frase){
	Evento ev;
	int res = 0, counter = 0, dados[4];

	memset(&ev, 0, sizeof(Evento));

	/* A apanhar o texto todo separado por ':' */
	frase = strtok(frase, ":");
	strcpy(ev.descricao, frase);

	while (counter < 4) {
		dados[counter++] = atoi(strtok(NULL, ":"));
	} counter = 0;
	while ((frase = strtok(NULL, ":"))) {
		strcpy(ev.pessoa[counter++], frase);
	}

	ev.dia = dados[0] / 1000000; ev.mes = (dados[0] / 10000) % 100; ev.ano = dados[0] % 10000;
	ev.hora = dados[1] / 100; ev.min = dados[1] % 100;
	ev.duracao = dados[2];
	ev.sala = dados[3];

	/* Adicionar evento sse a sala não estiver ocupada */
	if (pode_agendar_evento(ev))
		adiciona_evento_aux(ev, res);
}

/*********************************** Listar Eventos ***********************************/
void lista_eventos(Evento *lista, int size) {
	int i, j;

	for (i = size-1; i >= 0; i--) {
		Evento ev = lista[i];
		if (ev.sala == 0) continue;

		printf("%s %02d%02d%04d %02d%02d %02d Sala%d %s\n", ev.descricao, ev.dia,ev.mes,ev.ano, ev.hora,ev.min, ev.duracao, ev.sala, ev.pessoa[0]);
		for (j = 1; j < NUM_PESSOAS && ev.pessoa[j][0] != 0; j++) {
			if (j == 1) printf("*");
			printf(" %s", ev.pessoa[j]);
		}
		printf("\n");
	}
}

void lista_todos() {
	Evento lista[NUM_SALAS * NUM_EVENTOS];
	memcpy(lista, Todos, NUM_SALAS * NUM_EVENTOS * sizeof(Evento));
	qsort(lista, NUM_SALAS * NUM_EVENTOS, sizeof(Evento), cmp_evento);
	lista_eventos(lista, NUM_SALAS * NUM_EVENTOS);
}

void lista_sala(int sala) {
	lista_eventos(Todos[sala-1], NUM_EVENTOS);
}


/********************************** Altera Eventos ***********************************/
/* Altera Evento: a função core. Quase todas as funções passam por esta */
void altera_evento(Evento ev, void (*altera_funcao)(Evento, int)) {
	int res = procura_todas_salas(ev, evento_pela_descricao);
	if (res != -1)
		altera_funcao(ev, res);
	else
		printf("Evento %s inexistente.\n", ev.descricao);
}

/* Apaga um Evento */
void apaga_evento_aux(Evento ev, int res) {
	int sala = SalaDescoberta;
	memcpy(&ev, &Todos[sala][res], sizeof(Evento));
	memset(&Todos[sala][res], 0, sizeof(Evento)); /* por o evento todo a 0 */
	qsort(Todos[sala], ContaTodos[sala], sizeof(Evento), cmp_evento);
	ContaTodos[sala]--;
}

void apaga_evento(const char *descricao) {
	Evento ev;
	strcpy(ev.descricao, descricao);
	altera_evento(ev, apaga_evento_aux);
}

/* Altera o início */
void altera_inicio_aux(Evento ev, int res) {
	int sala = SalaDescoberta;
	int hora = ev.hora;
	int min = ev.min;
	int old_hora = Todos[sala][res].hora;
	int old_min = Todos[sala][res].min;

	memcpy(&ev, &Todos[sala][res], sizeof(Evento));
	apaga_evento_aux(Todos[sala][res], res);
	ev.hora = hora;
	ev.min = min;

	if (!pode_agendar_evento(ev)) {
		ev.hora = old_hora;
		ev.min = old_min;
	}

	adiciona_evento_aux(ev, res);
}

void altera_inicio(char *frase) {
	Evento ev;
	int inicio;

	strcpy(ev.descricao, strtok(frase, ":"));
	inicio = atoi(strtok(NULL, ":"));
	ev.hora = inicio / 100;
	ev.min = inicio % 100;

	altera_evento(ev, altera_inicio_aux);
}

/* Altera a duração */
void altera_duracao_aux(Evento ev, int res) {
	int sala = SalaDescoberta;
	int duracao = ev.duracao;
	int old_duracao = Todos[sala][res].duracao;

	memcpy(&ev, &Todos[sala][res], sizeof(Evento));
	apaga_evento_aux(Todos[sala][res], res);
	ev.duracao = duracao;

	if (!pode_agendar_evento(ev))
		ev.duracao = old_duracao;

	adiciona_evento_aux(ev, res);
}

void altera_duracao(char *frase) {
	Evento ev;

	strcpy(ev.descricao, strtok(frase, ":"));
	ev.duracao = atoi(strtok(NULL, ":"));

	altera_evento(ev, altera_duracao_aux);
}


/* Muda a sala */
void muda_sala_aux(Evento ev, int res) {
	int sala = SalaDescoberta;
	int new_sala = ev.sala;

	memcpy(&ev, &Todos[sala][res], sizeof(Evento));
	apaga_evento_aux(ev, res);
	ev.sala = new_sala;

	if (!pode_agendar_evento(ev)) {
		ev.sala = sala+1;
	}
	adiciona_evento_aux(ev, res);
}

void muda_sala(char *frase) {
	Evento ev;

	strcpy(ev.descricao, strtok(frase, ":"));
	ev.sala = atoi(strtok(NULL, ":"));

	altera_evento(ev, muda_sala_aux);
}


/* Adiciona ou remove participantes */
void adiciona_participante_aux(Evento ev, int res) {
	int i, sala = SalaDescoberta;

	for (i = 1; i < NUM_PESSOAS && strcmp(Todos[sala][res].pessoa[i], ""); i++) {
		if (strcmp(Todos[sala][res].pessoa[i], ev.pessoa[0]) == 0)
			return; /* Se ele já existe, ignorar */
	}
	if (i == NUM_PESSOAS)
		printf("Impossivel adicionar participante. Evento %s ja tem 3 participantes.\n", ev.descricao);
	else
		strcpy(Todos[sala][res].pessoa[i], ev.pessoa[0]);


}

void adiciona_participante(char *frase) {
	Evento ev;

	strcpy(ev.descricao, strtok(frase, ":"));
	strcpy(ev.pessoa[0], strtok(NULL, ":"));

	altera_evento(ev, adiciona_participante_aux);
}

void remove_participante_aux(Evento ev, int res) {
	int i, sala = SalaDescoberta;
	int idx = 1;
	for (i = 1; i < NUM_PESSOAS && Todos[sala][res].pessoa[i] != 0; i++) {
		if (strcmp(Todos[sala][res].pessoa[i], ev.pessoa[0]) == 0) idx = i;
	}
	if (i == 1)
		printf("Impossivel remover participante. Participante %s e o unico participante no evento %s.\n", ev.pessoa[0], ev.descricao);
	else
		strcpy(Todos[sala][res].pessoa[idx], ev.pessoa[0]);
}

void remove_participante(char *frase) {
	Evento ev;

	strcpy(ev.descricao, strtok(frase, ":"));
	strcpy(ev.pessoa[0], strtok(NULL, ":"));

	altera_evento(ev, remove_participante_aux);
}

/********************************* Função main *********************************/
int main() {
	bool loop = true;
	char command;
	char input[TAM_BUFFER];

	while (loop){
		/* buscar comando do input e removendo o \n */
		fgets(input, TAM_BUFFER, stdin); input[strlen(input)-1] = 0;
		command = input[0];

		switch(command){
			case 'a': /*chama a função que adiciona eventos, comando a*/
				adiciona_evento(input+2);
				break;
			case 'l': /*chama a função que lista eventos,comando l*/
				lista_todos();
				break;
			case 's': /*chama a função que lista eventos da sala, comando s*/
				lista_sala(input[2] - '0');
				break;
			case 'r': /*chama a função que apaga eventos, comando r*/
				apaga_evento(input+2);
				break;
			case 'i': /*chama a função que altera o inicio de um evento, comando i*/
				altera_inicio(input+2);
				break;
			case 't': /*chama a função que altera a duração de um evento, comando t*/
				altera_duracao(input+2);
				break;
			case 'm': /*chama a função que muda a sala de um evento,comando m*/
				muda_sala(input+2);
				break;
			case 'A': /*chama a função que adiciona participantes, comando A*/
				adiciona_participante(input+2);
				break;
			case 'R': /*chama a função que remove participantes, comando R*/
				remove_participante(input+2);
				break;
			case 'x':
				loop = false; /* o processo termina, comando x */
		}
	}

	return 0;
}

