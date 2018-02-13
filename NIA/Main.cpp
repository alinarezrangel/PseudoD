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

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <string>
#include <locale>
#include <cstdio>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <exception>
#include <dlfcn.h>

//#include "pseudod.hh"
//#include "NEA/PDData.hh"

#include "NEA/interno/data.hh"
#include "NEA/interno/token.hh"
#include "NEA/interno/tokenizer.hh"

#include "interprete.hh"

using namespace std;

int main (int argc, char* argv[])
{
	bool interactivo = false;
	string bepd = "", nea = "", op = "", mn = "", err = "", ord ="";
	string prgNom = argv[0];

	if(argc == 1)
	{
		cerr << "Error argumentos insuficientes" << endl;
		cerr << "Pruebe con " << prgNom << " --help para ayuda" << endl;
		return 1;
	}

	op = argv[1];

	if((op == "-h") || (op == "--help"))
	{
		cout << prgNom << "  -  El interprete de PseudoD" << endl;
		cout << "Version " << PSEUDOD_VERSION << endl;
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

	if((op == "-v") || (op == "--version"))
	{
		cout << "PseudoD " << PSEUDOD_VERSION << endl;
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

	if((bepd == "--") || (bepd == "-d"))
	{
		bepd = "/opt/pseudod/bepd/";
	}

	if((nea == "--") || (nea == "-d"))
	{
		nea = "/usr/lib/libpseudod.so";
	}

	if(op == "-i")
	{
		interactivo = true;
		cout << "Interprete en linea de comandos de PseudoD" << endl;
		cout << "PseudoD version " << PSEUDOD_VERSION << " en C++11" << endl;
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
			while((pseudod::Ejecutar) && (cin))
			{
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

				try
				{
					istringstream in(buff);

					in.imbue(std::locale());

					pseudod::ejecutar(in);
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

				cout << endl << ">>> " << flush;
			}

			cout << "Adios!" << endl;
		}
		else
		{
			ifstream in(mn.c_str());

			in.imbue(std::locale());

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
			cerr << "Error al convertir numeros: " << e.what() << endl;
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
