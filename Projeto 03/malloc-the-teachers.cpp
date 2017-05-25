#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <string>
#include <map>
#include <bits/stdc++.h>


#define NUM_PROFESSORS 100
#define NUM_SCHOOLS 50
#define PREFS 5
#define FILE_NAME "input.txt"
#define PRF_START 130
#define SCH_START 3719
#define MAX_PROFS 2
#define ERR -1

using namespace std;


class School;
class Professor;


class Professor {
    private:
        int id;
        int num_skills;
        int pref[PREFS];
        int idx;

    public:
        int sch;

        // constructor
        Professor (int _id, int _n) {
            this->id = _id;
            this->num_skills = _n;
            this->idx = -1;
            this->sch = -1; //está livre
        }
        // destructor
        ~Professor (void);

        // setters
        void setId (int _id) {
            this->id = _id;
        }
        void setSkills (int _n) {
            this->num_skills = _n;
        }
        void setPrefs (int * _arr) {
            for (int i = 0; i < PREFS; i++) {
                this->pref[i] = _arr[i];
            }
        }

        //getters
        int getId (void) {
            return this->id;
        }
        int getSkills (void) {
            return this->num_skills;
        }
        int * getPrefs (void) {
            return this->pref;
        }

        // methods
        int propouse(void){
            this->idx++;
            if (this->idx >= 5) {
                throw invalid_argument("Professor.propouse: out of range");
            }
            return this->pref[idx];
        }

        int is_free(){
            return (this->sch == -1);
        }

        int bind(int sch_id){
            if (! (0<sch_id && sch_id < NUM_PROFESSORS) )
                throw invalid_argument("Professor.bind: sch_id out of range");

            return this->sch = sch_id;
        }

        void gets_dumped(){
            if (this->sch != -1)
                throw invalid_argument("Professor.gets_dumped: professor " + to_string(this->id+1) + " not linked");
            this->sch = -1;
        }
};


class School {
    private:
        int id;
        int req_skills;
        int profs[MAX_PROFS];


    public:
        int linked_to[MAX_PROFS];

        // constructor
        School (int _id, int _r) {
            this->id = _id;
            this->req_skills = _r;

            this->linked_to[0] = -1;
            this->linked_to[1] = -1;
        }
        // destructor
        ~School (void);

        // setters
        void setId (int _id) {
            this->id = _id;
        }
        void setSkills (int _r) {
            this->req_skills = _r;
        }

        // getters
        int getId (void) {
            return this->id;
        }
        int getSkills (void) {
            return this->req_skills;
        }

        // methods
        bool is_free(){
            return (this->linked_to[0] == -1 || this->linked_to[1] == -1);
        }

        bool bind(int prof_id){
            for (int i = 0; i<MAX_PROFS; i++) {

                if (this->linked_to[i] == -1) {
                    this->linked_to[i] = prof_id;
                    return true;
                }
                
            }
            return false;
        }

        list<int> get_professors(){
            list<int> p;

            for (int i = 0; i < MAX_PROFS; i++) {
                if (this->linked_to[i] != -1) {
                    p.push_back (this->linked_to[i]);
                }
            }

            return p;
        }

        bool prefers(Professor *a, Professor *b){
            int skill_a, skill_b, idx_a, idx_b;
            int * pref_a, * pref_b;
            skill_a = a->getSkills ();
            skill_b = b->getSkills ();

            if ((skill_a >= this->req_skills && skill_b >= this->req_skills) ||
                (skill_a < this->req_skills && skill_b < this->req_skills)) {
                pref_a = a->getPrefs ();
                pref_b = b->getPrefs ();

                for (int i = 0; i < PREFS; i++) {
                    if (pref_a[i] == this->id)
                        idx_a = i;
                    if (pref_b[i] == this->id)
                        idx_b = i;
                }

                return (idx_a <= idx_b);
            }
            else
                return (skill_a >= this->req_skills);
            return false;

            //return (skill_a >= this->req_skills and skill_b < this->req_skills);
        }

        void kicks(int prof_id){
            for (int i = 0; i < MAX_PROFS; i++){
                if (this->linked_to[i] == prof_id){
                    this->linked_to[i] = -1;
                    return;
                }
            }

            throw invalid_argument("School.kicks: invalid prof_id " + to_string(prof_id));
        }

};


int get_free_professor(Professor ** prof){
    for (int i = 0; i < NUM_PROFESSORS; i++){
        if ( prof[i]->is_free() ){
            return i;
        }
    }
    throw "Non avaiable professors";
    return -1;
}

void matching (Professor ** prof, School ** schl){
    int alone_prof_id; Professor *cur;
    int provoked_school; School *the_school;
    int line = 0;

    while(true){
        printf("line = %d\n", ++line);
        alone_prof_id = get_free_professor(prof);
        if (alone_prof_id == -1){
            break;
        }
        cur = prof[alone_prof_id];
        provoked_school = cur->propouse();
        printf("voked_school = %d\n", provoked_school);
        the_school = schl[provoked_school];
        printf("school_id = %d\n", the_school->getId());

        if (the_school->is_free()){
            cur->bind(provoked_school);
            the_school->bind(alone_prof_id);
            printf("(E%d):[P%d]\n", the_school->getId ()-1, alone_prof_id+1);
        } else {
            list<int> p = the_school->get_professors();
            for (auto enemy : p){
                Professor *the_enemy = prof[enemy];

                if (the_school->prefers(cur, the_enemy)){
                    the_school->kicks(enemy);
                    the_school->bind(alone_prof_id);
                    the_enemy->gets_dumped();
                    cur->bind(provoked_school);
                    printf("(E%d):[P%d]\n", the_school->getId ()-1, alone_prof_id+1);
                }
            }
        }
    }

    for (int i = 0; i < NUM_SCHOOLS; i++) {
        School * school = schl[i];
        printf("E%d : [P%d, P%d]\n", school->getId (), school->linked_to[0], school->linked_to[1]);
    }
}


int main (void) {

    ifstream fp;
    string line;

    Professor *profArray[NUM_PROFESSORS];
    School *schArray[NUM_SCHOOLS];

    //map <Professor, School> match;

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
        profArray[i]->setPrefs (preference);

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

    matching (profArray, schArray);

    
    return 0;
}
