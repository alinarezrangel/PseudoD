/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*****                                                                  *****
***** Página oficial de PseudoD en:                                    *****
*****     http://pseudod.sourceforge.net/                              *****
*****                                                                  *****
****************************************************************************
**************************************************************************/

/*
Define la macro INTERACTIVO a 1 para un interprete en linea de comandos
https://sourceforge.net/projects/pseudod/
*/
#ifndef __PSEUDOD_NIA_INTERPRETE_HH__
#define __PSEUDOD_NIA_INTERPRETE_HH__ 1

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
	typedef void (*inic_nea)(
		std::vector<PDCadena>&,
		std::vector<PDCadena>&,
		std::vector<PDCadena>&,
		std::vector<int>&,
		std::vector<std::stack<PDCadena>>&,
		PDFuncionNIA
	);
	typedef void (*ejec_nea)(PDCadena, std::istream&);
	typedef void (*liber_nea)(void);

	extern bool Ejecutar;
	extern PDvar::PDDatos DATOS_INT;
	extern std::vector<int> AMBITO;
	extern inic_nea iniciar_nea;
	extern ejec_nea ejecutar_nea;
	extern liber_nea liberar_nea;
	extern void* coneccion_nea;

	void procesar(
		PDCadena,
		std::istream&,
		PDFuncionNEA
	);
	PDCadena iniciar(
		PDCadena,
		PDCadena,
		PDCadena
	);
	int terminar(void);
	void ejecutar(PDCadena);
	void ejecutar(std::istream&);
}

#endif
