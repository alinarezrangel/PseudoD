/*
**PseudoD 1.5.0
**Creado por Alejandro Linarez Rangel
Define la macro INTERACTIVO a 1 para un interprete en linea de comandos
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

vector<string> nombres;
vector<string> valores;

vector<string> punteros;
vector<int> valor;

vector< stack<string> > pilas;
bool Ejecutar = true;

PDDatos DATOS_INT(nombres,valores,punteros,valor,pilas);

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
			nombres.push_back(a);
			valores.push_back(string("nulo"));
			nombres.push_back(a+string("#NOMBRE."));
			valores.push_back(a);
			nombres.push_back(a+string("#Tipo."));
			valores.push_back(string("PseudoVariable"));
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
		(*FUNCION)("Importar.Tipos.Mientras",e);
	}
	else if(o == "liberar")
	{
		(*FUNCION)("Importar.Tipos.Borrar",e);
	}
	else if(o == "puntero")
	{
		string var,val;
		e >> var >> val;
			punteros.push_back(var);
			valor.push_back(buscar(nombres,val));
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
		int i = buscar(punteros,h);
		valor[i]++;
		if(valor[i] >= nombres.size())
		{
			valor[i]--;
		}
	}
	else if((o == "decrementar_p")||(o == "decrementar_puntero"))
	{
		string h;
		e >> h;
		int i = buscar(punteros,h);
		valor[i]--;
		valor[i]++;
		if(valor[i] < 0)
		{
			valor[i]++;
		}
	}
	else if(o == "escribir")
	{
		string h;
		e >> h;
		cout << DATOS_INT.ObtenerVariable(h);
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
	else if((o == "oper")||(o == "operador"))
	{
		string variable1,oper,h;
		e  >> variable1 >> oper >> h;
		string& a = DATOS_INT.ObtenerVariable(variable1);
		if(oper == "=*") // oper A =* LIT
		{
			string B;
			getline(e,B,'\n');
			h += B;
			a = h;
		}
		else if((oper == "=?")||(oper == "=¿?")) // oper A =? B
		{
			a = DATOS_INT.ObtenerVariable(h);
		}
		else
		{
			if((oper == "igual")||(oper == "copiar"))
				oper = "igual";
			if(oper == "¿iguales?")
				oper = "igualA";
			string orden = DATOS_INT.ObtenerVariable(variable1+"#Tipo.")+"."+oper+"#cod. "+variable1+"#NOMBRE. "+h+"#NOMBRE. #(Final).";
			istringstream in(orden);
			procesar("llamar",in,FUNCION);
			if(oper == "igualA")
			{
				a = (*DATOS_INT.pilas)[indicepi].top();
				(*DATOS_INT.pilas)[indicepi].pop();
			}
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
			funcion = valores[1] + funcion;
			en.close();
			en.open(funcion.c_str());
			if(!en)
			{
				cout << "ERROR EN \'utilizar " << func << "\' se traduce a \'utilizar " << funcion << "\' no existe el archivo " << funcion << endl;
				return;
			}
		}
		string a = valores[buscar(nombres,string("__ARCH__"))];
		valores[buscar(nombres,string("__ARCH__"))] = funcion;
		string h;
		while(en >> h)
		{
			procesar(h,en, FUNCION);
		}
		valores[buscar(nombres,string("__ARCH__"))] = a;
	}
	else if(o == "llamar")
	{
		string var;
		e >> var;
		string a = DATOS_INT.ObtenerVariable(var);
		//*
		string b = "";
		e >> b;
		vector<string> param;
		while(b != "#(Final).")
		{
			param.push_back(b);
			e >> b;
		}
		for(int i = 0;i < param.size();i++)
		{
			string& a = DATOS_INT.ObtenerVariable(param[i]);
			pilas[indicepi].push(a);
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
		nombres.push_back(nom);
		valores.push_back(func);
		nombres.push_back(nom+"#NOMBRE.");
		valores.push_back(nom);
		nombres.push_back(nom+"#Tipo.");
		valores.push_back("PseudoFuncion");
		nombres.push_back(nom+"#cod.");
		valores.push_back(func);
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
	else if(o == "empujar")
	{
		string variable1;
		e >> variable1;
		string& a = DATOS_INT.ObtenerVariable(variable1);
		pilas[indicepi].push(a);
	}
	else if(o == "sacar")
	{
		string variable1;
		e >> variable1;
		string& a = DATOS_INT.ObtenerVariable(variable1);
		a = pilas[indicepi].top();
		pilas[indicepi].pop();
	}
	else if(o == "usar_pila")
	{
		e >> indicepi;
			DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL") = to_string(indicepi);
	}
	else if(o == "crear_pila")
	{
		stack<string> t;
		pilas.push_back(t);
			DATOS_INT.ObtenerVariable("VG_NUMERO_PILAS") = to_string(pilas.size());
	}
	else if(o == "si")
	{
		string variable1;
		e >> variable1;
		string& val = DATOS_INT.ObtenerVariable(variable1);
		string cond = ((val == "verdadero")? "si" : "no");
		string ord;
		int i = 0;
		i = AMBITO.size();
		AMBITO.push_back(0);
		while(AMBITO.size() != i)
		{
			e >> ord;
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
		string variable1;
		e >> variable1;
		string& val = DATOS_INT.ObtenerVariable(variable1);
		string cond = ((val == "falso")? "si" : "no");
		string ord;
		int i = 0;
		i = AMBITO.size();
		AMBITO.push_back(0);
		while(AMBITO.size() != i)
		{
			e >> ord;
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
	else if((o == "comparar_i")||(o == "¿son_iguales?"))
	{
		string variable1;
		e >> variable1;
		string& a = DATOS_INT.ObtenerVariable(variable1);
		string var1, var2;
		e >> var1 >> var2;
		string& b = DATOS_INT.ObtenerVariable(var1);
		string& c = DATOS_INT.ObtenerVariable(var2);
		a = "falso";
		if(b == c)
		{
			a = "verdadero";
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
	}
	string libpseudod = "/usr/lib/libpseudod.so";
	if(argc >= 4)
	{
		libpseudod = argv[3];
	}
	void* dll = dlopen(libpseudod.c_str(), RTLD_LAZY);
	if(dll == NULL)
	{
		cout << dlerror() << endl;
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
		cout << "Error al usar libpseudod.so" << endl;
		return 1;
	}
	nombres.push_back(string("__MAIN__"));
	valores.push_back(string(argv[1]));
	nombres.push_back(string("__LIB__"));
	valores.push_back(string(((argc >= 3)? argv[2] : "nulo")));
	nombres.push_back(string("__ARCH__"));
	valores.push_back(string(argv[1]));
	nombres.push_back("VG_PILA_ACTUAL");
	valores.push_back("0");
	nombres.push_back("VG_NUMERO_PILAS");
	valores.push_back("0");
	string a(argv[1]);
#ifndef INTERACTIVO
	ifstream en(a.c_str());
	string base;
	(*funcion)(nombres,valores,punteros,valor,pilas,procesar);
	while((en >> base)&&(Ejecutar))
	{
		procesar(base, en, func2);
	}
#else
	cout << "Interprete en linea de comandos de PseudoD" << endl;
	cout << "Creado por Alejandro Linarez Rangel" << endl;
	cout << "PseudoD version 1.9.0 en C++11" << endl;
	cout << ">>> ";
	string base;
	(*funcion)(nombres,valores,punteros,valor,pilas,procesar);
	while((cin >> base)&&(Ejecutar))
	{
		procesar(base, cin, func2);
		cout << endl << ">>> ";
	}
#endif
	(*func3)();
	return dlclose(dll);
}
