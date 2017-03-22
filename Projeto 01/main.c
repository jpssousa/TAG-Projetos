/*
	2017-1
	Teoria e Aplicação de Grafos - Turma A
	Prof. Díbio Borges

	Aluno: Cristiano Cardoso 		15/0058349
	Aluno: João Pedro Silva Sousa	15/0038381
*/
#include <stdio.h>
#include <stdlib.h>

#include "localization.h"
#include "ext_string.h"

#define APP_VERSION	0.01 /*increment each milestone*/
#define IN_FILE 	"amigos_tag20171.csv"/*"amigos_tag20171.txt"*/
#define ERR_FATAL 	-1
#define TAM_NOME	32

typedef struct aluno aluno;
typedef struct lista_adjacencia l_adj;

typedef struct lista_adjacencia {
	aluno 	*valuno;
	l_adj 	*prox;
} l_adj;
typedef struct aluno { /* vértice do grafo */
	char 	nome[TAM_NOME];
	int 	matricula;
	l_adj 	*amigos;
} aluno;

int parseAluno(char *linha){
	int matricula; char nome[TAM_NOME];

	if (sscanf(linha, "%d, %[^,]32s,", &matricula, nome) != 2)
		return -1;

	linha = findCharAt(linha, ',', 2);

	printf("\tMatri.: %d; Nome: %s\n Str: %s\n", matricula, nome, linha);

	/*expects "[0-9]+,}?/g"*/

	return -1;
}

/*
Peform memory allocation for a stage scene.
A stage does contain a certain number of objects.
They are pre-allocated in a pool to avoid a high number of malloc calls.
Malloc is known to be slow if called too many times
See more: Pool Memory Allocation
*/
aluno 	*studentObjectsPool = NULL;
int 	studentObjectsPool_numElements;
int 	studentObjectsPool_maxElements;
aluno *engineRequestStageAlloc(int pool_size){
	studentObjectsPool = malloc(pool_size * sizeof(aluno));
	studentObjectsPool_numElements = 0;
	studentObjectsPool_maxElements = pool_size;

	return studentObjectsPool;
}
int addAluno(int *matricula, char *nome){
	if (studentObjectsPool_numElements == studentObjectsPool_maxElements)
		return -1;

	aluno *theAluno;
	theAluno = &studentObjectsPool[++studentObjectsPool_numElements];
	theAluno->matricula = *matricula;
	theAluno->nome[0] = '\0';

	return studentObjectsPool_numElements;
}



int main (){
	FILE 	*infile;
	char 	*line = NULL;
	size_t 	read_c, len;


	/*Verificar suporte ao UTF8*/
	if (!enableUTF8support()){
		printf("(W) Nao foi possivel habilitar suporte a acentuacao. Alguns caracters nao serao exibidos corretamente.\n");
	}


	infile = fopen(IN_FILE, "r");
	if (!infile){
		printf("(E) Não foi possível abrir arquivo de entrada %s. Abortando operação.\n", IN_FILE);
		return ERR_FATAL;
	}

	/*Ler Header*/
	int numItems;
	if (fscanf(infile, "%d\n", &numItems) == 0){
		printf("(E) Falha na leitura do cabelhaço de %s. Abortando operação.\n", IN_FILE);
		return ERR_FATAL;
	}
	if (! engineRequestStageAlloc(numItems)){
		printf("(E) Falha ao alocar memória para processamento.\n");
		return ERR_FATAL;
	}

	int idx=1, idAluno;
	while(1){
		read_c = getline(&line, &len, infile);
		if (read_c == -1)
			break;

		idAluno = parseAluno(line);
		if (idAluno == -1){
			printf("(E) Linha %d inválida. Ignorando linha.\n", idx);
		}
		idx++;	
	}

	fclose(infile);

	return 0;
}