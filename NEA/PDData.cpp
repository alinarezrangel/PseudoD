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

#include "PDData.hh"

namespace PDvar
{
	PDDatos::PDDatos(PDDatos& dt)
	{
		this->nombrev = dt.nombrev;
		this->valorv = dt.valorv;
		this->nombrep = dt.nombrep;
		this->nvapunt = dt.nvapunt;
		this->pilas = dt.pilas;
		this->ERROR = dt.ERROR;
		this->VERDADERO = dt.VERDADERO;
		this->FALSO = dt.FALSO;
		this->manager = false;
		this->adver = dt.adver;
	}

	PDDatos::PDDatos(void)
	{
		this->nombrev = std::make_shared<std::vector<PDCadena>>(std::vector<PDCadena>());
		this->valorv = std::make_shared<std::vector<Variante>>(std::vector<Variante>());
		this->nombrep = std::make_shared<std::vector<PDCadena>>(std::vector<PDCadena>());
		this->nvapunt = std::make_shared<std::vector<int>>(std::vector<int>());
		this->pilas = std::make_shared<std::vector<std::stack<Variante>>>(std::vector<std::stack<Variante>>());
		this->ERROR = PDCadena(":C++:error:");
		this->VERDADERO = PDCadena("verdadero");
		this->FALSO = PDCadena("falso");
		this->manager = true;
		this->adver = 1;
	}

	PDDatos::~PDDatos()
	{
	}

	Variante& PDDatos::ObtenerVariable(PDCadena n)
	{
		n = this->ResolverNombre(n);
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

	Variante& PDDatos::ObtenerPuntero(PDCadena n)
	{
		n = this->ResolverNombre(n);
		int i = buscar((*this->nombrep),n);
		if(i == -1)
		{
			throw PDvar::ErrorDelNucleo(
				"Error en el manejador de datos: C++'ObtenerPuntero(" +
				n + ")': No se encontro el puntero"
			);
		}
		else
		{
			return (*this->valorv)[(*this->nvapunt)[i]];
		}
	}

	int& PDDatos::ObtenerIndicePuntero(PDCadena n)
	{
		n = this->ResolverNombre(n);
		int i = buscar((*this->nombrep), n);
		if(i == -1)
		{
			throw PDvar::ErrorDelNucleo(
				"Error en el manejador de datos: C++'ObtenerVariable(" +
				n + ")': No se encontro la variable o puntero"
			);
		}
		else
		{
			return (*this->nvapunt)[i];
		}
	}

	Variante PDDatos::Tope(int p)
	{
		return (*this->pilas)[p].top();
	}

	void PDDatos::BorrarTope(int p)
	{
		(*this->pilas)[p].pop();
	}

	void PDDatos::Empujar(const Variante& n, int p)
	{
		(*this->pilas)[p].push(n);
	}

	void PDDatos::CrearPila()
	{
		(*this->pilas).push_back(std::stack<Variante>());
	}

	Variante PDDatos::Sacar(int n)
	{
		Variante val = this->Tope(n);
		this->BorrarTope(n);
		return val;
	}

	void PDDatos::CrearVariable(PDCadena n, bool t, int va, const Variante& vl)
	{
		if(t)
		{
			(*this->nombrev).push_back(n);
			(*this->valorv).push_back(vl);
		}
		else
		{
			(*this->nombrep).push_back(n);
			(*this->nvapunt).push_back(va);
		}
	}

	int PDDatos::BuscarIndice(bool t, PDCadena n)
	{
		n = this->ResolverNombre(n);
		int ind = 0;
		if(t)
		{
			ind = buscar((*this->nombrev), n);
		}
		else
		{
			ind = buscar((*this->nombrep), n);
		}
		return ind;
	}

	bool PDDatos::ExisteVariable(bool t, PDCadena n)
	{
		n = this->ResolverNombre(n);
		int ind = -1;
		if(t)
		{
			ind = buscar((*this->nombrev), n);
		}
		else
		{
			ind = buscar((*this->nombrep), n);
		}
		return (ind >= 0);
	}

	void PDDatos::Ejecutar(PDCadena ord)
	{
		std::istringstream in(ord);
		PDCadena orden = "escribir CMMERROR";
		in >> orden;
		(*this->PROCESAR)(orden, in, this->PROCESO);
	}

	void PDDatos::Ejecutar(PDCadena ord, std::istream& in)
	{
		(*this->PROCESAR)(ord, in, this->PROCESO);
	}

	PDCadena PDDatos::ResolverNombre(PDCadena nombre)
	{
		// Se debe seleccionar todos los texto entre "<" y ">" para
		// evaluarlos hacia un nombre.
		if(nombre.find('<') == std::string::npos)
			return nombre;
		PDCadena res = "";
		PDCadena buff = "";
		bool inEval = false;
		for(int i = 0; i < nombre.size(); i++)
		{
			if(nombre[i] == '<')
			{
				inEval = true;
				continue;
			}
			if(nombre[i] == '>')
			{
				inEval = false;
				res += this->ObtenerVariable(buff).ObtenerCadena();
				buff = "";
				continue;
			}
			if(inEval)
			{
				buff += nombre[i];
			}
			else
			{
				res += nombre[i];
			}
		}
		return res;
	}
	PDDatos& PDDatos::operator=(PDDatos& dt)
	{
		if(this != &dt)
		{
			this->nombrev = dt.nombrev;
			this->valorv = dt.valorv;
			this->nombrep = dt.nombrep;
			this->nvapunt = dt.nvapunt;
			this->pilas = dt.pilas;
			this->ERROR = dt.ERROR;
			this->VERDADERO = dt.VERDADERO;
			this->FALSO = dt.FALSO;
			this->manager = false;
			this->adver = dt.adver;
		}
		return *this;
	}

	PDEntradaBasica::PDEntradaBasica(PDCadena tok, std::istream& in, PDDatos& dat)
	{
		this->token = tok;
		this->in = &in;
		this->data = &dat;
	}

	PDEntradaBasica::~PDEntradaBasica(void)
	{
	}

	PDCadena PDEntradaBasica::ObtenerToken(void)
	{
		return this->token;
	}

	std::istream& PDEntradaBasica::ObtenerFlujo(void)
	{
		return (*(this->in));
	}

	PDDatos* PDEntradaBasica::ObtenerMemoria(void)
	{
		return this->data;
	}

	PDvar::Variante ValorDelToken(PDCadena tok, std::istream& in, PDDatos* data)
	{
		if(tok == "llamar")
		{
			data->Ejecutar("llamar", in);
			PDvar::Variante top = data->Sacar(data->ObtenerVariable("VG_PILA_ACTUAL"));
			return top;
		}
		if(tok == "comparar")
		{
			PDCadena orden = "", arg1 = "", op = "", arg2 = "";
			in >> orden >> arg1 >> op >> arg2;
			orden += " " + arg1 + " " + op + " " + arg2 +
				" ___codigo_pseudod_buffer_interno___";
			data->Ejecutar(orden);
			return data->ObtenerVariable("___codigo_pseudod_buffer_interno___");
		}
		if(tok == "ejecutar")
		{
			PDCadena orden = "", arg1 = "", op = "", arg2 = "", arg3 = "";
			in >> orden >> arg1 >> op >> arg2 >> arg3;
			if(arg2 != "es")
				throw PDvar::ErrorDeSintaxis("Token invalido: se esperaba \"es\" no " + arg2);
			orden += " " + arg1 + " " + op + " ___codigo_pseudod_buffer_interno___";
			data->Ejecutar(orden);
			return (
				(data->ObtenerVariable("___codigo_pseudod_buffer_interno___")
					== data->ObtenerVariable(arg3))?
					data->VERDADERO : data->FALSO
				);
		}
		if(tok == "¿son_iguales?") // verificado
		{
			PDCadena arg1 = "", arg2 = "";
			in >> arg1 >> arg2;
			return (
				(data->ObtenerVariable(arg1) == data->ObtenerVariable(arg2))?
					data->VERDADERO : data->FALSO
				);
		}
		if((tok == "son") || (tok == "sean"))
		{
			PDCadena op = "", tok1 = "", tok2 = "", ytok = "";
			in >> op >> tok1;
			PDvar::Variante val1 = ValorDelToken(tok1, in, data);
			in >> ytok >> tok2;
			PDvar::Variante val2 = ValorDelToken(tok2, in, data);
			if(ytok != "y")
			{
				throw PDvar::ErrorDeSintaxis(
					PDCadena("Error en el parser(expr): 'son/sean iguales/diferentes") +
					" expr y expr': se esperaba 'y' no " + ytok
				);
			}
			if(op == "iguales")
			{
				return ((val1 == val2)? data->VERDADERO : data->FALSO);
			}
			if(op == "diferentes")
			{
				return ((val1 != val2)? data->VERDADERO : data->FALSO);
			}
			throw PDvar::ErrorDeSintaxis(
				PDCadena("Error en el parser(expr): 'son/sean iguales/diferentes") +
				" expr y expr': se esperaba 'iguales/diferentes' no " + op
			);
		}
		if(tok == "no") // verificado
		{
			PDCadena pd;
			in >> pd;
			return (ValorDelToken(pd, in, data) == data->VERDADERO)?
				data->FALSO : data->VERDADERO;
		}
		if(tok[0] == '{')
		{
			if(tok[tok.size() - 1] == '}')
			{
				return tok.substr(1, tok.size() - 2);
			}
			PDCadena str = "";
			std::getline(in, str, '}');
			return tok.substr(1, tok.size()) + str;
		}
		if((tok.size() >= 2)
			&& (tok[0] == (char)0xC2)
			&& (tok[1] == (char)0xAB)) // Comillas angulares (apertura "«")
		{
			char l = tok[tok.size() - 1];
			char l2 = tok[tok.size() - 2];
			// Comillas angulares (cierre "»")
			if((l2 == (char)0xC2) && (l == (char)0xBB))
			{
				// Fin:
				return tok.substr(2, tok.size() - 4);
			}
			// Busca el final:
			l = (char)0;
			l2 = (char)0;
			PDCadena str = "";
			do
			{
				if(!in)
				{
					throw PDvar::ErrorDeSintaxis(
						"Error en el parser(expr): 'expr' es «»: EOF inesperado"
					);
				}
				str += in.get();
				if(str.size() >= 2)
				{
					l = str[str.size() - 1];
					l2 = str[str.size() - 2];
				}
			}
			while((l2 != (char)0xC2) || (l != (char)0xBB));
			// <= Comillas angulares (cierre "»")
			return tok.substr(2, tok.size()) + str.substr(0, str.size() - 2);
		}
		if(data->ExisteVariable(true, tok) || data->ExisteVariable(false, tok))
		{
			return data->ObtenerVariable(tok);
		}
		throw PDvar::ErrorDelNucleo(
			"Error en el parser(expr): 'expr' es '" + tok +
			"': Token invalido"
		);
	}

	namespace Din
	{
		ModuloDinamico::ModuloDinamico(void)
		{}
		ModuloDinamico::~ModuloDinamico(void)
		{}
		bool ModuloDinamico::ManejarFuncion(PDvar::Din::Argumentos* args)
		{
			return true;
		}
	}
}
