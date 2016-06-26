/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*** Este es PseudoD version 2.1.0                                      *****
*** Log de este archivo:                                               *****
*** Formato: DD/MM/YYYY: txt                                           *****
*** **** 14/09/2014: Se creo el archivo.                               *****
*** **** 06/03/2015: Se agrego soporte para errores fatales.           *****
*** **** 10/04/2015: Mejoras a si,si_no y mientras.                    *****
*** **** 02/01/2016: Interprete embebido.                              *****
*** **** 22/03/2016: Agregadas opciones -d.                            *****
****************************************************************************
**************************************************************************/

/*
Define la macro INTERACTIVO a 1 para un interprete en linea de comandos
https://sourceforge.net/projects/pseudod/
*/
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
//#include "NEA/PDData.hh"
#include "interprete.hh"
using namespace std;

int main (int argc, char* argv[])
{
	bool interactivo = false;
	string bepd = "",nea = "",op = "",mn = "",err = "",ord ="";
	string prgNom = argv[0];
	if(argc == 1)
	{
		cerr << "Error argumentos insuficientes" << endl;
		cerr << "Pruebe con " << prgNom << " --help para ayuda" << endl;
		return 1;
	}
	op = argv[1];
	if((op == "-h")||(op == "--help"))
	{
		cout << prgNom << "  -  El interprete de PseudoD" << endl;
		cout << "Version 2.1.0" << endl;
		cout << "Opciones:" << endl;
		cout << "   " << prgNom << " (-h|--help): Muestra esta ayuda y termina." << endl;
		cout << "   " << prgNom << " archivo bepd nea: ejecuta archivo con la ruta a BEPD bepd y el NEA nea." << endl;
		cout << "   " << prgNom << " -i bepd nea: ejecuta en modo interactivo." << endl;
		cout << "Deje una opcion en -- o en -d para reemplazarla por su valor por defecto" << endl;
		cout << "Los valores por defecto son:" << endl;
		cout << "*) bepd:    /opt/pseudod/bepd/" << endl;
		cout << "*) nea:     /usr/lib/libpseudod.so" << endl;
		cout << "*) plugins: /opt/pseudod/plugins/" << endl;
		cout << "Creado por Alejandro Linarez Rangel" << endl;
		return 0;
	}
	if(argc < 4)
	{
		cerr << "Error argumentos insuficientes" << endl;
		cerr << "Pruebe con " << prgNom << " --help para ayuda" << endl;
		return 1;
	}
	bepd = argv[2];
	nea = argv[3];
	mn = op;
	if((bepd == "--")||(bepd == "-d"))
	{
		bepd = "/opt/pseudod/bepd/";
	}
	if((nea == "--")||(nea == "-d"))
	{
		nea = "/usr/lib/libpseudod.so";
	}
	if(op == "-i")
	{
		interactivo = true;
		cout << "Interprete en linea de comandos de PseudoD" << endl;
		cout << "PseudoD version 2.1.0 en C++11" << endl;
		cout << "Creado por Alejandro Linarez Rangel" << endl;
		cout << ">>> ";
		mn = "nulo";
	}
	err = pseudod::iniciar(nea,bepd,mn);
	if(err != "Ok")
	{
		cerr << "La creacion del interprete dio un error:" << endl;
		cerr << err << endl;
		cerr << "Abortando..." << endl;
		return 1;
	}
	try
	{
		if(interactivo)
		{
			while((pseudod::Ejecutar)&&(cin))
			{
				/*
				pseudod::DATOS_INT.Ejecutar(ord,cin);
				cout << ">>> ";
				*/
				char l1 = ' ';
				char l2 = ' ';
				string buff = "";
				while((l2 != '\n') || (l1 != '\n'))
				{
					l2 = l1;
					l1 = cin.get();
					if((l1 == '\n') && (l2 != '\n'))
					{
						cout << ">>> " << flush;
					}
					buff += l2;
				}
				buff += l1;
				pseudod::ejecutar(buff);
				cout << endl << ">>> " << flush;
			}
			cout << "Adios!" << endl;
		}
		else
		{
			ifstream in(mn.c_str());
			pseudod::ejecutar(in);
		}
	}
	catch(const PDvar::Error& e)
	{
		cerr << "PseudoD lanzo un error:" << endl;
		cerr << e.Mensaje() << endl;
		cerr << "En " << pseudod::DATOS_INT.ObtenerVariable("__ARCH__") << endl;
	}
	catch(const std::exception& e)
	{
		if((string(e.what()) == "stoi")||(string(e.what()) == "stoll")||(string(e.what()) == "stold"))
		{
			cerr << "Error al convertir numeros" << endl;
		}
		else
		{
			cerr << "Error " << e.what() << endl;
		}
		cerr << "En " << pseudod::DATOS_INT.ObtenerVariable("__ARCH__") << endl;
	}
	catch(string e)
	{
		cerr << "PseudoD lanzo un error fatal:" << endl;
		cerr << e << endl;
		cerr << "En " << pseudod::DATOS_INT.ObtenerVariable("__ARCH__") << endl;
	}
	catch(...)
	{
		cerr << "En " << pseudod::DATOS_INT.ObtenerVariable("__ARCH__") << endl;
		cerr << "Error no identificado!" << endl;
	}
	return pseudod::terminar();
}
