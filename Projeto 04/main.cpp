#include <bits/stdc++.h>

using namespace std;


#define FILE_NAME "dirty_deputies_v3.csv"
#define CSV_DELIMITER ';'
#define DELIMITER_COUNT 7


class Deputy {
public:
    string name;
    string state;
    string party;

    Deputy () {};

    ~Deputy () {};
};


class Company {
public:
    string id;
    string name;

    Company () {};

    ~Company () {};
};


class Refund {
public:
    string date;
    string description;
    double value;

    Refund () {};

    ~Refund () {};
};


void readLine (Deputy ** dep, Company ** comp, Refund ** ref, string line) {
    int del_count, pos = 0;
    vector <string> foo;

    for (del_count = 0; del_count < DELIMITER_COUNT; del_count++) {
        pos = line.find (CSV_DELIMITER);
        foo.push_back (line.substr (0, pos));
        line.erase (0, pos + 1);
    }

    (*dep)->name        = foo[0];
    (*dep)->state       = foo[1];
    (*dep)->party       = foo[2];
    (*ref)->description = foo[3];
    (*comp)->name       = foo[4];
    (*comp)->id         = foo[5];
    (*ref)->date        = foo[6];
    (*ref)->value = atof (line.c_str ());
}


int main (void) {
    
    ifstream fp;
    string line;

    list <Deputy *>  deputy_list;
    list <Company *> company_list;
    list <Refund *>  refund_list;

    fp.open (FILE_NAME);

    for (int i = 0; i < 3; i++) {
        Deputy  *dep  = new Deputy ();
        Company *comp = new Company ();
        Refund  *ref  = new Refund ();

        getline (fp, line);
        readLine (&dep, &comp, &ref, line);

        deputy_list.push_back  (dep);
        company_list.push_back (comp);
        refund_list.push_back  (ref);

        cout << line << endl << endl;

        cout << "Name: " << dep->name << endl;
        cout << "State: " << dep->state << endl;
        cout << "Party: " << dep->party << endl;
        cout << "Id: " << comp->id << endl;
        cout << "Name: " << comp->name << endl;
        cout << "Desc: " << ref->description << endl;
        cout << "Date: " << ref->date << endl;
        cout << "Value: " << ref->value << endl;
        cout << endl <<endl;
    }

    for (auto i : deputy_list) {
        i->~Deputy ();
    }

    for (auto i : company_list) {
        i->~Company ();
    }

    for (auto i : refund_list) {
        i->~Refund ();
    }

    fp.close ();

    return 0;
}