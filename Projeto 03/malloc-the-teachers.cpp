/**
 * Aluno: João Pedro Silva Sousa 15/0038381
 * TODO: nome, matrícula e identificação do problema
 *
 * Programa escrito em liguagem C++
 * Compilador utilizado :$ g++ --version
 * g++ (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609
 * Copyright (C) 2015 Free Software Foundation, Inc.
 *
 * Comando de compilação: $ g++ -Wall -std=c++11 malloc-the-teachers.cpp -o match
 * Execução: $ ./match
 *
 */


#include <bits/stdc++.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#define NUM_PROFESSORS 100
#define NUM_SCHOOLS 50
#define FILE_NAME "input.txt"
#define PRF_START 130
#define SCH_START 3719
#define MAX_PROFS 2
#define PREFS 5
#define ERROR -1


using namespace std;


class School;
class Professor;


class Professor {

    public:
        int id;
        int linked_to;
        int num_skills;
        int prefs[PREFS];
        int idx;

        // constructor
        Professor(int _id, int _n) {
            this->id = _id;
            this->num_skills = _n;
            this->linked_to = -1;
            this->idx = -1;
        }

        // destructor
        ~Professor();

        // methods
        void setPreferences (int * _arr) {
            for (int i = 0; i < PREFS; i++) {
                this->prefs[i] = _arr[i];
            }
        }

        bool isFree (void) {
            return (this->linked_to == -1);
        }

        int propose (void) {
            this->idx++;
            if (this->idx >= PREFS)
                throw invalid_argument ("Professor.propose: out of range");
            return prefs[this->idx];
        }

        void link (int sch_id) {
            if (sch_id < 0)
                throw invalid_argument ("Professor.link: Invalid school.");
            this->linked_to = sch_id;
        }

        void getsDumped (void) {
            this->linked_to = -1;
        }
    
};


class School {

    public:
        int id;
        int req_skills;
        int linked_to[MAX_PROFS];

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

        //methods
        bool isFree (void) {
            return (this->linked_to[0] == -1 || this->linked_to[1] == -1);
        }

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

        void link (int index, int prof) {
            this->linked_to[index] = prof;
        }

        int kicks (int i) {
            int prof_id = this->linked_to[i];
            this->linked_to[i] = -1;
            return prof_id;
        }
};


int getFreeProfessor (Professor ** prof) {
    for (int i = 0; i < NUM_PROFESSORS; i++) {
        if (prof[i]->isFree ()) {
            return i;
        }
    }
    //throw invalid_argument ("Non avaiable professors.");
    return ERROR;
}


void stableMatching (Professor ** profArray, School ** schArray) {
    int alone_prof_id, voked_school;
    Professor * p_cur;
    School * s_cur;

    while (true) {
        alone_prof_id = getFreeProfessor (profArray);
        if (alone_prof_id == -1) {
            break;
        }

        p_cur = profArray[alone_prof_id];
        voked_school = p_cur->propose()-1;
        s_cur = schArray[voked_school];

        if (s_cur->isFree ()) {
            for (int i = 0; i < MAX_PROFS; i++) {
                if (s_cur->linked_to[i] == -1) {
                    //printf("linking professor [%d] to school [%d]\n", p_cur->id-1, s_cur->id-1);
                    s_cur->link (i, alone_prof_id); // link school to professor
                    p_cur->link (s_cur->id-1); // link professor to school
                    break;
                }
            }
        }
        else {
            for (int i = 0; i < MAX_PROFS; i++) {
                if (s_cur->prefers (p_cur, profArray[s_cur->linked_to[i]]) && (profArray[s_cur->linked_to[i]]->idx < PREFS-1)) {
                    //printf("removing link: professor [%d] to school [%d]\n", s_cur->linked_to[i], s_cur->id-1);
                    int to_kick = s_cur->kicks (i); // kicks professor
                    profArray[to_kick]->getsDumped (); // undo link between professor and school
                    //printf("linking professor [%d] to school [%d]\n", p_cur->id-1, s_cur->id-1);
                    s_cur->link (i, alone_prof_id); // link school to new professor
                    p_cur->link (s_cur->id-1); // link professor to school
                    break;
                }
            }
        }


    }

    /*
    while (exist free man m who still has a woman w to propose to) {
        w = first woman on m's list to whom m has not proposed yet
        if (w is free) {
            (m, w) become engaged
        }
        else { // some pair (m', w) already exists
            if (w prefers m to m') {
                m' becomes free
                (m, w) become engaged
            }
            else {
                (m', w) remain engaged
            }
        }
    }
    */
}


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

        /*printf("(P%d, %d): (E%d, E%d, E%d, E%d, E%d)\n",
            id, skill, preference[0], preference[1], preference[2], preference[3], preference[4]);*/
    }

    fp.seekg (SCH_START, ios_base::beg);

    for (int i = 0; i < NUM_SCHOOLS; i++) {
        getline (fp, line);
        sscanf (line.c_str (), "(E%d):(%d)", &id, &skill);

        schArray[i] = new School (id, skill);
        //printf("(E%d):(%d)\n", id, skill);
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
