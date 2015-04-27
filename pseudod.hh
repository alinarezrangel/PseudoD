/*
**PseudoD 1.5.0
**Creado por Alejandro Linarez Rangel
*/
#ifndef DLL_H
#define DLL_H
	
	#include <iostream>
	#include <fstream>
	#include <cstdlib>
	#include <vector>
	#include <stack>
	#include <string>
	#include <cstring>
	#include <cstdio>
	#include <algorithm>
	#include <cctype>
	#include <sstream>
	
	#include "./includefilelib.hh"
	
	using namespace std;
	
	#include "./includefile.hh"
	
	using namespace PDvar;
	
	PDDatos* PDDATA;
	string C;
	
	extern "C" void PDInicializar(vector<string>& nvar,vector<string>& vvar,vector<string>& npun,vector<int>& vpun,vector< stack<string> >& pil,void (*procesar)(string o,istream& e, void(*)(string,istream&)));
	string PDS(string o);
	extern "C" void PDEjecutar(string o,istream& i);
	extern "C" void PDFinal(void);
	
#endif /* ~~~~DLL_H */
