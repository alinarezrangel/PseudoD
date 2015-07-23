/*
**PseudoD 1.5.0
**Creado por Alejandro Linarez Rangel
*/
#ifndef PDDATA_DEFINED_H
#define PDDATA_DEFINED_H

#include "PDData.hh"

template<class T>
int buscar(std::vector<T> a,T b)
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
			throw std::string("Error en el manejador de memoria de PseudoD: No existe la variable o puntero "+n);
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
			throw std::string("Error en el manejador de memoria de PseudoD: No existe la variable o puntero "+n);
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
		int ind;
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
	
	void PDDatos::Ejecutar(string ord)
	{
		istringstream in(ord);
		string orden = "";
		in >> orden;
		(*(this->PROCESAR))(orden,in,this->PROCESO);
	}
	
	void PDDatos::Ejecutar(string ord,istream& in)
	{
		string orden = "";
		(*(this->PROCESAR))(orden,in,this->PROCESO);
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
			data->Ejecutar(tok,in);
			string top = data->Tope(cae(data->ObtenerVariable("VG_PILA_ACTUAL")));
			data->BorrarTope(cae(data->ObtenerVariable("VG_PILA_ACTUAL")));
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
		if(tok == "¿son_iguales?")
		{
			string arg1,arg2;
			in >> arg1 >> arg2;
			return ((data->ObtenerVariable(arg1) == data->ObtenerVariable(arg2))? "verdadero" : "falso");
		}
		if(tok == "¿no_son_iguales?")
		{
			string arg1,arg2;
			in >> arg1 >> arg2;
			return ((data->ObtenerVariable(arg1) != data->ObtenerVariable(arg2))? "verdadero" : "falso");
		}
		throw string("No se reconoce el token " + tok);
	}
}

#endif
