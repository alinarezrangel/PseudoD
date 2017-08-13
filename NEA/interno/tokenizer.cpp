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

#include "NEA/interno/tokenizer.hh"

namespace pseudod
{
	Tokenizador::Tokenizador(void)
		: tokens(0),
			itertk(tokens.begin()),
			tkpos(0),
			findelflujo(true)
	{}
	Tokenizador::Tokenizador(std::istream& stream)
		: tokens(0),
			itertk(tokens.begin()),
			tkpos(0),
			findelflujo(true)
	{
		this->TokenizarFlujo(stream, Tokenizador::Reemplazar);
	}
	Tokenizador::Tokenizador(const ListaTokens& tks)
		: tokens(tks),
			itertk(tokens.begin()),
			tkpos(0),
			findelflujo(tks.size() == 0)
	{}
	Tokenizador::Tokenizador(const Tokenizador& tk)
		: tokens(tk.tokens),
			itertk(IncrementaIteradorPor(tokens.begin(), tk.tkpos)),
			tkpos(tk.tkpos),
			findelflujo(tk.findelflujo)
	{}
	Tokenizador::Tokenizador(Tokenizador&& tk)
		: tokens(std::move(tk.tokens)),
			itertk(IncrementaIteradorPor(tokens.begin(), tk.tkpos)),
			tkpos(tk.tkpos),
			findelflujo(tk.findelflujo)
	{
		tk.tkpos = 0;
		tk.itertk = tk.tokens.begin();
	}
	Tokenizador::~Tokenizador(void)
	{}

	Tokenizador& Tokenizador::operator=(const Tokenizador& tk)
	{
		if(this != &tk)
		{
			this->tokens = tk.tokens;
			this->itertk = IncrementaIteradorPor(this->tokens.begin(), tk.tkpos);
			this->tkpos = tk.tkpos;
			this->findelflujo = tk.findelflujo;
		}

		return *this;
	}
	Tokenizador& Tokenizador::operator=(Tokenizador&& tk)
	{
		this->tokens = std::move(tk.tokens);
		this->itertk = IncrementaIteradorPor(this->tokens.begin(), tk.tkpos);
		this->tkpos = tk.tkpos;
		this->findelflujo = tk.findelflujo;

		tk.tkpos = 0;
		tk.itertk = tk.tokens.begin();

		return *this;
	}

	bool Tokenizador::TokenizarFlujo(
		std::istream& stream,
		Tokenizador::TipoTokenizacion tktype
	)
	{
		// Tokenizador de PseudoD 2.2.0:
		// Extraer **caracter por caracter** cada caracter del flujo.
		// Selecciona la sintáxis especial para literales
		// De no ser una literal, crea un token con el NMemonico
		// De ser una literal, crea un token con el texto (excluyendo delimitadores)
		// Si se encuentra el token PD_FUNCION, extrae la siguiente sintáxis:
		// -- FUNCION "nombre" [ CON "nombre" [ OPERADOR_Y "nombre" ]... ] "cuerpo" FIN_FUNCION
		// Recuerda que las funciones deben ser almacenadas como un Cuerpo de Código
		// y no como un flujo de tokens (por ahora).

		// [...] comentarios (eliminados, no se tokenizan a menos que esten en su
		// propia línea y comiencen con "[DOCUMENTA")
		// {...} «...» texto
		// <...> ... identificadores.

		PDCadena buffer = "";
		char rc = '\0';
		int lineno = 0;
		bool first_expr = true;
		bool funcion = false;
		bool clase = false;
		bool iniciotk = true;

		std::locale lc = std::locale();

		if(tktype == Tokenizador::Reemplazar)
		{
			this->tokens.clear();
		}

		auto parseBuffer = [&, this]()
		{
			if(
				(buffer.size() > 0) &&
				(buffer.front() != std::char_traits<char>::eof())
			)
			{
				NMemonicoProxy proxy;

				proxy = ConvertirCadenaANMemonico(buffer);

				if(!proxy.matched)
				{
					// No existe el nmemonico, puede ser un identificador

					Token::ValorLiteral vl;

					vl.valor = buffer;
					vl.tipo = Token::ValorLiteral::Identificador;

					this->tokens.push_back(Token(vl, Token::DatosFuente(lineno)));

					if(funcion)
					{
						funcion = false;

						NMemonicoProxy px = ConvertirCadenaANMemonico("");
						PDCadena lin = "", func = "";

						while(px != NMemonico::PD_FIN_FUNCION)
						{
							std::getline(stream, lin, '\n');

							func += lin + '\n';

							lin.erase(std::remove_if(lin.begin(),
								lin.end(),
								[](char x){return std::isspace(x);}),
								lin.end()
							);

							px = ConvertirCadenaANMemonico(lin);
						}

						Token::ValorLiteral vl;

						vl.valor = func;
						vl.tipo = Token::ValorLiteral::CuerpoDeCodigo;

						this->tokens.push_back(Token(vl, Token::DatosFuente(lineno)));
					}
				}
				else
				{
					this->tokens.push_back(Token(proxy, Token::DatosFuente(lineno)));

					if((proxy == NMemonico::PD_FUNCION) && (!funcion) && (!clase))
					{
						funcion = true;
					}

					if(proxy == NMemonico::PD_CLASE)
					{
						clase = true;
					}

					if(proxy == NMemonico::PD_FIN_CLASE)
					{
						clase = false;
					}

					if(proxy == NMemonico::PD_UTILIZAR)
					{
						// Leer la ruta al archivo o recurso

						std::string ruta = "";

						stream >> ruta;

						Token::ValorLiteral vl;

						vl.valor = ruta;
						vl.tipo = Token::ValorLiteral::Cadena;

						this->tokens.push_back(Token(vl, Token::DatosFuente(lineno)));
					}
				}

				buffer = "";
			}

			first_expr = false;

			if((rc == '\r') || (rc == '\n'))
			{
				lineno ++;
				first_expr = true;
			}

			iniciotk = true;
		};

		while(!stream.eof())
		{
			rc = stream.get();

			if(rc == std::char_traits<char>::eof())
			{
				// EOF en la lectura
				break;
			}

			if((rc == '\r') || (rc == '\n'))
			{
				lineno ++;
				first_expr = true;
			}

			// Agregar el carácter al buffer excepto si es un espacio en blanco.

			if(std::isspace<char>(rc, lc))
			{
				parseBuffer();

				continue;
			}

			if((rc == (char)0xC2) && iniciotk)
			{
				// Cadena angular «...» en UTF-8

				// El siguiente byte **debe** ser un 0xAB

				char cc = rc;

				rc = stream.get();

				if(rc != (char)0xAB)
				{
					/*
					throw PDvar::ErrorDeSintaxis(
						"Literal de texto de comillas angulares inválida: código UTF-8 no válido"
					);
					*/

					// Podriamos lanzar un error, pero esto eliminaria muchos codigos
					// UTF-8 validos, solo ignora silenciosamente el "error":
					buffer += cc;
					buffer += rc;

					continue;
				}

				buffer = "";

				while((rc = stream.get()))
				{
					if(rc == (char)0xC2)
					{
						char bf = stream.get();

						if(bf == (char)0xBB)
						{
							// Fin del literal
							break;
						}

						// Caracter UTF-8 similar

						buffer += rc;
						buffer += bf;

						continue;
					}

					buffer += rc;
				}

				Token::ValorLiteral vl;

				vl.valor = buffer;
				vl.tipo = Token::ValorLiteral::Cadena;

				this->tokens.push_back(Token(vl, Token::DatosFuente(lineno)));

				first_expr = false;

				buffer = "";
				iniciotk = true;

				continue;
			}

			if((rc == '{') && iniciotk)
			{
				// Cadena angular {...} en ASCII

				buffer = "";

				std::getline(stream, buffer, '}');

				Token::ValorLiteral vl;

				vl.valor = buffer;
				vl.tipo = Token::ValorLiteral::Cadena;

				this->tokens.push_back(Token(vl, Token::DatosFuente(lineno)));

				first_expr = false;

				buffer = "";
				iniciotk = true;

				continue;
			}

			if((rc == '[') && iniciotk)
			{
				buffer = "";

				std::getline(stream, buffer, ']');

				if(first_expr && (buffer.find("[DOCUMENTA") == 0))
				{
					// Recordar el comentario.

					Token::ValorLiteral vl;

					vl.valor = buffer;
					vl.tipo = Token::ValorLiteral::Comentario;

					this->tokens.push_back(Token(vl, Token::DatosFuente(lineno)));
				}

				first_expr = false;

				buffer = "";
				iniciotk = true;

				continue;
			}

			if(std::isdigit<char>(rc, lc) && iniciotk)
			{
				buffer = rc;

				while(stream)
				{
					rc = stream.get();

					if(
						std::isspace<char>(rc, lc) ||
						(rc == std::char_traits<char>::eof())
					)
					{
						break;
					}

					if((!std::isdigit<char>(rc, lc)) && (rc != '.'))
					{
						throw PDvar::ErrorDeSintaxis(
							"Literal numérica inválida: solo se permiten dígitos (0-9) y el "
							"separador decimal (.)"
						);
					}

					buffer += rc;
				}

				Token::ValorLiteral vl;

				vl.valor = buffer;
				vl.tipo = Token::ValorLiteral::Cadena;

				this->tokens.push_back(Token(vl, Token::DatosFuente(lineno)));

				first_expr = false;

				buffer = "";
				iniciotk = true;

				continue;
			}

			buffer += rc;
			iniciotk = false;
		}

		parseBuffer();

		this->itertk = this->tokens.begin();
		this->tkpos = 0;
		this->findelflujo = this->itertk == this->tokens.end();

		return true;
	}

	Tokenizador::ListaTokens& Tokenizador::ObtenerTokens(void)
	{
		return this->tokens;
	}

	Tokenizador::IteradorLT Tokenizador::ObtenerIterador(void)
	{
		return this->itertk;
	}
	Tokenizador::Posicion Tokenizador::ObtenerPosicion(void)
	{
		return this->tkpos;
	}

	bool Tokenizador::IgnorarHasta(Tokenizador::IteradorLT iter)
	{
		return this->FijarIteradorA(iter);
	}
	bool Tokenizador::FijarIteradorA(Tokenizador::IteradorLT iter)
	{
		Tokenizador::Posicion mtpos = 0;

		for(mtpos = 0; mtpos < this->tokens.size(); mtpos++)
		{
			if(IncrementaIteradorPor(this->tokens.begin(), mtpos) == iter)
				break;
		}

		if(mtpos >= this->tokens.size())
		{
			return false;
		}

		this->itertk = iter;
		this->tkpos = mtpos;

		return true;
	}
	bool Tokenizador::IgnorarHasta(Tokenizador::Posicion iter)
	{
		return this->FijarIteradorA(iter);
	}
	bool Tokenizador::FijarIteradorA(Tokenizador::Posicion iter)
	{
		if(iter >= this->tokens.size())
		{
			return false;
		}

		this->tkpos = iter;
		this->itertk = IncrementaIteradorPor(this->tokens.begin(), this->tkpos);

		return true;
	}

	Tokenizador::IteradorLT Tokenizador::BuscarToken(
		Tokenizador::IteradorLT iter,
		const Token& hasta,
		bool acumulable,
		const Token& acc
	)
	{
		int c = 0;

		for(;iter != this->tokens.end(); iter++)
		{
			if(acumulable && (*iter == acc))
			{
				c++;

				continue;
			}

			if(*iter == hasta)
			{
				if(c == 0)
					return iter;
				else
					c--;
			}
		}

		return iter;
	}
	Tokenizador::IteradorLT Tokenizador::BuscarToken(
		Tokenizador::IteradorLT iter,
		const Token& hasta1,
		const Token& hasta2,
		bool acumulable,
		const Token& acc
	)
	{
		int c = 0;

		for(;iter != this->tokens.end(); iter++)
		{
			if(acumulable && (*iter == acc))
			{
				c++;

				continue;
			}

			if(*iter == hasta1)
			{
				if(c == 0)
					return iter;

				continue;
			}

			if(*iter == hasta2)
			{
				if(c == 0)
					return iter;
				else
					c--;

				continue;
			}
		}

		return iter;
	}

	const Token& Tokenizador::ObtenerTokenActual(void)
	{
		return *this->itertk;
	}
	void Tokenizador::ExtraerToken(Token& out)
	{
		/*
		std::cout
			<< "ExtraerToken a "
			<< this->tkpos
			<< " de "
			<< this->tokens.size()
			<< std::endl;
		//*/

		if(this->itertk == this->tokens.end())
		{
			this->findelflujo = true;
			return;
		}

		out = *this->itertk;
		this->itertk++;
		this->tkpos++;
	}

	void Tokenizador::Borrar(void)
	{
		this->tokens.clear();
		this->itertk = this->tokens.begin();
		this->tkpos = 0;
		this->findelflujo = true;
	}

	bool Tokenizador::FinDelFlujo(void)
	{
		return this->findelflujo;
	}

	Tokenizador::operator bool(void)
	{
		return !this->FinDelFlujo();
	}

	Tokenizador& operator>>(Tokenizador& tk, Token& tok)
	{
		tk.ExtraerToken(tok);

		return tk;
	}
}
