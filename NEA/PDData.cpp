/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*** Este es PseudoD version 2.0.0                                      *****
*** Log de este archivo:                                               *****
*** Formato: DD/MM/YYYY: txt                                           *****
*** **** 14/09/2014: Se creo el archivo.                               *****
*** **** 08/03/2015: Se agrego soporte para CygWin y Windows           *****
*** **** 26/04/2015: Se agrego soporte para Meta-Programacion          *****
*** **** 26/04/2015: Se agrego el operador "son"                       *****
****************************************************************************
**************************************************************************/


#ifndef PDDATA_DEFINED_H
#define PDDATA_DEFINED_H

#include "PDData.hh"

template<class T>
int buscar(std::vector<T> a,T b)
{
	for(int i = (a.size() - 1);i >= 0;i--)
	{
		if(a[i] == b)
		{
			return i;
		}
	}
	return -1;
}

std::string eas(long long int i)
{
#ifndef MINGW
	return std::to_string(i);
#else
	char buffer[50];
	sprintf(buffer,"%d",i);
	return std::string(buffer);
#endif
}

std::string dac(long double i)
{
#ifndef MINGW
	return std::to_string(i);
#else
	char buffer[50];
	sprintf(buffer,"%a",i);
	return std::string(buffer);
#endif
}

long long int cae(std::string i)
{
#ifndef MINGW
	return std::stoll(i);
#else
	return atoi(i.c_str());
#endif
}

long double caf(std::string i)
{
#ifndef MINGW
	return std::stold(i);
#else
	return atof(i.c_str());
#endif
}

namespace PDvar
{
	template<typename RV,typename AT>
	PDCallBack<RV,AT>::PDCallBack (RV (*F)(AT)) : PDObjeto()
	{
		this->Fcn = &F;
	}
	
	template<typename RV,typename AT>
	PDCallBack<RV,AT>::~PDCallBack ()
	{
		// ~~~~~~~~~Nada
	}
	
	template<typename RV,typename AT>
	RV PDCallBack<RV,AT>::Call(AT p)
	{
		return (**this->Fcn)(p);
	}
	
	template<typename RV,typename AT>
	RV PDCallBack<RV,AT>::Call()
	{
		return (**this->Fcn)(this->param);
	}
	
	template<typename RV,typename AT>
	void PDCallBack<RV,AT>::Set(AT v)
	{
		this->param = v;
	}
	
	template<typename RV,typename AT>
	AT PDCallBack<RV,AT>::Get()
	{
		return this->param;
	}
	
	Error::Error(void) noexcept : PseudoDClass("Error"), ErrorMessage("")
	{
	}
	Error::Error(const Error& other) noexcept
	{
		this->PseudoDClass = other.ObtenerClaseEnPseudoD();
		this->ErrorMessage = other.ObtenerErrorElemental();
	}
	Error::Error(string classname,string message) noexcept
		: PseudoDClass(classname), ErrorMessage(message)
	{
	}
	Error::~Error(void) noexcept
	{
	}
	Error& Error::operator=(const Error& other) noexcept
	{
		this->PseudoDClass = other.ObtenerClaseEnPseudoD();
		this->ErrorMessage = other.ObtenerErrorElemental();
	}
	const char* Error::what(void) noexcept
	{
		return this->ObtenerErrorElemental().c_str();
	}
	string Error::ObtenerClaseEnPseudoD(void) const noexcept
	{
		return this->PseudoDClass;
	}
	string Error::ObtenerErrorElemental(void) const noexcept
	{
		return this->ErrorMessage;
	}
	void Error::FijarClase(string classname) noexcept
	{
		this->PseudoDClass = classname;
	}
	void Error::FijarMensaje(string message) noexcept
	{
		this->ErrorMessage = message;
	}
	string Error::Mensaje(void) const noexcept
	{
		return "(" + this->ObtenerClaseEnPseudoD() + ") " + this->ObtenerErrorElemental();
	}
	ErrorDeSintaxis::ErrorDeSintaxis(void) noexcept : Error("ErrorDeSintaxis","")
	{}
	ErrorDeSintaxis::ErrorDeSintaxis(string message) noexcept : Error("ErrorDeSintaxis",message)
	{}
	ErrorDeSintaxis::ErrorDeSintaxis(const Error& other) noexcept : Error(other)
	{}
	ErrorDeSintaxis::~ErrorDeSintaxis(void) noexcept
	{}
	ErrorDeSemantica::ErrorDeSemantica(void) noexcept : Error("ErrorDeSemantica","")
	{}
	ErrorDeSemantica::ErrorDeSemantica(string message) noexcept : Error("ErrorDeSemantica",message)
	{}
	ErrorDeSemantica::ErrorDeSemantica(const Error& other) noexcept : Error(other)
	{}
	ErrorDeSemantica::~ErrorDeSemantica(void) noexcept
	{}
	ErrorDelNucleo::ErrorDelNucleo(void) noexcept : Error("ErrorDelNucleo","")
	{}
	ErrorDelNucleo::ErrorDelNucleo(string message) noexcept : Error("ErrorDelNucleo",message)
	{}
	ErrorDelNucleo::ErrorDelNucleo(const Error& other) noexcept : Error(other)
	{}
	ErrorDelNucleo::~ErrorDelNucleo(void) noexcept
	{}
	
	PDDatos::PDDatos(vector<string>& nvar,vector<string>& vvar,vector<string>& npun,vector<int>& vpun,vector< stack<string> >& pil)
	{
		this->nombrev = &nvar;
		this->valorv = &vvar;
		this->nombrep = &npun;
		this->nvapunt = &vpun;
		this->pilas = &pil;
		this->ERROR = ":C++:Error:";
		this->VERDADERO = "verdadero";
		this->FALSO = "falso";
		this->manager = false;
		this->adver = 1;
	}
	
	PDDatos::PDDatos(void)
	{
		this->nombrev = new vector<string>();
		this->valorv = new vector<string>();
		this->nombrep = new vector<string>();
		this->nvapunt = new vector<int>();
		this->pilas = new vector< stack<string> >();
		this->ERROR = ":C++:Error:";
		this->VERDADERO = "verdadero";
		this->FALSO = "falso";
		this->manager = true;
		this->adver = 1;
	}

	PDDatos::~PDDatos()
	{
		if(this->manager)
		{
			delete this->nombrev;
			delete this->valorv;
			delete this->nombrep;
			delete this->nvapunt;
			delete this->pilas;
		}
		else
		{
			this->nombrev = NULL;
			this->valorv = NULL;
			this->nombrep = NULL;
			this->nvapunt = NULL;
			this->pilas = NULL;
		}
	}

	string& PDDatos::ObtenerVariable(string n)
	{
		int i = buscar((*this->nombrev),n);
		if(i == -1)
		{
			return this->ObtenerPuntero(n);
		}
		else
		{
			return (*this->valorv)[i];
		}
	}

	string& PDDatos::ObtenerPuntero(string n)
	{
		int i = buscar((*this->nombrep),n);
		if(i == -1)
		{
			throw PDvar::ErrorDelNucleo("Error en el manejador de datos: C++'ObtenerPuntero(n)': No se encontro el puntero");
		}
		else
		{
			return (*this->valorv)[(*this->nvapunt)[i]];
		}
	}
	
	int& PDDatos::ObtenerIndicePuntero(string n)
	{
		int i = buscar((*this->nombrep),n);
		if(i == -1)
		{
			throw PDvar::ErrorDelNucleo("Error en el manejador de datos: C++'ObtenerVariable(n)': No se encontro la variable o puntero");
		}
		else
		{
			return (*this->nvapunt)[i];
		}
	}

	string PDDatos::Tope(int p)
	{
		return (*this->pilas)[p].top();
	}

	void PDDatos::BorrarTope(int p)
	{
		(*this->pilas)[p].pop();
	}

	void PDDatos::Empujar(string n, int p)
	{
		(*this->pilas)[p].push(n);
	}

	void PDDatos::CrearPila()
	{
		(*this->pilas).push_back(stack<string>());
	}
	
	string PDDatos::Sacar(int n)
	{
		string val = this->Tope(n);
		this->BorrarTope(n);
		return val;
	}

	void PDDatos::CrearVariable(string n,string t,int va,string vl)
	{
		if(t == "Puntero")
		{
			(*this->nombrep).push_back(n);
			(*this->nvapunt).push_back(va);
		}
		else
		{
			(*this->nombrev).push_back(n);
			(*this->valorv).push_back(vl);
		}
	}
	
	int PDDatos::BuscarIndice(string t,string n)
	{
		int ind = 0;
		if(t == "Variable")
		{
			ind = buscar((*this->nombrev),n);
		}
		else
		{
			ind = buscar((*this->nombrep),n);
		}
		return ind;
	}
	
	bool PDDatos::ExisteVariable(string n,string t)
	{
		int ind = -1;
		if(t == "Variable")
		{
			ind = buscar((*this->nombrev),n);
		}
		else
		{
			ind = buscar((*this->nombrep),n);
		}
		return (ind >= 0);
	}
	
	void PDDatos::Ejecutar(string ord)
	{
		istringstream in(ord);
		string orden = "escribir CMMERROR";
		in >> orden;
		(*this->PROCESAR)(orden,in,this->PROCESO);
	}
	
	void PDDatos::Ejecutar(string ord,istream& in)
	{
		(*this->PROCESAR)(ord,in,this->PROCESO);
	}
	
	/*
	explicit PDEntradaBasica(string tok, istream& in, PDDatos& dat);
			virtual ~PDEntradaBasica(void);
			istream& ObtenerFlujo(void);
			PDDatos* ObtenerMemoria(void);
			string ObtenerToken(void);
	*/
	PDEntradaBasica::PDEntradaBasica(string tok, istream& in, PDDatos& dat)
	{
		this->token = tok;
		this->in = &in;
		this->data = &dat;
	}
	
	PDEntradaBasica::~PDEntradaBasica(void)
	{
	}
	
	string PDEntradaBasica::ObtenerToken(void)
	{
		return this->token;
	}
	
	istream& PDEntradaBasica::ObtenerFlujo(void)
	{
		return (*(this->in));
	}
	
	PDDatos* PDEntradaBasica::ObtenerMemoria(void)
	{
		return this->data;
	}
	
	string ValorDelToken(string tok,istream& in,PDDatos* data)
	{
		/*
		(variable|llamar funcion args #(Final).|comparar|ejecutar|¿son_iguales?|no)
		*/
		if(tok == "llamar")
		{
			data->Ejecutar("llamar",in);
			string top = data->Sacar(cae(data->ObtenerVariable("VG_PILA_ACTUAL")));
			return top;
		}
		if(tok == "comparar")
		{
			string orden,arg1,op,arg2;
			in >> orden >> arg1 >> op >> arg2;
			orden += " " + arg1 + " " + op + " " + arg2 + " ___codigo_pseudod_buffer_interno___";
			data->Ejecutar(orden);
			return data->ObtenerVariable("___codigo_pseudod_buffer_interno___");
		}
		if(tok == "ejecutar")
		{
			string orden,arg1,op,arg2,arg3;
			in >> orden >> arg1 >> op >> arg2 >> arg3;
			if(arg2 != "es")
				throw string("Token invalido: se esperaba \"es\" no " + arg2);
			orden += " " + arg1 + " " + op + " ___codigo_pseudod_buffer_interno___";
			data->Ejecutar(orden);
			return ((data->ObtenerVariable("___codigo_pseudod_buffer_interno___") == data->ObtenerVariable(arg3))? "verdadero" : "falso");
		}
		if(tok == "¿son_iguales?") // verificado
		{
			string arg1,arg2;
			in >> arg1 >> arg2;
			return ((data->ObtenerVariable(arg1) == data->ObtenerVariable(arg2))? "verdadero" : "falso");
		}
		if((tok == "son")||(tok == "sean"))
		{
			string op,tok1,val1,tok2,val2,ytok;
			in >> op >> tok1;
			val1 = ValorDelToken(tok1,in,data);
			in >> ytok >> tok2;
			val2 = ValorDelToken(tok2,in,data);
			if(ytok != "y")
			{
				throw string("No se reconoce el token " + tok + " se esperaba \"y\"");
			}
			if(op == "iguales")
			{
				return ((val1 == val2)? "verdadero" : "falso");
			}
			if(op == "diferentes")
			{
				return ((val1 != val2)? "verdadero" : "falso");
			}
		}
		if(tok == "no") // verificado
		{
			string pd;
			in >> pd;
			return (ValorDelToken(pd,in,data) == "verdadero")? "falso" : "verdadero";
		}
		if((buscar((*data->nombrev),tok) != -1)||(buscar((*data->nombrep),tok) != -1))
		{
			return data->ObtenerVariable(tok);
		}
		throw PDvar::ErrorDelNucleo("Error en el parser(expr): 'expr': Token invalido");
	}
}

#endif
