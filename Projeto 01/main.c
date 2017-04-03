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

int verboseEnabled = 1;
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

void prettyPrintVertexList (node_L *vList){
	int 		idx=1;
	t_vertex 	*cur;
	aluno 		*theAluno;

	printf("Posição\tMatricula\tNome\tGrau\n");
	while(vList){
		cur = (t_vertex*)vList->data;
		theAluno = (aluno*)cur->content;

		printf("%d:\t%d\t%s\t%d\n", idx, theAluno->matricula, theAluno->nome, cur->length);

		vList = vList->next; idx++;
	}
	printf("\n");
}

void printVertexList (node_L *list){

	printf("{");
	while(list){
		aluno *curAluno = (aluno*)((t_vertex*)list->data)->content;
		int alunoID = (curAluno - studentObjectsPool);
		/*printf(" %d;", curAluno->matricula);*/
		printf(" %d;", alunoID+1);


		list = list->next;
	}
	printf(" }");
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
	destroyList(alunoGrau);

	/*p = gVertexList(studentGraph);*/
	printVertexList(alunoGrau);

	engineRequestStageDelete();
	fclose(infile);

	return 0;
}

