/*
**PseudoD 1.5.0
**Creado por Alejandro Linarez Rangel
*/
#ifndef PDDATA_DEFINED_H
#define PDDATA_DEFINED_H

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
	}

	PDDatos::~PDDatos()
	{
		this->nombrev = NULL;
		this->valorv = NULL;
		this->nombrep = NULL;
		this->nvapunt = NULL;
		this->pilas = NULL;
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
			return PDDatos::ERROR;
		}
		else
		{
			return (*this->valorv)[(*this->nvapunt)[i]];
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

	void PDDatos::CrearVariable(string n,string t,int va)
	{
		if(t == "Puntero")
		{
			(*this->nombrep).push_back(n);
			(*this->nvapunt).push_back(va);
		}
		else
		{
			(*this->nombrev).push_back(n);
			(*this->valorv).push_back(string("nulo"));
		}
	}
}

#endif
