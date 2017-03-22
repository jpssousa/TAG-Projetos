/*
	That header includes two inline functions that enable UTF8
	printing on the terminal.
	TODO: Linux implementation still missing!
*/
#ifdef _WIN32
	#include <windows.h>

	/*source: http://stackoverflow.com/a/19071749*/
	int enableUTF8support(){
		SetConsoleOutputCP(65001);
		return 1;
	}
#else
	int enableUTF8support(){
		return 0;
	}
#endif