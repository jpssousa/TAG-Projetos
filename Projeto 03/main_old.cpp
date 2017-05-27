#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <string>
#include <map>

#define APP_VERSION 0.21

#define NUM_PROFESSORS 100
#define NUM_SCHOOLS 50
#define MAX_PREFS 5
#define MAX_SKILLS 3
#define FILE_NAME "input.txt"
#define PRF_START 130
#define SCH_START 3719
#define MAX_PROFS 2
#define ERR -1

using namespace std;

/* MINGW "helps us", really... */
#include <sstream>
template <typename T>
  std::string stoi ( T Number )
  {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
  }

template <class T>
class PoolManager{
	private:
		T** pool_array;
		int max_elements;
	public:
		PoolManager(int max_elements){
			if (max_elements < 0){
				throw invalid_argument("PoolManager::PoolManager max_elements must be positive");
			}

			pool_array = (T**)malloc(max_elements * sizeof(void*));
			int idx;
			for (idx=0; idx < max_elements; idx++)
				pool_array[idx] = NULL;

			this->max_elements = max_elements;
		}
		~PoolManager(){
			free(this->pool_array);
		}

		int add_element(T* elem_p){
			if ( elem_p->id < 0 || elem_p->id >= max_elements){
				return -1;
			}
			pool_array[elem_p->id] = elem_p;
			return elem_p->id;
		}

		T* get_fromID(int id){
			if (id < 0 || id >= max_elements){
				return NULL;
			}
			return pool_array[id];
		}

		void get_element_list(list<T*> &l){
			for (int idx=0; idx<max_elements; idx++){
				if (pool_array[idx]){
					l.push_back(pool_array[idx]);
				}
			}
		}

		void free_all(){
			int idx;
			for (idx=0; idx<max_elements; idx++)
				free(pool_array[idx]);
		}
};

class Professor;
class School;

class Professor{
	public:
		int id;
		int num_skills;
		int preferences[MAX_PREFS]; int pref_idx=0;
		School *works_for;

		Professor(int id, int num_skills){
			this->id = id;
			this->num_skills = num_skills;
			this->works_for = NULL;
		};

		void set_prefs(int *the_prefs){
			for (int i=0; i<MAX_PREFS; i++){
				preferences[i] = the_prefs[i]-1;
			}
		}

		static Professor* 	get_free_professor(list<Professor*> &pl);

		bool 	is_free();
		int 	propouse();
		void	gets_job(School *s);
		void	loses_job();
		int 	get_priority_idx(int school_id);
};

Professor* Professor::get_free_professor(list<Professor*> &pl){
	for (auto prof: pl){
		if (prof->is_free()){
			pl.remove(prof);
			pl.push_back(prof);
			return prof;
		}
	}
	return NULL;
}

bool Professor::is_free(){
	return (!works_for);
}

int Professor::propouse(){
	if (pref_idx >= MAX_PREFS){
		throw runtime_error("Professor::propouse " + stoi(id) + " priority list overflow"); // mingw does not support to_string. source: https://stackoverflow.com/a/12975602
	}
	return preferences[pref_idx++];
}

void Professor::gets_job(School *s){
	if (! this->is_free() ){
		cout << "the school is: " << works_for << endl;
		throw runtime_error("Professor::gets_job " + stoi(id) + " has to be unemployed first");
	}
	works_for = s;
}

void Professor::loses_job(){
	if ( this->is_free() ){
		throw runtime_error("Professor::loses_job " + stoi(id) + " does not have a job!");
	}
	works_for = NULL;
}

int Professor::get_priority_idx(int school_id){
	for (int i=0; i<MAX_PREFS; i++){
		if (preferences[i] == school_id){
			return i;
		}
	}
	throw runtime_error("Professor::get_priority_idx " + stoi(id) + " not found school_id");
}


class School {
	public:
		int id;
		int min_skill;
		list<Professor*> linked_to;
		int capacity = 2;

		School(int id, int min_skill){
			if (min_skill < 0 || min_skill > MAX_SKILLS)
				throw invalid_argument("School::School min_skill must be positive");
			this->id = id;
			this->min_skill = min_skill;
		}

		bool 				is_free();
		void 				accepts(Professor *p);
		void				kicks(Professor *p);
		list<Professor*> 	get_team();
		bool				prefers(Professor *a, Professor *b);
};

bool School::is_free(){
	return (linked_to.size() < capacity);
}

void School::accepts(Professor *p){
	if (linked_to.size() > capacity){
		throw runtime_error("School::accepts " + stoi(id) + " full capacity reached");
	}
	linked_to.push_back(p);
	p->gets_job(this);
}

void School::kicks(Professor *p){
	linked_to.remove(p);
	p->loses_job();
}

list<Professor*> School::get_team(){
	return linked_to;
}

bool School::prefers(Professor *a, Professor *b){
	return (a->pref_idx > b->pref_idx);
}


void make_matches(PoolManager<Professor> &pm, PoolManager<School> &sm){

	list<Professor*> pl;
	Professor* unemployed_human;
	School* provoked_school;

	pm.get_element_list(pl);
	//Professor::sort_descending(pl);
	while(true){
		unemployed_human = Professor::get_free_professor(pl);
		if (! unemployed_human){
			break;
		}

		int prop_id = unemployed_human->propouse();
		provoked_school = sm.get_fromID(prop_id);
		if (!provoked_school){
			break;
		}

		if (provoked_school->is_free()){
			provoked_school->accepts(unemployed_human);
		} else {
			list<Professor*> enemy_set = provoked_school->get_team();
			for (auto enemy : enemy_set){
				if ( provoked_school->prefers(unemployed_human, enemy) ){
					provoked_school->kicks(enemy);
					provoked_school->accepts(unemployed_human);
					break;
				}
			}
		}
	}

	list<School*> sl; string line;
	sm.get_element_list(sl);
	for (auto school: sl){
		list<Professor*> p = school->get_team();
		cout << "E" << school->id+1 << endl;
		line = "\t[";
		for (auto professor: p){
			line += "P" + stoi(professor->id+1) + ", ";
		}
		line += "\b\b]";
		cout << line << endl;
	}
}

int main(void){
	ifstream fp;
    string line;

    PoolManager<Professor> pm(NUM_PROFESSORS);
    PoolManager<School> sm(NUM_SCHOOLS);

    int id, skill, preference[MAX_PREFS];
    int read_c;

    Professor *the_professor, *cur;
    School *the_school;

    fp.open(FILE_NAME);

    if (!fp.is_open ()) {
        cout << "Could not open file \"" << FILE_NAME << "\"." << endl;
        exit(1);
    }

    fp.seekg (PRF_START, ios_base::beg);

    for (int i = 0; i < NUM_PROFESSORS; i++) {
        getline (fp, line);
        read_c = sscanf (line.c_str(), "(P%d, %d): (E%d, E%d, E%d, E%d, E%d)",
            &id, &skill, &preference[0], &preference[1], &preference[2], &preference[3], &preference[4]);
        if (read_c != 7){
        	i--; // hotfix for windows (CRLF makes PRF_START being different)
        	continue;
        }
        the_professor = new Professor(id-1, skill);
        the_professor->set_prefs(preference);
        read_c = pm.add_element(the_professor);
    }

    fp.seekg (SCH_START, ios_base::beg);

    for (int i = 0; i < NUM_SCHOOLS; i++) {
        getline (fp, line);
        read_c = sscanf (line.c_str(), "(E%d):(%d)", &id, &skill);
        if (read_c != 2){
        	i--;
        	continue;
        }

        the_school = new School(id-1, skill);
        sm.add_element(the_school);
    }

    fp.close();

    make_matches(pm,sm);

    pm.free_all();
    sm.free_all();

    return 0;
}