/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*****                                                                  *****
***** Página oficial de PseudoD en:                                    *****
*****     http://pseudod.sourceforge.net/                              *****
*****                                                                  *****
****************************************************************************
Copyright 2016-2018 Alejandro Linarez Rangel

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**************************************************************************/

/*
Define la macro INTERACTIVO a 1 para un interprete en linea de comandos
https://sourceforge.net/projects/pseudod/
*/
#ifndef HDR_PSEUDOD_NIA_INTERPRETE_HH
#define HDR_PSEUDOD_NIA_INTERPRETE_HH 1

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <string>
#include <locale>
#include <cstdio>
#include <algorithm>
#include <utility>
#include <cctype>
#include <sstream>
#include <exception>
#include <dlfcn.h>

#include "NEA/interno/data.hh"
#include "NEA/interno/nmemoic.hh"
#include "NEA/interno/token.hh"
#include "NEA/interno/tokenizer.hh"
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
	typedef void (*ejec_nea)(pseudod::Token, pseudod::Tokenizador&);
	typedef void (*liber_nea)(void);

	extern bool Ejecutar;
	extern PDvar::PDDatos DATOS_INT;
	extern std::vector<int> AMBITO;
	extern inic_nea iniciar_nea;
	extern ejec_nea ejecutar_nea;
	extern liber_nea liberar_nea;
	extern void* coneccion_nea;

	void procesar(
		Tokenizador&,
		PDFuncionNEA
	);
	void procesar(
		Token,
		Tokenizador&,
		PDFuncionNEA
	);

	void iniciar(
		PDCadena nea,
		PDCadena bepd,
		PDCadena main
	);
	int terminar(void);

	void ejecutar(PDCadena);
	void ejecutar(std::istream&);
}

#endif
