/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*** Este es PseudoD version 2.1.0                                      *****
*** Log de este archivo:                                               *****
*** Formato: DD/MM/YYYY: txt                                           *****
*** **** 2/01/2016: Se creo el archivo.                               *****
****************************************************************************
**************************************************************************/

/*
Define la macro INTERACTIVO a 1 para un interprete en linea de comandos
https://sourceforge.net/projects/pseudod/
*/
#ifndef __INTERPRETE_PSEUDOD_H__
#define __INTERPRETE_PSEUDOD_H__

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
#include <exception>
#include <dlfcn.h>

//#include "pseudod.hh"
#include "NEA/PDData.hh"

namespace pseudod
{
	using namespace std;
	using namespace PDvar;
	typedef void (*inic_nea)(vector<string>&,vector<string>&,vector<string>&,vector<int>&,vector<stack<string> >&,void (*procesar)(string o,istream& e, void(*FUNCION)(string,istream&)));
	typedef void (*ejec_nea)(string,istream&);
	typedef void (*liber_nea)(void);
	extern bool Ejecutar;
	extern PDDatos DATOS_INT;
	extern vector<int> AMBITO;
	extern inic_nea iniciar_nea;
	extern ejec_nea ejecutar_nea;
	extern liber_nea liberar_nea;
	extern void* coneccion_nea;
	void procesar(string o,istream& e, void(*FUNCION)(string,istream&));
	string iniciar(string nea,string bepd,string main);
	int terminar(void);
	void ejecutar(string linea);
	void ejecutar(istream& entrada);
}

#endif /* __INTERPRETE_PSEUDOD_H__ */
