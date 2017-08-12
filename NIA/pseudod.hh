/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*****                                                                  *****
***** Página oficial de PseudoD en:                                    *****
*****     http://pseudod.sourceforge.net/                              *****
*****                                                                  *****
****************************************************************************
Copyright 2016 Alejandro Linarez Rangel

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

#ifndef HDR_PSEUDOD_NIA_PSEUDOD_HH
#define HDR_PSEUDOD_NIA_PSEUDOD_HH 1

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

#include "NEA/interno/data.hh"
#include "NEA/interno/nmemoic.hh"
#include "NEA/interno/token.hh"
#include "NEA/interno/tokenizer.hh"
#include "NEA/PDData.hh"

#include "includefilelib.hh"
#include "includefile.hh"

#define PSEUDOD_NEA_SALIR_PC return

extern PDvar::PDDatos* PDDATA;

PDCadena PDS(PDCadena o);

extern "C" void PDInicializar(
	std::vector<PDCadena>&,
	std::vector<PDCadena>&,
	std::vector<PDCadena>&,
	std::vector<int>&,
	std::vector<std::stack<PDCadena>>&,
	PDFuncionNIA
);
extern "C" void PDEjecutar(pseudod::Token, pseudod::Tokenizador&);
extern "C" void PDFinal(void);

#endif
