/*
	2017-1
	Teoria e Aplicação de Grafos - Turma A
	Prof. Díbio Borges

	Aluno: Cristiano Cardoso 		15/0058349
	Aluno: João Pedro Silva Sousa	15/0038381
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h> /*ANSI C. See: https://goo.gl/7YrFxn*/

#include "localization.h"
#include "ext_string.h"
#include "Headers/graph.h"
#include "Headers/list.h"

#define APP_VERSION	0.2 /*increment each milestone*/
#define IN_FILE 	"amigos_tag20171.txt"
#define ERR_FATAL 	-1
#define STR_BUFFER	64 /*longest full name is 36*/
#define MAX_ALUNOS	39



typedef struct aluno { /* vértice do grafo */
	char 	nome[STR_BUFFER];
	int 	matricula;
} aluno;

/*
Peform memory allocation for a stage scene.
A stage does contain a certain number of objects.
They are pre-allocated in a pool to avoid a high number of malloc calls.
Malloc is known to be slow if called too many times
See more: Pool Memory Allocation
*/
aluno 	*studentObjectsPool = NULL;
int 	studentObjectsPool_numElements = -1;
int 	studentObjectsPool_maxElements = -1;
t_graph *studentGraph = NULL;
int engineRequestStageAlloc(int pool_size){
	studentObjectsPool = malloc(pool_size * sizeof(aluno));
	studentObjectsPool_numElements = 0;
	studentObjectsPool_maxElements = pool_size;

	studentGraph = createGraph (pool_size);

	return (studentObjectsPool && studentGraph);
}
void engineRequestStageDelete(){
	free(studentObjectsPool);
	studentObjectsPool_numElements = -1;
	studentObjectsPool_maxElements = -1;

	destroyGraph(studentGraph);
}
int addAluno(int matricula, char *nome){
	if (studentObjectsPool_numElements == studentObjectsPool_maxElements)
		return -1;

	aluno 	*theAluno;
	int 	idAluno = studentObjectsPool_numElements;

	theAluno = &studentObjectsPool[idAluno];
	theAluno->matricula = matricula;
	strcpy(nome, theAluno->nome);
	studentGraph->vertexArray[idAluno].content = theAluno;

	studentObjectsPool_numElements = idAluno+1;

	return idAluno;
}

int verboseEnabled = 0;
void printVerbose(const char *format, ...){ /*source: https://goo.gl/sPKGhi*/
	if (! verboseEnabled)
		return;

	va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

int parseAlunos(FILE *pfile){
	int 	alunoID, matricula;
	char 	nome[STR_BUFFER], links[STR_BUFFER], *links_cur;

	int lineNumber=0;
	int read_c, link_c, amigoID;
	while(1){
		read_c = fscanf (pfile, "%d | %[^|] |%[^\n]", &matricula, nome, links);

		if (read_c == EOF){
			printVerbose("(I) Fim do arquivo %s\n", IN_FILE);
			break;
		}
		if (read_c != 3){
			printVerbose("(W) Erro na leitura de linha %d\n", lineNumber);
			continue; /*todo: warning, this causes infinite loop*/
		}
		alunoID = addAluno(matricula, nome);
		printVerbose("(I) Novo aluno: id-%d matricula-%d\n", alunoID, matricula);

		link_c=0; links_cur = links;
		while (1){
			if (sscanf(links_cur, " %d", &amigoID) != 1)
				break;

			if (amigoID == -1){
				printVerbose("(I) Aluno id-%d não possui amigos.\n", alunoID);
				break;
			}

			addEdge(studentGraph, alunoID, amigoID-1); /*IN_FILE aluno's ID start from 1 not 0*/

			links_cur = findNextChar(links_cur+1, ' ');
			link_c++;
		}

		printVerbose("(I) Lidos %d amigos de %d\n", link_c, matricula);
		lineNumber++;
	}

	return -1;
}

node_L *AlunoSortedList(){
	node_L 		*degreeSoted;
	t_vertex 	*graphVertexPool = studentGraph->vertexArray;
	t_vertex	*m, *n;

	if (! graphVertexPool)
		return NULL;

	degreeSoted = createList(graphVertexPool);

	int idx=1;
	while (idx < MAX_ALUNOS){
		m = &graphVertexPool[idx];

		/*insert element m of the pool into the list mantaining the descending order*/
		node_L *cur = degreeSoted;
		while (1){
			n = (t_vertex *)cur->data;

			if (m->length > n->length){
				if (! insertHere(cur->prev, m)){
					degreeSoted = pushToBegining(degreeSoted, m);
				}
				break;
			}

			if (! cur->next){
				insertHere(cur, m);
				break;
			}
			cur = cur->next;
		}

		idx++;
	}

	return degreeSoted;
}

int listSize(node_L *l){
	int r=0;

	while(l){
		r++; l = l->next;
	}
	return r;
}

void prettyPrintCliqueList(node_L *l){
	printf("\n");
	int i = 0; int alunoID;
	while(l){
		node_L *clique;
		clique = (node_L*)l->data;
		printf("Clique número %d: {", i);
		while(clique){
			t_vertex *v1 = (t_vertex*)clique->data;

			aluno *curAluno = (aluno*)v1->content;
			alunoID = (curAluno - studentObjectsPool);

			printf(" %d", alunoID+1);

			clique = clique->next;
		}

		printf("}\n");
		i++;
		l = l->next;
	}
}

void prettyPrintVertexList (node_L *vList){
	int 		idx=1;
	t_vertex 	*cur;
	aluno 		*theAluno;

	printf("Posiçao\tMatricula\tNome\tGrau\n");
	while(vList){
		cur = (t_vertex*)vList->data;
		theAluno = (aluno*)cur->content;

		printf("%d:\t%d\t%s\t%d\n", idx, theAluno->matricula, theAluno->nome, cur->length);

		vList = vList->next; idx++;
	}
	printf("\n");
}

void printVertexList (node_L *list){
	if (!verboseEnabled)
		return;

	printf("{");
	while(list){
		aluno *curAluno = (aluno*)((t_vertex*)list->data)->content;
		int alunoID = (curAluno - studentObjectsPool);
		/*printf(" %d;", curAluno->matricula);*/
		printf(" %d;", alunoID+1);


		list = list->next;
	}
	printf(" }\n");
}

void printAdjList (t_node *adj){
	if (!verboseEnabled)
		return;

	printf("{");
	while(adj){
		aluno *curAluno = (aluno*)adj->vertex->content;
		int alunoID = (curAluno - studentObjectsPool);
		printf(" %d;", alunoID+1);

		adj = adj->next;
	}
	printf(" }\n");
}

#define emptyList() NULL
#define edgesOf(v) v->head

void alunoPrint(t_vertex *v){
	if (!verboseEnabled)
		return;

	aluno *curAluno = (aluno*)v->content;
	int alunoID = (curAluno - studentObjectsPool);
	printf("%d", alunoID+1);
}

node_L *listIntersection(node_L *a, t_node *b){
	node_L *newList = emptyList();
	node_L *cur;

	while(b){
		cur = a;
		while(cur){
			t_vertex *v1,*v2;
			v1 = (t_vertex*)cur->data;
			v2 = (t_vertex*)b->vertex;
			/*printf("v1:"); alunoPrint(v1);
			printf(" v2:"); alunoPrint(v2); printf("; ");*/
			if (v1 == v2){
				newList = insertHere2(newList, v1);
				break;
			}

			cur = cur->next;
		}
		/*printf("\n");*/

		b = b->next;
	}
	/*printf("\n");*/

	return newList;
}

node_L *listCopy(node_L *l){
	node_L *newList = emptyList();

	while(l){
		newList = insertHere2(newList, l->data);

		l = l->next;
	}

	return newList;
}

node_L *listRemove(node_L *l, t_vertex *data){
	node_L *newList = l;

	while(l){
		t_vertex *w;
		w = (t_vertex*)l->data;

		if (w == data){
			if (l->prev == NULL) { /*start of list*/
				newList = l->next;
				if (l->next)
					l->next->prev = NULL;
			}

			if (l->prev != NULL && l->next != NULL){
				l->prev->next = l->next;
				l->next->prev = l->prev;
			}

			if (l->next == NULL){
				if (l->prev)
					l->prev->next = NULL;
			}

			free(l);
		}

		l = l->next;
	}

	return newList;
}

node_L *maxCliqueSet = NULL;
void insertSortedClique (node_L *clique){

	if (!maxCliqueSet){
		maxCliqueSet = clique;
		return;
	}
	node_L *cur = maxCliqueSet;

	while(cur){

		if (listSize(clique) > listSize(cur)){
			if (! insertHere(cur->prev, clique)){
				maxCliqueSet = pushToBegining(maxCliqueSet, clique);
			}
			break;
		}

		if (!cur->next){
			insertHere(cur, clique);
			break;
		}
		cur = cur->next;
	}
}

void _bk(node_L *r, node_L *p, node_L *x){
	printVerbose("--- _bk\n");
	printVerbose("\tr: "); printVertexList(r);
	printVerbose("\tp: "); printVertexList(p);
	printVerbose("\tx: "); printVertexList(x);

	if (p == emptyList() && x == emptyList()){
		/*insertSortedClique(listCopy(r));*/
		maxCliqueSet = insertHere2(maxCliqueSet, listCopy(r));
	}

	t_vertex *v;
	node_L *rORv, *pANDnv, *xANDnv;
	while(p){
		v = (t_vertex*)p->data;
		printVerbose("<--v ");

		printVerbose("\tnv: "); printAdjList(edgesOf(v));

		rORv = insertHere2(listCopy(r), v);
		pANDnv = listIntersection(p, edgesOf(v));
		xANDnv = listIntersection(x, edgesOf(v));

		printVerbose("\t rORv: "); printVertexList(rORv);
		printVerbose("\t pANDnv: "); printVertexList(pANDnv);
		printVerbose("\t xANDnv: "); printVertexList(xANDnv);

		_bk(rORv, pANDnv, xANDnv);

		p = listRemove(p, v);
		x = insertHere2(x, v);

		printVerbose("v-->");
	}
}

void bk (node_L *vertexList){
	maxCliqueSet = emptyList();

	_bk(emptyList(), vertexList, emptyList());
}

int main (int argc, const char *argv[]){
	FILE 	*infile;
	char 	*line = NULL;

	/*Verificar suporte ao UTF8*/
	if (!enableUTF8support()){
		printf("(W) Nao foi possivel habilitar suporte a acentuacao. Alguns caracters nao serao exibidos corretamente.\n");
	}


	infile = fopen(IN_FILE, "r");
	if (!infile){
		printf("(E) Não foi possível abrir arquivo de entrada %s. Abortando operação.\n", IN_FILE);
		return ERR_FATAL;
	}

	if (! engineRequestStageAlloc(MAX_ALUNOS)){
		printf("(E) Falha ao alocar memória para processamento. Abortando operação\n");
		fclose(infile);
		return ERR_FATAL;
	}

	parseAlunos(infile);

	node_L *alunoGrau = AlunoSortedList();
	prettyPrintVertexList(alunoGrau);
	/*destroyList(alunoGrau);*/

	/*p = gVertexList(studentGraph);*/
	/*printVertexList(alunoGrau);*/
	/*t_vertex *cur = (t_vertex*)alunoGrau->data;*/
	/*printAdjList(cur->head);*/

	bk(alunoGrau);
	prettyPrintCliqueList(maxCliqueSet);


	engineRequestStageDelete();
	fclose(infile);

	return 0;
}

