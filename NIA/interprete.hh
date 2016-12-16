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
	typedef void (*inic_nea)(
		PDvar::PDDatos*,
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

#endif /* __INTERPRETE_PSEUDOD_H__ */

