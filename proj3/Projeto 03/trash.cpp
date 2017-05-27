
/*
bool isFreeProf (bool arr) {
    for (auto a : arr) {
        if (a == false)
            return true;
    }
    return false;
}


bool isFreeSchl (bool arr[][MAX_PROFS]) {
    for (auto a : arr) {
        if (a[0] == false || a[1] == false)
            return true;
    }
    return false;
}


int findFreeProfessor (bool arr) {
    for (int i = 0; i , NUM_PROFESSORS; i++) {
        if (arr[i] == false)
            return i;
    }
    return ERR;
}
*/
/*
map <Professor, School> matching (Professor &prof, School &schl) {

    map<School, list<Professor>> sch_pro;

    bool free_Prof[NUM_PROFESSORS];
    bool free_Schl[NUM_SCHOOLS][MAX_PROFS];

    for (auto a : free_Prof) a = false;
    for (auto a : free_Schl) {
        a[0] = false;
        a[1] = false;
    }

    while (isFree (free_Prof) && isFreeSchl (free_Schl)) {
        Professor * prf = prof[findFreeProfessor (free_Prof)];
        int sch_id = prf->propouse();
        School * sch = schl[sch_id];

        if (free_Schl[sch_id] == false || free_Schl[sch_id] == false) {

        }
    }

}
*/