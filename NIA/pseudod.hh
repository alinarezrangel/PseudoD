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

#ifndef __PSEUDOD_NIA_PSEUDOD_HH__
#define __PSEUDOD_NIA_PSEUDOD_HH__ 1

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
#include "NEA/PDData.hh"

#include "includefilelib.hh"
#include "includefile.hh"

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
extern "C" void PDEjecutar(PDCadena, std::istream&);
extern "C" void PDFinal(void);

#endif
