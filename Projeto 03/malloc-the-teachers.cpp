/**
 * 2017-1
 * Teoria e Aplicação de Grafos - Turma A
 * Prof. Díbio Borges
 *
 * Aluno: João Pedro Silva Sousa    15/0038381
 * Aluno: Cristiano Cardoso         15/0058349
 *
 * Programa escrito em liguagem C++
 * Compilador utilizado :$ g++ --version
 * g++ (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609
 * Copyright (C) 2015 Free Software Foundation, Inc.

0. Observações
    - Utilize **quebra de linha** em seu editor de textos para melhor visualização do arquivo.
1. Compilação
    - Navegue até o diretorio de onde foi extraido o programa e entre com o comando
        $ make
    - Um executável entitulado psmatch deve ser gerado nesta pasta
    - Caso não funcionar, entre com o seguinte comando:
        $ g++ -Wall -std=c++11 malloc-the-teachers.cpp -o psmatch
2. Execução
    - Entre no terminal
        $ ./pspatch
    - Caso queira salvar a saida em um arquivo faça
        $ ./pspatch > saida.txt
3. Entrada
    - Se constitui em um arquivo .txt entitulado input.txt contendo os dados de professores a serem alocados
    em escolas de suas preferência, bem como as escolas com suas preferências de habilitações.
    No caso dos professores, cada um deve ter 5 escolas em que querem lecionar ordenada da maior prioridade
    para a menor (esqurda para a direita). Além disso, o professor deve informar seu número de habilitações.
    Enquanto que as escolas devem informar o número de habilitações que desejam de um professor.
    - O algoritmo apresentado neste programa, conforme o especificado no roteiro, produz um emparelhamento
    perfeito e estável onde todos os professores estão alocados em alguma escola e dentro de suas preferências.
4. Saída
    - Será uma lista onde cada linha tem o formado (En): [Pm, Po], onde n e (m,o) é o número da escola e
    professores que foram alocados para esta, respectivamente.
    - No final será apresentada a distribuição de nível de preferência por professor.
    Isto é, quantos professores ficaram alocados em sua primeira preferência, em sua segunda e assim por diante.
 */

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <map>


#define NUM_PROFESSORS 100      // número de professores a serem lidos
#define NUM_SCHOOLS 50          // número de escolas a serem lidas
#define FILE_NAME "input.txt"   // localização do arquivo de entrada
#define PRF_START 130           // byte em que se inicia a lista de professores (ignorar as linhas inválidas)
#define SCH_START 3719          // byte em que se inicia a lista de escolas
#define MAX_PROFS 2             // número máximo de professores que uma escola pode receber
#define PREFS 5                 // número máximo de preferências que um professor pode ter
#define ERROR -1                // código de erro


using namespace std;


class School;                   // vértice do grafo
class Professor;                // vértice do grafo


class Professor {

    public:
        int id;
        int linked_to;          // forma um grafo onde as arestas são o local de trabalho do professor
        int num_skills;         // conteúdo do nó do grafo
        int prefs[PREFS];       // forma um grafo onde as arestas são as preferências
        int idx;                // índice que retorna a preferência a medida que o professor vai tentando as escolas

        // constructor
        Professor(int _id, int _n) {
            this->id = _id;
            this->num_skills = _n;
            this->linked_to = -1;
            this->idx = -1;
        }

        // destructor
        ~Professor();

        // define as preferências de um professor (array lida do arquivo é copiada para o objeto)
        void setPreferences (int * _arr) {
            for (int i = 0; i < PREFS; i++) {
                this->prefs[i] = _arr[i];
            }
        }

        // verificar se o professor não está vinculado a uma escola
        bool isFree (void) {
            return (this->linked_to == -1);
        }

        // função que faz o professor realizar uma proposta a uma escola
        int propose (void) {
            this->idx++;
            if (this->idx >= PREFS)
                throw invalid_argument ("Professor.propose: out of range");
            return prefs[this->idx];
        }

        // professor passa a estar vinculado a uma escola        
        void link (int sch_id) {
            if (sch_id < 0)
                throw invalid_argument ("Professor.link: Invalid school.");
            this->linked_to = sch_id;
        }

        // professor volta a ficar livre
        void getsDumped (void) {
            this->linked_to = -1;
        }
    
};


class School {

    public:
        int id;
        int req_skills;             // propriedade do nó para verificar qual a preferência de uma escola
        int linked_to[MAX_PROFS];   // arestas do grafo resultante do emprelhamento, que ligam escola a professor

        // constructor
        School (int _id, int _sk) {
            this->id = _id;
            this->req_skills = _sk;
            for (int i = 0; i < MAX_PROFS; i++) {
                this->linked_to[i] = -1;
            }
        }

        // destructor
        ~School ();

        // verifica se há vagas na escola
        bool isFree (void) {
            return (this->linked_to[0] == -1 || this->linked_to[1] == -1);
        }

        // realiza a verficação se uma escola prefere um professor A a um B
        // retorna true se A é preferível
        bool prefers (Professor * a, Professor * b) {
            int skill_a = a->num_skills;
            int skill_b = b->num_skills;

            // TODO: sad code below
            if (skill_a >= this->req_skills && skill_b >= this->req_skills) {
                return (skill_a >= skill_b);
                //return false;
            }
            if (skill_a < this->req_skills && skill_b < this->req_skills) {
                return (skill_a <= skill_b);
                //return false;
            }
            if (skill_a < this->req_skills && skill_b >= this->req_skills) {
                return false;
            }
            if (skill_a >= this->req_skills && skill_b < this->req_skills) {
                return true;
            }
            throw invalid_argument ("School.prefers: no match.");
        }

        // realiza o link entre escola e professor
        void link (int index, int prof) {
            this->linked_to[index] = prof;
        }

        // escola se desfaz de um professor
        int kicks (int i) {
            int prof_id = this->linked_to[i];
            this->linked_to[i] = -1;
            return prof_id;
        }
};

// função retorna um professor livre da array dos professores
int getFreeProfessor (Professor ** prof) {
    for (int i = 0; i < NUM_PROFESSORS; i++) {
        if (prof[i]->isFree ()) {
            return i;
        }
    }
    //throw invalid_argument ("Non avaiable professors.");
    return ERROR;
}

// algoritmo principal para encontrar o emparelhamento
// fonte: https://en.m.wikipedia.org/wiki/Stable_marriage_problem#Algorithm
void stableMatching (Professor ** profArray, School ** schArray) {
    int fprof_id, provoked_school;
    Professor * p_cur;
    School * s_cur;

    while (true) {
        fprof_id = getFreeProfessor (profArray); // obtém professor livre
        if (fprof_id == -1) {
            break;
        }

        p_cur = profArray[fprof_id];
        provoked_school = p_cur->propose()-1; // professor pede uma escola ainda não requisitada de sua lista de preferências
        s_cur = schArray[provoked_school];

        if (s_cur->isFree ()) { // caso a escola estiver com vagas
            for (int i = 0; i < MAX_PROFS; i++) { // uma vaga é encontrada para o professor
                if (s_cur->linked_to[i] == -1) {
                    s_cur->link (i, fprof_id); // link school to professor
                    p_cur->link (s_cur->id-1); // link professor to school
                    break;
                }
            }
        }
        else {
            for (int i = 0; i < MAX_PROFS; i++) { // todos os concorrentes do professor são analisados
                if (s_cur->prefers (p_cur, profArray[s_cur->linked_to[i]]) && (profArray[s_cur->linked_to[i]]->idx < PREFS-1)) { // sucesso do professor em conseguir uma vaga em relação ao seu concorrente
                    int to_kick = s_cur->kicks (i); // kicks professor
                    profArray[to_kick]->getsDumped (); // undo link between professor and school
                    s_cur->link (i, fprof_id); // link school to new professor
                    p_cur->link (s_cur->id-1); // link professor to school
                    break;
                }
            }
        }


    }
}

// realiza a leitura dos arquivos e chama a função stable matching
void printReport (Professor ** profArray, School ** schArray) {
    int distribution[PREFS];
    string foo[] = {"primeira", "segunda", "terceira", "quarta", "quinta"};

    for (int i = 0; i < PREFS; i++) {
        distribution[i] = 0;
    }

    for (int i = 0; i < NUM_PROFESSORS; i++) {
        distribution[profArray[i]->idx]++;
    }

    printf("  Exibindo emparelhamento no formato\n  (código escola) : [código professor 1, código professor 2]\n\n");
    for (int i = 0; i < NUM_SCHOOLS; i++) {
        printf("  (E%2d): [P%3d, P%3d]\n", schArray[i]->id, schArray[i]->linked_to[0]+1, schArray[i]->linked_to[1]+1);
    }
    printf("\n\n");
    printf("  Distribuição:\n");
    for (int i = 0; i < PREFS; i++) {
        printf(" >> %2d professores alocados na %s escola da lista.\n", distribution[i], foo[i].c_str());
    }
}

// realiza a leitura dos arquivos e chama a função stable matching
int main (void) {

    ifstream fp;
    string line;

    Professor *profArray[NUM_PROFESSORS];
    School *schArray[NUM_SCHOOLS];

    int id, skill, preference[PREFS];


    fp.open(FILE_NAME);

    if (!fp.is_open ()) {
        cout << "Could not open file \"" << FILE_NAME << "\"." << endl;
        exit(1);
    }

    fp.seekg (PRF_START, ios_base::beg);

    for (int i = 0; i < NUM_PROFESSORS; i++) {
        getline (fp, line);
        sscanf (line.c_str (), "(P%d, %d): (E%d, E%d, E%d, E%d, E%d)",
            &id, &skill, &preference[0], &preference[1], &preference[2], &preference[3], &preference[4]);

        profArray[i] = new Professor (id, skill);
        profArray[i]->setPreferences (preference);
    }

    fp.seekg (SCH_START, ios_base::beg);

    for (int i = 0; i < NUM_SCHOOLS; i++) {
        getline (fp, line);
        sscanf (line.c_str (), "(E%d):(%d)", &id, &skill);

        schArray[i] = new School (id, skill);
    }

    try {
        stableMatching (profArray, schArray);
    }
    catch (invalid_argument& ia) {
        cout << "invalid_argument: " << ia.what() << endl;
    }

    printReport (profArray, schArray);
    fp.close ();

    
    return 0;
}