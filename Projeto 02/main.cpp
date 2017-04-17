#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <string>
#include <regex>

using namespace std;

#define IN_FILE			"disciplinas-tag-2017-1.csv"
#define APP_VERSION		0.1
#define APP_NAME		"MatriculaWeb CPM"
#define FATAL_ERR		-1


bool verboseEnabled = true; //and false;
void printVerbose(string str) {
	if (verboseEnabled)
		cout << str << endl;
}


map<string, float> factorMap = {
	make_pair("F", 0.5),
	make_pair("M", 1.0),
	make_pair("D", 1.5)
};

class Disciplina {
	public:
		string	cod;
		float	peso;

		Disciplina(string cod, float peso) {
			this->cod = cod;
			this->peso = peso;
		}
};

class fluxoDisciplina {
	int			numElementos;
	int			**matrizAdjacencia;
	Disciplina	**vertices;

	private:
		int		*elementPool;

	public:
		fluxoDisciplina(int numElementos) {
			if (numElementos < 0)
				throw invalid_argument("numElementos must be positive");

			matrizAdjacencia = (int**)malloc(numElementos * sizeof(int*));
			elementPool = (int*)calloc(pow(numElementos, 2), sizeof(int));
			if (matrizAdjacencia == NULL || elementPool == NULL) {
				free(matrizAdjacencia);
				throw bad_alloc();
			}
			for (int i = 0; i < numElementos; i++) {
				matrizAdjacencia[i] = &elementPool[i * numElementos];
			}

			vertices = (Disciplina**)calloc(numElementos, sizeof(Disciplina*));

			this->numElementos = numElementos;
		}
		~fluxoDisciplina() {
			free(matrizAdjacencia); free(elementPool);

			for (int idx = 0; idx < numElementos; idx++) {
				delete vertices[idx];
			}
		}
		bool		addEdge(int a, int b);
		bool		removeEdge(int a, int b);
		Disciplina	*getElement(int id);
		void		setElement(int id, Disciplina *d);
		int			getElementCount();
		void		clone(fluxoDisciplina &dest);
		bool		hasPreRequisitos(int id);
};

bool fluxoDisciplina::addEdge(int a, int b) {
	if ( !(0 <= a && a < numElementos) || !(0 <= b && b < numElementos) ){
		return false;
	}

	matrizAdjacencia[a][b] = 1;

	return true;
}

bool fluxoDisciplina::removeEdge(int a, int b) {
	if (!(0 <= a && a < numElementos) || !(0 <= b && b < numElementos)) {
		return false;
	}

	matrizAdjacencia[a][b] = 0;

	return true;
}

Disciplina *fluxoDisciplina::getElement(int id) {
	if (! (0 <= id && id < numElementos)) {
		throw invalid_argument(string(__func__) + " id(" + to_string(id) + ") is out of range [0," + to_string(numElementos) + "]");
	}

	return vertices[id];
}

void fluxoDisciplina::setElement(int id, Disciplina *d) {
	if (!(0 <= id && id < numElementos)) {
		throw invalid_argument(string(__func__) + " id(" + to_string(id) + ") is out of range [0," + to_string(numElementos) + "]");
	}

	vertices[id] = d;
}

int fluxoDisciplina::getElementCount() {
	return numElementos;
}

void fluxoDisciplina::clone(fluxoDisciplina &dest) {
	if (this->numElementos > dest.numElementos) {
		throw invalid_argument("destination element does not have enough space to clone source data.");
	}

	/*adjacency matrix copy routine*/
	for (int i = 0; i < this->numElementos; i++) {
		for (int j = 0; j < this->numElementos; j++) {
			dest.matrizAdjacencia[i][j] = this->matrizAdjacencia[i][j];
		}
	}

	/*vertex copy routine*/
	//TODO: this part we should clone also each Disciplina element to avoid the dest have dead pointers if 'this' is deleted
	for (int i = 0; i < this->numElementos; i++) {
		dest.vertices[i] = this->vertices[i];
	}
}

bool fluxoDisciplina::hasPreRequisitos(int id) {
	return false;
}

class disciplinaScheduler {
	int courseID;
	map<string, unsigned> semestreOF;

	public:
		disciplinaScheduler(int courseID) {
			this->courseID = courseID;
		}
	int readSemestreDataFromFile(string fileName);
	bool toLinearSequence(fluxoDisciplina &container, list<Disciplina*> &ordedList);
	private:
		void getSortedVerticesList(fluxoDisciplina &source, list<Disciplina*> &dest);
};

int disciplinaScheduler::readSemestreDataFromFile(string fileName) {
	string		line;
	ifstream	inFile(fileName);
	regex		headerHandler("(\\d+),[ ]*?(\\d+)");
	regex		lineHandler("(\\d+),?");
	smatch		match_results;

	printVerbose("(I)::" + string(__func__) + " Attempting to read input file " + fileName);
	if (!inFile) {
		throw ifstream::failure("Fail to open " + fileName);
	}

	getline(inFile, line); /*read header*/
	int courseID, periodNumber;
	if (!regex_match(line, match_results, headerHandler) || match_results.size() < 3) {
		throw domain_error("Bad header in input file " + fileName);
	}
	courseID = stoi(match_results.str(1));
	periodNumber = stoi(match_results.str(2));
	if (periodNumber < 0)
		throw invalid_argument("periodNumber must be positive");
	if (courseID != this->courseID) {
		throw invalid_argument("file courseID ("+to_string(courseID)+") does not match disciplinaScheduler object one ("+to_string(this->courseID)+").");
	}

	int idx = 0; string disciplinaID;
	while (getline(inFile, line)) {
		idx++;

		while (regex_search(line, match_results, lineHandler)) {
			disciplinaID = match_results.str(1);
			semestreOF[disciplinaID] = idx;

			line = match_results.suffix().str();
		}
	}

	printVerbose("(I)::" + string(__func__) + " Read " + to_string(semestreOF.size()) + " disciplina elements");

	return 0;
}

bool disciplinaScheduler::toLinearSequence(fluxoDisciplina &container, list<Disciplina*> &ordedList) {

	fluxoDisciplina instance(container.getElementCount());
	container.clone(instance);

	list<Disciplina*> s;
	this->getSortedVerticesList(instance, s);

	int p; cin >> p;

	list<Disciplina*>::iterator it;
	printVerbose("(I)::" + string(__func__) + " s result:"); string line("");
	for (it = s.begin(); it != s.end(); it++) {
		line += (*it)->cod + ", ";
	}
	printVerbose(line);



	return true;
}

void disciplinaScheduler::getSortedVerticesList(fluxoDisciplina &source, list<Disciplina*> &dest) {
	int max = source.getElementCount();
	Disciplina *cur;
	list<Disciplina*>::iterator it;


	for (int i = 0; i < max; i++) {
		if (source.hasPreRequisitos(i)) { // TODO: implement check for pre-requisitos
			continue;
		}
		cur = source.getElement(i);

		for (it = dest.begin(); it != dest.end(); it++) {
			if (semestreOF[cur->cod] < semestreOF[(*it)->cod]) {
				dest.insert(it, cur); break;
			}
		}
		if (it == dest.end()) {
			dest.push_back(cur);
		}
	}
}

bool parseDisciplinasFile(ifstream *infile, fluxoDisciplina &container) {
	string		line;
	const char	infoPattern[] = "(\\d{6}),([\\w ]+),(\\d+),([FMD ])(,[\\d, ]*)";
	const char	numberPattern[] = "\\d+";
	regex		infoHandler(infoPattern); /*function called just one time: no problems to create the object here*/
	regex		requisitosHandler(numberPattern);


	match_results<string::const_iterator> match_itr; int idx = 0; int lineNumber = 1;
	smatch::iterator it;

	string cod, requisitosLine; int creditos; float peso;
	printVerbose("(I) Reading elements from file (Codigo (ID) - Peso (P) - Requisitos (R))");
	while (getline(*infile, line)) {
		regex_match(line, match_itr, infoHandler);

		if (match_itr.empty() || match_itr.size() < 6 || match_itr.str(4) == " ") {
			cout << "(I) Ignorando linha " << lineNumber << endl; lineNumber++;
			continue;
		}

		cod = match_itr.str(1);
		creditos = stoi(match_itr.str(3)); /*can raise an invalid_argument execption*/
		requisitosLine = match_itr.str(5);

		peso = (float)creditos * factorMap[match_itr.str(4)];

		container.setElement(idx, new Disciplina(cod, creditos));

		string reqList = "";
		while ( regex_search(requisitosLine, match_itr, requisitosHandler) ) {
			reqList += match_itr.str(0) + "; ";
			if (! container.addEdge(idx, stoi(match_itr.str(0)))) {
				printVerbose("(W) Could not add edge between " + to_string(idx) + " and " + match_itr.str(0) + ".");
			}
			requisitosLine = match_itr.suffix().str();
		}

		printVerbose(cod + " - " + to_string(peso) + " - " + reqList);

		idx++; lineNumber++;
	}

	return true;
}

int main()
{
	cout << APP_NAME << " v" << APP_VERSION << endl;

	ifstream fileHandler(IN_FILE);

	if (!fileHandler) {
		cout << "Nao foi possivel abrir arquivo de entrada " << IN_FILE << ". Abortando operacao." << endl;
		return FATAL_ERR;
	}

	fluxoDisciplina fp(35); /*we already know how many elements we're going work with, but it would be a better if this is told on the header*/
	bool parseSucess = parseDisciplinasFile(&fileHandler, fp);

	disciplinaScheduler dps(1856);
	dps.readSemestreDataFromFile("fluxo-tag-2017-1.csv");
	list<Disciplina*>path;
	dps.toLinearSequence(fp, path);
	

    return 0;
}