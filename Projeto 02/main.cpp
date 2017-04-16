#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <regex>

using namespace std;

#define IN_FILE			"disciplinas-tag-2017-1.csv"
#define APP_VERSION		0.1
#define APP_NAME		"MatriculaWeb CPM"
#define FATAL_ERR		-1


bool debugEnabled = true; //and false;
void printVerbose(string str) {
	if (debugEnabled)
		cout << str << endl;
}


map<string, float> factorMap = {
	make_pair("F", 0.5),
	make_pair("M", 1.0),
	make_pair("D", 1.5)
};

bool parseDisciplinasFile(ifstream *infile) {
	string		line;
	const char	infoPattern[] = "(\\d{6}),([\\w ]+),(\\d+),([FMD ])(,[\\d, ]*)";
	const char	numberPattern[] = "\\d+";
	regex		infoHandler(infoPattern); /*function called just one time: no problems to create the object here*/
	regex		requisitosHandler(numberPattern);


	match_results<string::const_iterator> match_itr; int idx = 1;
	smatch::iterator it;

	string ID, requisitosLine; int creditos; float peso;
	printVerbose("(I) Reading elements from file (Codigo (ID) - Peso (P) - Requisitos (R))");
	while (getline(*infile, line)) {
		regex_match(line, match_itr, infoHandler);

		if (match_itr.empty() || match_itr.size() < 6 || match_itr.str(4) == " ") {
			cout << "(I) Ignorando linha " << idx << endl; idx++;
			continue;
		}

		ID = match_itr.str(1);
		creditos = stoi(match_itr.str(3)); /*can raise an invalid_argument execption*/
		requisitosLine = match_itr.str(5);

		peso = (float)creditos * factorMap[match_itr.str(4)];

		string reqList = "";
		while ( regex_search(requisitosLine, match_itr, requisitosHandler) ) {
			reqList += match_itr.str(0) + "; ";
			requisitosLine = match_itr.suffix().str();
		}

		printVerbose(ID + " - " + to_string(peso) + " - " + reqList);

		idx++;
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

	bool parseSucess = parseDisciplinasFile(&fileHandler);

    return 0;
}