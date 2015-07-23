/*
**PseudoD 1.9.5
**Creado por Alejandro Linarez Rangel
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

#include "pseudod.hh"
#include "NEA/PDData.hh"
using namespace std;
using namespace PDvar;

template<class T>
int buscar(vector<T> a,T b)
{
	for(int i = 0;i < a.size();i++)
	{
		if(a[i] == b)
		{
			return i;
		}
	}
	return -1;
}

/*,vector<string>,vector<string>,vector<string>,vector<int>,vector<stack<string> >
,nombres,valores,punteros,valor,pilas
*/
bool Ejecutar = true;

PDDatos DATOS_INT;

void procesar(string o,istream& e, void(*FUNCION)(string,istream&))
{
	static string espacio = "";
	static int indicepi = 0;
	static string archivo;
	static vector<int> AMBITO(2);
	if(!Ejecutar)
		return;
	if(o == "adquirir")
	{
		string a;
		e >> a;
		bool existe = false;
		for (int i = 0; i < (*DATOS_INT.nombrev).size(); i += 1)
		{
			if((*DATOS_INT.nombrev)[i] == a)
				existe = true;
		}
		if(existe)
		{
			throw string("¡SISTEMA CORRUPTOOOOO! en adquirir "+a);
		}
			DATOS_INT.CrearVariable(a,"Variable",0,a);
			DATOS_INT.CrearVariable(a+string("#Tipo"),"Variable",0,"PseudoVariable");
			DATOS_INT.CrearVariable(a+string("#NOMBRE"),"Variable",0,a);
	}
	else if(o == "instancia") // alias de Importar.Tipos.Instancia
	{
		(*FUNCION)("Importar.Tipos.Instancia",e);
	}
	else if((o == "estructura")||(o == "clase"))
	{
		(*FUNCION)("Importar.Tipos.Estructura",e);
	}
	else if((o == "heredar"))
	{
		(*FUNCION)("Importar.Tipos.Heredar",e);
	}
	else if((o == "contiene"))
	{
		(*FUNCION)("Importar.Tipos.EstrucEstruc",e);
	}
	else if((o == "redireccionar"))
	{
		(*FUNCION)("Importar.Tipos.Redireccionar",e);
	}
	else if((o == "mientras"))
	{
		(*FUNCION)("Importar.PseudoD.Mientras",e);
	}
	else if(o == "liberar")
	{
		(*FUNCION)("Importar.Tipos.Borrar",e);
	}
	else if(o == "puntero")
	{
		string var,val;
		e >> var >> val;
		bool existe = false;
		for (int i = 0; i < (*DATOS_INT.nombrep).size(); i += 1)
		{
			if((*DATOS_INT.nombrep)[i] == var)
				existe = true;
		}
		if(existe)
		{
			throw string("¡SISTEMA CORRUPTOOOOO! en puntero "+var);
		}
		DATOS_INT.CrearVariable(var,"Puntero",DATOS_INT.BuscarIndice("Variable",val));
	}
	else if(o == "usar_espacio")
	{
		string h;
		e >> h;
		espacio = h;
	}
	else if((o == "incrementar_p")||(o == "incrementar_puntero"))
	{
		string h;
		e >> h;
		DATOS_INT.ObtenerIndicePuntero(h)++;
		if(DATOS_INT.ObtenerIndicePuntero(h) >= (*DATOS_INT.nombrev).size())
		{
			throw string("Error incrementando el puntero "+h+": acceso denegado a memoria prohibida");
		}
	}
	else if((o == "decrementar_p")||(o == "decrementar_puntero"))
	{
		string h;
		e >> h;
		DATOS_INT.ObtenerIndicePuntero(h)--;
		if(DATOS_INT.ObtenerIndicePuntero(h) < 0)
		{
			throw string("Error decrementando el puntero "+h+": acceso denegado a memoria prohibida");
		}
	}
	else if(o == "escribir")
	{
		string h;
		e >> h;
		cout << DATOS_INT.ObtenerVariable(h);
		#ifdef INTERACTIVO
			cout << endl;
		#endif
	}
	else if(o == "ejecutar")
	{
		string h;
		e >> h;
		istringstream entrada(DATOS_INT.ObtenerVariable(h));
		while(entrada >> h)
			procesar(h,entrada, FUNCION);
	}
	else if(o == "nl")
	{
		cout << endl;
	}
	else if(((o == "oper")||(o == "operador"))||(o == "fijar"))
	{
		string variable1,oper,h;
		e  >> variable1 >> oper >> h;
		string& a = DATOS_INT.ObtenerVariable(variable1);
		if(oper == "=*") // fijar A =* hola
		{
			string B;
			getline(e,B,'\n');
			h += B;
			a = h;
		}
		else if((oper == "=?")||(oper == "=¿?")) // fijar A =? B
		{
			a = DATOS_INT.ObtenerVariable(h);
		}
		else
		{
			throw string("No se detecta el operador "+oper);
		}
	}
	else if(o == "leer")
	{
		string variable1;
		e >> variable1;
		string& a = DATOS_INT.ObtenerVariable(variable1);
		cin >> a;
	}
	else if(o == "utilizar")
	{
		string func;
		e >> func;
		string funcion = func;
		ifstream en(funcion.c_str());
		if(!en)
		{
			// el archivo no esta en una ruta actual, ejecutalo desde BEPD
			funcion = DATOS_INT.ObtenerVariable("__LIB__") + funcion;
			en.close();
			en.open(funcion.c_str());
			if(!en)
			{
				cerr << "ERROR EN \'utilizar " << func << "\' se traduce a \'utilizar " << funcion << "\' no existe el archivo " << funcion << endl;
				throw string("Se intento importar un archivo inexistente");
			}
		}
		
		string a = DATOS_INT.ObtenerVariable("__ARCH__");
		DATOS_INT.ObtenerVariable("__ARCH__") = funcion;
		string h;
		while(en >> h)
		{
			procesar(h,en, FUNCION);
		}
		DATOS_INT.ObtenerVariable("__ARCH__") = a;
	}
	else if(o == "llamar")
	{
		string var;
		e >> var;
		string a = DATOS_INT.ObtenerVariable(var);
		//*
		string bpalab = "";
		e >> bpalab;
		vector<string> param;
		while(bpalab != "#(Final).")
		{
			param.push_back(bpalab);
			if(!(e >> bpalab))
			{
				throw string("Error en llamar "+var+" : no se encontro #(Final). pero si un EOF");
			}
		}
		string nombre_var, tipo_var;
		int numeral = var.rfind("#");
		//clog << numeral << endl;
		if(numeral == string::npos)
			numeral = var.size();
		nombre_var = var.substr(0,numeral);
		tipo_var = DATOS_INT.ObtenerVariable(nombre_var+string("#Tipo"));
		//clog << tipo_var << endl;
		for(int i = (param.size()-1);i >= 0;i--)
		{
			string& a = DATOS_INT.ObtenerVariable(param[i]);
			(*DATOS_INT.pilas)[indicepi].push(a);
		}
		if(tipo_var != "PseudoFuncion")
		{
			DATOS_INT.Empujar(nombre_var,cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL")));
			//clog << nombre_var << endl;
		}
		//*/
		istringstream st(a);
		string h;
		while(st >> h)
		{
			procesar(h,st,FUNCION);
		}
	}
	else if(o == "funcion")
	{
		vector<string> lineas;
		string nom;
		e >> nom;
		string lin = "";
		while(lin != "finfun")
		{
			getline(e,lin,'\n');
			lineas.push_back(lin);
			lin.erase(std::remove_if(lin.begin(), 
                              lin.end(),
                              [](char x){return std::isspace(x);}),
                lin.end());
		}
		string func;
		for(int i = 0;i < lineas.size();i++)
		{
			func += lineas[i] + "\n";
		}
		DATOS_INT.CrearVariable(nom,"Variable",0,func);
		DATOS_INT.CrearVariable(nom+string("#NOMBRE"),"Variable",0,nom);
		DATOS_INT.CrearVariable(nom+string("#Tipo"),"Variable",0,"PseudoFuncion");
		DATOS_INT.CrearVariable(nom+string("#cod"),"Variable",0,func);
	}
	else if(o == "finfun")
	{
		// Nada
	}
	else if(o == "sistema")
	{
		string h;
		getline(e,h,'\n');
		system(h.c_str());
	}
	else if(o.front() == '[')
	{
		string h;
		e >> h;
		while(!(h.back()==']'))
		{
			e >> h;
		}
	}
	else if((o == "empujar")||(o == "enviar_parametro")||(o == "devolver"))
	{
		string variable1;
		e >> variable1;
		string& a = DATOS_INT.ObtenerVariable(variable1);
		DATOS_INT.Empujar(a,cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL")));
	}
	else if((o == "sacar")||(o == "recibir_resultado")||(o == "recibir_parametro"))
	{
		string variable1;
		e >> variable1;
		string& a = DATOS_INT.ObtenerVariable(variable1);
		a = DATOS_INT.Sacar(cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL")));
	}
	else if(o == "usar_pila")
	{
		e >> indicepi;
			DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL") = eas(indicepi);
	}
	else if(o == "crear_pila")
	{
		DATOS_INT.CrearPila();
		DATOS_INT.ObtenerVariable("VG_NUMERO_PILAS") = eas((*DATOS_INT.pilas).size());
	}
	else if(o == "si")
	{
		string variable1,val;
		e >> variable1;
		if(variable1 == "llamar")
		{
			procesar("llamar",e,FUNCION);
			val = DATOS_INT.Sacar(cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL")));
		}
		else if(variable1 == "¿son_iguales?")
		{
			string var1,var2;
			e >> var1 >> var2;
			val = ((DATOS_INT.ObtenerVariable(var1) == DATOS_INT.ObtenerVariable(var2))? DATOS_INT.VERDADERO : DATOS_INT.FALSO);
		}
		else if(variable1 == "ejecutar")
		{
			string var1,var2,ord,es,valor;
			e >> ord >> var1 >> var2 >> es >> valor;
			if(es != "es")
				throw string("Error en si ejecutar "+ord+": Su sintaxis es: variable variable \"es\" resultado: no se escribio es, se escribio:" + es);
			istringstream in(var1 + " " + var2 + " ___codigo_pseudod_buffer_interno___");
			procesar(ord,in,FUNCION);
			val = ((DATOS_INT.ObtenerVariable("___codigo_pseudod_buffer_interno___") == DATOS_INT.ObtenerVariable(valor))? DATOS_INT.VERDADERO : DATOS_INT.FALSO);
		}
		else if(variable1 == "comparar")
		{
			string var1,op,var2,ord;
			e >> ord >> var1 >> op >> var2;
			istringstream in(var1 + " " + op + " " + var2 + " ___codigo_pseudod_buffer_interno___");
			procesar(ord,in,FUNCION);
			val = DATOS_INT.ObtenerVariable("___codigo_pseudod_buffer_interno___");
		}
		else
			val = DATOS_INT.ObtenerVariable(variable1);
		string cond = ((val == DATOS_INT.VERDADERO)? "si" : "no");
		string ord;
		int i = 0;
		i = AMBITO.size();
		AMBITO.push_back(0);
		while(AMBITO.size() != i)
		{
			e >> ord;
			if(ord == "=*")
			{
				getline(e,ord,'\n');
				e >> ord;
			}
			if(cond == "si")
			{
				procesar(ord, e, FUNCION);
			}
			else if(ord == "fin")
			{
				AMBITO.pop_back();
			}
			else if((ord == "si")||(ord == "si_no"))
			{
				AMBITO.push_back(0);
			}
		}
	}
	else if(o == "si_no")
	{
		string variable1,val;
		e >> variable1;
		if(variable1 == "llamar")
		{
			procesar("llamar",e,FUNCION);
			val = DATOS_INT.Sacar(cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL")));
		}
		else if(variable1 == "¿son_iguales?")
		{
			string var1,var2;
			e >> var1 >> var2;
			val = ((DATOS_INT.ObtenerVariable(var1) == DATOS_INT.ObtenerVariable(var2))? DATOS_INT.VERDADERO : DATOS_INT.FALSO);
		}
		else if(variable1 == "ejecutar")
		{
			string var1,var2,ord,es,valor;
			e >> ord >> var1 >> var2 >> es >> valor;
			if(es != "es")
				throw string("Error en si ejecutar "+ord+": Su sintaxis es: variable variable \"es\" resultado: no se escribio es, se escribio:" + es);
			istringstream in(var1 + " " + var2 + " ___codigo_pseudod_buffer_interno___");
			procesar(ord,in,FUNCION);
			val = ((DATOS_INT.ObtenerVariable("___codigo_pseudod_buffer_interno___") == DATOS_INT.ObtenerVariable(valor))? DATOS_INT.VERDADERO : DATOS_INT.FALSO);
		}
		else if(variable1 == "comparar")
		{
			string var1,op,var2,ord;
			e >> ord >> var1 >> op >> var2;
			istringstream in(var1 + " " + op + " " + var2 + " ___codigo_pseudod_buffer_interno___");
			procesar(ord,in,FUNCION);
			val = DATOS_INT.ObtenerVariable("___codigo_pseudod_buffer_interno___");
		}
		else
			val = DATOS_INT.ObtenerVariable(variable1);
		string cond = ((val == DATOS_INT.FALSO)? "si" : "no");
		string ord;
		int i = 0;
		i = AMBITO.size();
		AMBITO.push_back(0);
		while(AMBITO.size() != i)
		{
			e >> ord;
			if(ord == "=*")
			{
				getline(e,ord,'\n');
				e >> ord;
			}
			if(cond == "si")
			{
				procesar(ord, e, FUNCION);
			}
			else if(ord == "fin")
			{
				AMBITO.pop_back();
			}
			else if((ord == "si")||(ord == "si_no"))
			{
				AMBITO.push_back(0);
			}
		}
	}
	else if(o == "fin")
	{
		AMBITO.pop_back();
	}
	else if((o == "comparar_i")||(o == "¿son_iguales?")) // var1 var2 res
	{
		string var1, var2;
		e >> var1 >> var2;
		string variable1;
		e >> variable1;
		string& a = DATOS_INT.ObtenerVariable(variable1);
		string& b = DATOS_INT.ObtenerVariable(var1);
		string& c = DATOS_INT.ObtenerVariable(var2);
		a = DATOS_INT.FALSO;
		if(b == c)
		{
			a = DATOS_INT.VERDADERO;
		}
	}
	else if(o == "escribir_esp")
	{
		cout << " ";
	}
	else if((o == "sal")||(o == "salir"))
	{
		Ejecutar=false;
	}
	else
	{
			(*FUNCION)(o,e);
	}
}

/*

	}
	else if(o == "")
	{
		(void (*procesar)(string o,ifstream& e, void(*FUNCION)(string,ifstream&)))
*/

int main(int argc,char* argv[])
{
	if(!(argc >= 2))
	{
		cout << "Insuficientes parametros." << endl << "Uso:" << argv[0] << " (archivo)[.pd|.pseudo] (ruta de bepd) [ruta de libpseudod.so]" << endl;
		return 1;
	}
	if((string(argv[1]) == "--help")||(string(argv[1]) == "-h"))
	{
		cout << "Interprete de PseudoD, version 1.9.5(u1.9.5)" << endl;
		cout << "Uso: " << argv[0] << " (archivo a ejecutar) (ruta de bepd) [ruta de libpseudod.so]" << endl;
		cout << "PseudoD no tiene opciones excepto -h o --help" << endl;
		cout << "PseudoD fue creado por Alejandro Linarez Rangel" << endl;
		cout << endl;
		cout << "Pagina del proyecto: <https://sourceforge.net/projects/pseudod/>" << endl;
		cout << "Valores por defecto:" << endl;
		cout << "\truta de libpseudod.so: /usr/lib/libpseudod.so o"
						"/Library/libpseudod.dylib" << endl;
		cout << "\truta de bepd: /usr/lib/pseudod/1.9.3/" << endl;
		return 0;
	}
#ifdef MACOSX
	string libpseudod = "/Library/libpseudod.dylib";
#else
	string libpseudod = "/usr/lib/libpseudod.so";
#endif
	if(argc >= 4)
	{
		libpseudod = argv[3];
	}
	void* dll = dlopen(libpseudod.c_str(), RTLD_LAZY);
	if(dll == NULL)
	{
		cout << "Error al cargar libpseudod.so:" << dlerror() << endl;
		return 1;
	}
	void(*funcion)(vector<string>&,vector<string>&,vector<string>&,vector<int>&,vector<stack<string> >&,void (*procesar)(string o,istream& e, void(*FUNCION)(string,istream&)));
	void(*func2)(string,istream&);
	void(*func3)(void);
	funcion = (void (*)(vector<string>&,vector<string>&,vector<string>&,vector<int>&,vector<stack<string> >&,void (*procesar)(string o,istream& e, void(*FUNCION)(string,istream&))))dlsym(dll,"PDInicializar");
	func2 = (void(*)(string,istream&))dlsym(dll,"PDEjecutar");
	func3 = (void(*)(void))dlsym(dll,"PDFinal");
	if((funcion == NULL)||(func2 == NULL)||(func3==NULL))
	{
		cout << "Error al usar libpseudod.so:" << dlerror() << endl;
		return 1;
	}
	(*funcion)((*DATOS_INT.nombrev),(*DATOS_INT.valorv),(*DATOS_INT.nombrep),(*DATOS_INT.nvapunt),(*DATOS_INT.pilas),procesar);
	DATOS_INT.CrearVariable("__MAIN__","Variable",0,argv[1]);
	DATOS_INT.CrearVariable("__LIB__","Variable",0,((argc >= 3)? argv[2] : "/usr/lib/pseudod/1.9.3/"));
	DATOS_INT.CrearVariable("__ARCH__","Variable",0,argv[1]);
	DATOS_INT.CrearVariable("VG_PILA_ACTUAL","Variable",0,"0");
	DATOS_INT.CrearVariable("VG_NUMERO_PILAS","Variable",0,"0");
	string a(argv[1]);
	{
		if(DATOS_INT.ObtenerVariable("__LIB__") != "")
		{
			try
			{
				istringstream en("builtins.pseudo");
				procesar("utilizar", en, func2);
			}
			catch(const std::exception& e)
			{
				if((string(e.what()) == "stoi")||(string(e.what()) == "stoll"))
				{
					cerr << "Error al convertir numeros" << endl;
				}
				else
				{
					cerr << e.what() << endl;
				}
				cerr << "EN " << DATOS_INT.ObtenerVariable("__ARCH__") << endl;
				cerr << "Error cargando builtins.pseudo, ¡alto!" << endl;
				cerr << "Error cargando el archivo builtins.pseudo en "
						 << DATOS_INT.ObtenerVariable("__LIB__") << endl;
				(*func3)();
				dlclose(dll);
				return 1;
			}
			catch(const string& e)
			{
				cerr << "PseudoD lanzo un error fatal: " << e << endl;
				cerr << "EN " << DATOS_INT.ObtenerVariable("__ARCH__") << endl;
				cerr << "Error cargando el archivo builtins.pseudo en "
						 << DATOS_INT.ObtenerVariable("__LIB__") << endl;
				(*func3)();
				dlclose(dll);
				return 1;
			}
			catch(...)
			{
				cerr << "Error no identificado, ¡builtins.pseudo no aprovado!" << endl;
				cerr << "Verifique la version de BEPD, y verifique la fuente de descarga."
							<< endl;
				cerr << "Error cargando el archivo builtins.pseudo en "
						 << DATOS_INT.ObtenerVariable("__LIB__") << endl;
				(*func3)();
				dlclose(dll);
				return 1;
			}
		}
		else
		{
			cerr << "Error cargando el archivo builtins.pseudo en "
					 << DATOS_INT.ObtenerVariable("__LIB__") << endl;
			(*func3)();
			dlclose(dll);
			return 1;
		}
	}
#ifndef INTERACTIVO
	ifstream en(a.c_str());
	if(!en)
	{
		cerr << "Error interno: no se encontro el archivo " << a << endl;
		(*func3)();
		return dlclose(dll);
	}
	string base;
	try
	{
		while((en >> base)&&(Ejecutar))
		{
			procesar(base, en, func2);
		}
	}
#else
	cout << "Interprete en linea de comandos de PseudoD" << endl;
	cout << "Creado por Alejandro Linarez Rangel" << endl;
	cout << "PseudoD version u1.9.5 en C++11" << endl;
	cout << ">>> ";
	string base;
	try
	{
		while((cin >> base)&&(Ejecutar))
		{
			procesar(base, cin, func2);
			cout <<  ">>> ";
		}
	}
#endif
	catch(const exception& e)
	{
		if((string(e.what()) == "stoi")||(string(e.what()) == "stoll"))
		{
			cerr << "Error al convertir numeros" << endl;
		}
		else
		{
			cerr << e.what() << endl;
		}
		cerr << "EN " << DATOS_INT.ObtenerVariable("__ARCH__") << endl;
	}
	catch(const string& e)
	{
		cerr << "PseudoD lanzo un error fatal: " << e << endl;
		cerr << "EN " << DATOS_INT.ObtenerVariable("__ARCH__") << endl;
	}
	catch(...)
	{
		cout << "¡Error no identificado!" << endl;
		cout << "Puede ser un error en plugin o un ¡intento de acceso no permitido!"
				 << endl;
		cout << "Siga los siguientes pasos:" << endl;
		cout << "1). Verifique la documentacion de los agregados del NEA" << endl;
		cout << "2). Verifique la documentacion de los agregados dinamicos" << endl;
		cout << "3). Verifique la documentacion de las bibiotecas usadas," << endl;
		cout << "    incluso si estan escritas en PseudoD." << endl;
		cout << "Este error solo puede ser causado a nivel C++11,(codigo casi maquina)." << endl;
		cout << "Si o es ninguno de los pasos de arriba, contactese por los foros de" << endl;
		cout << "\"discussion\" en <https://www.sourceforge.net/projects/pseudod/>" << endl;
		cout << "Disculpe las molestias." << endl;
		
	}
	(*func3)();
	return dlclose(dll);
}
