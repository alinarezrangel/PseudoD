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

#include "nmemoic.hh"
#include "NEA/PDData.hh"

namespace pseudod
{
	//using namespace std;
	//using namespace PDvar;

	typedef void (*inic_nea)(
		std::vector<std::string>&,
		std::vector<std::string>&,
		std::vector<std::string>&,
		std::vector<int>&,
		std::vector< std::stack<std::string> >&,
		void (*procesar)(
			std::string o,
			std::istream& e,
			void (*FUNCION)(
				std::string,
				std::istream&
			)
		)
	);
	typedef void (*ejec_nea)(std::string, std::istream&);
	typedef void (*liber_nea)(void);

	extern bool Ejecutar;
	extern PDvar::PDDatos DATOS_INT;
	extern std::vector<int> AMBITO;
	extern inic_nea iniciar_nea;
	extern ejec_nea ejecutar_nea;
	extern liber_nea liberar_nea;
	extern void* coneccion_nea;

	void procesar(
		std::string o,
		std::istream& e,
		void (*FUNCION)(
			std::string,
			std::istream&
		)
	);
	std::string iniciar(
		std::string nea,
		std::string bepd,
		std::string main
	);
	int terminar(void);
	void ejecutar(std::string linea);
	void ejecutar(std::istream& entrada);
}

#endif /* __INTERPRETE_PSEUDOD_H__ */

