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

	PDDatos::PDDatos(PDDatos& otro)
	{
		this->nombrev = otro.nombrev;
		this->valorv = otro.valorv;
		this->nombrep = otro.nombrep;
		this->nvapunt = otro.nvapunt;
		this->pilas = otro.pilas;
		this->ERROR = otro.ERROR;
		this->VERDADERO = otro.VERDADERO;
		this->FALSO = otro.FALSO;
		this->manager = false;
		this->adver = otro.adver;
	}

	PDDatos::~PDDatos(void)
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
		int i = buscar((*this->nombrev), n);
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
		int i = buscar((*this->nombrep), n);
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

	void PDDatos::CrearPila(void)
	{
		this->pilas->push_back(std::stack<PDCadena>());
	}

	PDCadena PDDatos::Sacar(int n)
	{
		PDCadena val = this->Tope(n);
		this->BorrarTope(n);
		return val;
	}

	bool PDDatos::PilaVacia(int n)
	{
		return (*this->pilas)[n].empty();
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
		std::istringstream in(ord);
		pseudod::Tokenizador tkz(in);
		pseudod::Token tok;

		tkz >> tok;

		(*this->PROCESAR)(tok, tkz, this->PROCESO);

		//while(!tkz.FinDelFlujo())
		//	(*this->PROCESAR)(tkz, this->PROCESO);
	}

	void PDDatos::Ejecutar(PDCadena ord, std::istream& in)
	{
		std::istringstream zin(ord);
		pseudod::Tokenizador tkz(zin), tkq(in);
		pseudod::Token tok;

		tkz >> tok;

		(*this->PROCESAR)(tok, tkq, this->PROCESO);

		//while(!tkq.FinDelFlujo())
		//	(*this->PROCESAR)(tkq, this->PROCESO);
	}

	void PDDatos::Ejecutar(PDCadena ord, pseudod::Tokenizador& in)
	{
		std::istringstream ins(ord);
		pseudod::Tokenizador tkz(ins);
		pseudod::Token tok;

		tkz >> tok;

		(*this->PROCESAR)(tok, in, this->PROCESO);

		//while(!in.FinDelFlujo())
		//	(*this->PROCESAR)(in, this->PROCESO);
	}

	void PDDatos::Ejecutar(pseudod::Token ord, pseudod::Tokenizador& in)
	{
		(*this->PROCESAR)(ord, in, this->PROCESO);

		//while(!in.FinDelFlujo())
		//	(*this->PROCESAR)(in, this->PROCESO);
	}

	PDCadena PDDatos::ResolverNombre(PDCadena nombre)
	{
		// Se debe seleccionar todos los texto entre "<" y ">" para
		// evaluarlos hacia un nombre.
		if(nombre.find('<') == PDCadena::npos)
			return nombre;

		std::stack<PDCadena> buffs;
		buffs.push("");

		for(decltype(nombre)::size_type i = 0; i < nombre.size(); i++)
		{
			if(nombre[i] == '<')
			{
				buffs.push("");

				continue;
			}

			if(nombre[i] == '>')
			{
				if(buffs.size() == 1)
				{
					throw PDvar::ErrorDeSintaxis(
						"Resolucion de nombres: '>' unbalanceado con '" + nombre + "'"
					);
				}

				PDCadena& var = this->ObtenerVariable(buffs.top());
				buffs.pop();
				buffs.top() += var;

				continue;
			}

			buffs.top() += nombre[i];
		}

		return buffs.top();
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

	PDCadena ValorDelToken(pseudod::Token tok, pseudod::Tokenizador& in, PDDatos* data)
	{
		using namespace pseudod::Utilidades;

		if(Tokens::EsCadena(tok))
		{
			return Tokens::ObtenerValor(tok);
		}

		if(Tokens::EsIdentificador(tok))
		{
			return data->ObtenerVariable(Tokens::ObtenerValor(tok));
		}

		if(Tokens::EsComentario(tok))
		{
			in >> tok;

			return ValorDelToken(tok, in, data);
		}

		if(Tokens::EsCuerpoDeCodigo(tok))
		{
			throw PDvar::ErrorDelNucleo(
				"Error en el parser(expr): Cuerpo de codigo inesperado en la expresion"
			);
		}

		pseudod::NMemonicoProxy proxy = tok.ObtenerNMemonico();

		if(proxy == pseudod::NMemonico::PD_OPERADOR_TANTO)
		{
			pseudod::Token como, var;
			bool vl = true, ex = false;

			do
			{
				if(!(in >> var))
				{
					throw PDvar::ErrorDeSintaxis(
						"Error en el parser(expr): EOF inesperado"
					);
				}

				ex = (ValorDelToken(var, in, data) == data->VERDADERO);

				vl = vl && ex;

				if(in.FinDelFlujo())
				{
					throw PDvar::ErrorDeSintaxis(
						"Error en el parser(expr): EOF inesperado"
					);
				}

				como = in.ObtenerTokenActual();

				if((!Tokens::EsNMemonico(como))
					|| (como.ObtenerNMemonico() != pseudod::NMemonico::PD_OPERADOR_COMO))
				{
					break;
				}

				in >> como;
			} while(true);

			return vl? data->VERDADERO : data->FALSO;
		}

		if(proxy == pseudod::NMemonico::PD_OPERADOR_ALGUN)
		{
			pseudod::Token como, var;
			bool vl = true, ex = false;

			do
			{
				if(!(in >> var))
				{
					throw PDvar::ErrorDeSintaxis(
						"Error en el parser(expr): EOF inesperado"
					);
				}

				ex = (ValorDelToken(var, in, data) == data->VERDADERO);

				vl = vl || ex;

				if(in.FinDelFlujo())
				{
					throw PDvar::ErrorDeSintaxis(
						"Error en el parser(expr): EOF inesperado"
					);
				}

				como = in.ObtenerTokenActual();

				if((!Tokens::EsNMemonico(como))
					|| (como.ObtenerNMemonico() != pseudod::NMemonico::PD_OPERADOR_O))
				{
					break;
				}

				in >> como;
			} while(true);

			return vl? data->VERDADERO : data->FALSO;
		}

		if(proxy == pseudod::NMemonico::PD_SI)
		{
			in >> tok;

			return ValorDelToken(tok, in, data);
		}

		if(proxy == pseudod::NMemonico::PD_LLAMAR)
		{
			data->Ejecutar("llamar", in);

			int n = cae(data->ObtenerVariable("VG_PILA_ACTUAL"));

			if(data->PilaVacia(n))
			{
				throw PDvar::ErrorDeSemantica(
					"Error en el parser(expr): 'llamar ... FIN': La pila esta vacia"
				);
			}

			return data->Sacar(n);
		}

		if(proxy == pseudod::NMemonico::PD_OPERADOR_COMPARAR)
		{
			pseudod::Token ord;

			in >> ord;

			// Nota: el siguiente código puede ser extremadamente eficiente o
			// impresionantemente lento dependiendo de la implementacion de
			// pseudod::Tokenizador::ListaTokens
			auto iter = in.ObtenerIterador();
			pseudod::Tokenizador::ListaTokens& ls = in.ObtenerTokens();

			// iter es o un BidirectionalIterator o un RandomAccessIterator o un
			// ForwardIterator, la funcion pseudod::IncrementaIteradorPor funciona
			// con cualquier ForwardIterator y permite moverlo cual
			// RandomAccessIterator. Por seguridad de tipo, es mejor asumir que el
			// iterador es el menos poderoso y tratarlo commo ForwardIterator:
			auto itr = pseudod::IncrementaIteradorPor(iter, 3);

			pseudod::Token::ValorLiteral vl;

			vl.valor = "___codigo_pseudod_buffer_interno___";
			vl.tipo = pseudod::Token::ValorLiteral::Identificador;

			pseudod::Token faketoken(vl);

			//std::cout << "<" << in.ObtenerTokens().size() << std::endl;

			auto it = ls.insert(itr, faketoken);

			//std::cout << ">" << in.ObtenerTokens().size() << std::endl;

			data->Ejecutar(ord, in);

			ls.erase(it);

			return data->ObtenerVariable("___codigo_pseudod_buffer_interno___");
		}

		if(proxy == pseudod::NMemonico::PD_OPERADOR_EJECUTAR)
		{
			pseudod::Token ord, es, vl;

			in >> ord;

			auto iter = in.ObtenerIterador();
			auto ls = in.ObtenerTokens();
			auto itr = pseudod::IncrementaIteradorPor(iter, 2);

			pseudod::Token::ValorLiteral vl2;

			vl2.valor = "___codigo_pseudod_buffer_interno___";
			vl2.tipo = pseudod::Token::ValorLiteral::Identificador;

			pseudod::Token faketoken(vl2);

			ls.insert(itr, faketoken);

			data->Ejecutar(ord, in);

			in >> es >> vl;

			if((!Tokens::EsNMemonico(es))
				|| (es.ObtenerNMemonico() != pseudod::NMemonico::PD_OPERADOR_ES))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en el parser(expr): 'ejecutar ... es ...': Se esperaba 'es'"
				);
			}

			PDCadena vl3 = ValorDelToken(vl, in, data);

			return (
				(data->ObtenerVariable("___codigo_pseudod_buffer_interno___") == vl3)?
				data->VERDADERO : data->FALSO
			);
		}

		if(proxy == pseudod::NMemonico::PD_OPERADOR_SON_IGUALES)
		{
			pseudod::Token arg1, arg2;

			in >> arg1 >> arg2;

			if((!Tokens::EsIdentificador(arg1)) || (!Tokens::EsIdentificador(arg2)))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en el parser(expr): 'comparar_i ...': Se esperaban dos "
					"identificadores"
				);
			}

			return (
				(data->ObtenerVariable(Tokens::ObtenerValor(arg1))
					== data->ObtenerVariable(Tokens::ObtenerValor(arg2)))?
					data->VERDADERO : data->FALSO
			);
		}

		if(proxy == pseudod::NMemonico::PD_OPERADOR_SON)
		{
			pseudod::Token op, tok1, tok2, ytok;
			PDCadena val1 = "", val2 = "";

			in >> op >> tok1;
			val1 = ValorDelToken(tok1, in, data);
			in >> ytok >> tok2;
			val2 = ValorDelToken(tok2, in, data);

			if(
				(!Tokens::EsNMemonico(ytok)) ||
				(ytok.ObtenerNMemonico() != pseudod::NMemonico::PD_OPERADOR_Y)
			)
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en el parser(expr): 'son/sean iguales/diferentes"
					" expr y expr': se esperaba 'y'"
				);
			}

			if(!Tokens::EsNMemonico(op))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en el parser(expr): 'son/sean iguales/diferentes expr y expr'"
					": se esperaba KW- iguales/diferentes"
				);
			}

			if(op.ObtenerNMemonico() == pseudod::NMemonico::PD_OPERADOR_IGUALES)
			{
				return ((val1 == val2)? data->VERDADERO : data->FALSO);
			}
			else if(op.ObtenerNMemonico() == pseudod::NMemonico::PD_OPERADOR_DIFERENTES)
			{
				return ((val1 != val2)? data->VERDADERO : data->FALSO);
			}
			else
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en el parse(expr): 'son/sean iguales/diferentes'"
					": se esperaba iguales/diferentes"
				);
			}
		}

		if(proxy == pseudod::NMemonico::PD_OPERADOR_NO)
		{
			pseudod::Token pd;

			in >> pd;

			return (ValorDelToken(pd, in, data) == data->FALSO)?
				data->VERDADERO : data->FALSO;
		}

		throw PDvar::ErrorDelNucleo(
			"Error en el parser(expr): Token invalido"
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
