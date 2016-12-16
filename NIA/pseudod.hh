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

#ifndef __PSEUDOD_NEA_CORE_H__
#define __PSEUDOD_NEA_CORE_H__

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

#include "NEA/PDData.hh"

#include "includefilelib.hh"
#include "includefile.hh"

extern PDvar::PDDatos* PDDATA;

PDCadena PDS(PDCadena o);

extern "C" void PDInicializar(
	PDvar::PDDatos*,
	PDFuncionNIA
);
extern "C" void PDEjecutar(PDCadena, std::istream&);
extern "C" void PDFinal(void);

#endif /* ~~~~__PSEUDOD_NEA_CORE_H__ */
