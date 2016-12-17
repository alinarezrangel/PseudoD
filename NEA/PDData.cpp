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

#include "PDData.hh"

//#include "interno/data.cpp"

namespace PDvar
{
	PDDatos::PDDatos(
		std::vector<PDCadena>& nvar,
		std::vector<PDCadena>& vvar,
		std::vector<PDCadena>& npun,
		std::vector<int>& vpun,
		std::vector<std::stack<PDCadena>>& pil
	)
	{
		this->nombrev = &nvar;
		this->valorv = &vvar;
		this->nombrep = &npun;
		this->nvapunt = &vpun;
		this->pilas = &pil;
		this->ERROR = ":C++:error:";
		this->VERDADERO = "verdadero";
		this->FALSO = "falso";
		this->manager = false;
		this->adver = 1;
	}

	PDDatos::PDDatos(void)
	{
		this->nombrev = new std::vector<PDCadena>();
		this->valorv = new std::vector<PDCadena>();
		this->nombrep = new std::vector<PDCadena>();
		this->nvapunt = new std::vector<int>();
		this->pilas = new std::vector<std::stack<PDCadena>>();
		this->ERROR = ":C++:error:";
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

	PDCadena& PDDatos::ObtenerVariable(PDCadena n)
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

	PDCadena& PDDatos::ObtenerPuntero(PDCadena n)
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

	PDCadena PDDatos::Tope(int p)
	{
		return (*this->pilas)[p].top();
	}

	void PDDatos::BorrarTope(int p)
	{
		(*this->pilas)[p].pop();
	}

	void PDDatos::Empujar(PDCadena n, int p)
	{
		(*this->pilas)[p].push(n);
	}

	void PDDatos::CrearPila()
	{
		(*this->pilas).push_back(std::stack<PDCadena>());
	}

	PDCadena PDDatos::Sacar(int n)
	{
		PDCadena val = this->Tope(n);
		this->BorrarTope(n);
		return val;
	}

	void PDDatos::CrearVariable(PDCadena n, bool t, int va, PDCadena vl)
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
		istringstream in(ord);
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
		for(decltype(nombre)::size_type i = 0; i < nombre.size(); i++)
		{
			if(nombre[i] == '<')
			{
				inEval = true;
				continue;
			}
			if(nombre[i] == '>')
			{
				inEval = false;
				res += this->ObtenerVariable(buff);
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

	PDCadena ValorDelToken(PDCadena tok, std::istream& in, PDDatos* data)
	{
		if(tok == "llamar")
		{
			data->Ejecutar("llamar", in);
			string top = data->Sacar(cae(data->ObtenerVariable("VG_PILA_ACTUAL")));
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
				throw string("Token invalido: se esperaba \"es\" no " + arg2);
			orden += " " + arg1 + " " + op + " ___codigo_pseudod_buffer_interno___";
			data->Ejecutar(orden);
			return (
				(data->ObtenerVariable("___codigo_pseudod_buffer_interno___")
					== data->ObtenerVariable(arg3))?
					"verdadero" : "falso"
				);
		}
		if(tok == "¿son_iguales?") // verificado
		{
			PDCadena arg1 = "", arg2 = "";
			in >> arg1 >> arg2;
			return (
				(data->ObtenerVariable(arg1) == data->ObtenerVariable(arg2))?
					"verdadero" : "falso"
				);
		}
		if((tok == "son") || (tok == "sean"))
		{
			PDCadena op = "", tok1 = "", val1 = "", tok2 = "", val2 = "", ytok = "";
			in >> op >> tok1;
			val1 = ValorDelToken(tok1, in, data);
			in >> ytok >> tok2;
			val2 = ValorDelToken(tok2, in, data);
			if(ytok != "y")
			{
				throw PDvar::ErrorDeSintaxis(
					PDCadena("Error en el parser(expr): 'son/sean iguales/diferentes") +
					" expr y expr': se esperaba 'y' no " + ytok
				);
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
			return (ValorDelToken(pd, in, data) == "verdadero")?
				"falso" : "verdadero";
		}
		if(tok[0] == '{')
		{
			if(tok[tok.size() - 1] == '}')
			{
				return tok.substr(1, tok.size() - 2);
			}
			string str = "";
			getline(in, str, '}');
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
