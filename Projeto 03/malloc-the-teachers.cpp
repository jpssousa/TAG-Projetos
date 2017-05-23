#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>


#define NUM_PROFESSORS 100
#define NUM_SCHOOLS 50
#define PREFS 5
#define FILE_NAME "input.txt"
#define PRF_START 130
#define SCH_START 3719

using namespace std;


class School;
class Professor;


class Professor {
    private:
        int id;
        int num_skills;
        int pref[PREFS];

    public:
        // constructor
        Professor (int _id, int _n) {
            this->id = _id;
            this->num_skills = _n;
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
            for (int i =0; i < PREFS; i++) {
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
};


class School {
    private:
        int id;
        int req_skills;

    public:
        // constructor
        School (int _id, int _r) {
            this->id = _id;
            this->req_skills = _r;
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
};


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
        (*profArray)->setPrefs (preference);

        printf("(P%d, %d): (E%d, E%d, E%d, E%d, E%d)\n",
            id, skill, preference[0], preference[1], preference[2], preference[3], preference[4]);
    }

    fp.seekg (SCH_START, ios_base::beg);

    for (int i = 0; i < NUM_SCHOOLS; i++) {
        getline (fp, line);
        sscanf (line.c_str (), "(E%d):(%d)", &id, &skill);

        schArray[i] = new School (id, skill);
        printf("(E%d):(%d)\n", id, skill);
    }

    
    return 0;
}
