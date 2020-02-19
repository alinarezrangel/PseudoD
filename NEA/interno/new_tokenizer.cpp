/* Nuevo tokenizador.
 *
 * Copyright 2016 Alejandro Linarez Rangel
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cctype>
#include <cassert>
#include <regex>
#include <iostream>

#include "new_tokenizer.hh"

namespace pseudod
{
	namespace
	{
		bool EsEOF(char c)
		{
			return c == std::char_traits<char>::eof();
		}

		void AsertarNoEOF(char c)
		{
			if(EsEOF(c))
			{
				throw PDvar::ErrorDeSintaxis(
					"Fin del archivo inesperado mientras se leía el programa"
				);
			}
		}

		void EscribirTokens(const std::vector<Token>& tokens)
		{
			namespace TokenUtils = Utilidades::Tokens;
			std::cout << "TOK : ";
			for(const auto& tk : tokens)
			{
				if(TokenUtils::EsCadena(tk))
				{
					std::cout << "Cadena {" << TokenUtils::ObtenerValor(tk) << "}";
				}
				if(TokenUtils::EsIdentificador(tk))
				{
					std::cout << "Id. " << TokenUtils::ObtenerValor(tk);
				}
				if(TokenUtils::EsNumero(tk))
				{
					std::cout << "Numero " << TokenUtils::ObtenerValor(tk);
				}
				if(TokenUtils::EsCuerpoDeCodigo(tk))
				{
					std::cout << "CuerpoDeCodigo [COMIENZA\n" << TokenUtils::ObtenerValor(tk) << "\nTERMINA]";
				}
				if(TokenUtils::EsComentario(tk))
				{
					std::cout << "Comentario [" << TokenUtils::ObtenerValor(tk) << "]";
				}
				if(TokenUtils::EsNMemonico(tk))
				{
					std::cout << "NMemonico " << tk.ObtenerNMemonico().original;
				}
				std::cout << "    ";
			}
			std::cout << std::endl;
		}
	}

	NuevoTokenizador::NuevoTokenizador(void) : lugar(), producirComentarios(false)
	{}

	void NuevoTokenizador::ProducirComentarios(bool prod)
	{
		this->producirComentarios = prod;
	}

	bool NuevoTokenizador::ProduceComentarios(void) const
	{
		return this->producirComentarios;
	}

	std::pair<std::vector<Token>, bool> NuevoTokenizador::LeerToken(std::istream& in)
	{
		char c = this->LeerCaracter(in);
		if(EsEOF(c))
		{
			return std::make_pair(std::vector<Token>(), false);
		}
		else
		{
			return std::make_pair(this->TokensParaCaracter(in, c), true);
		}
	}

	Token::DatosFuente NuevoTokenizador::ObtenerLugarActual(void) const
	{
		return this->lugar;
	}

	char NuevoTokenizador::LeerCaracter(std::istream& in)
	{
		char c = in.get();
		if(c == '\n')
		{
			this->lugar.linea++;
		}
		return c;
	}

	std::string NuevoTokenizador::LeerHasta(std::istream& in, char end, bool incluirFinal)
	{
		return this->LeerHasta(
			in,
			[end](const std::string& s) { return s.back() == end; },
			incluirFinal
		);
	}

	std::string NuevoTokenizador::LeerHasta(std::istream& in, std::string end, bool incluirFinal)
	{
		return this->LeerHasta(
			in,
			[end](const std::string& resultado)
			{
				if(end.size() > resultado.size())
				{
					return false;
				}
				return resultado.substr(resultado.size() - end.size(), end.size()) == end;
			},
			incluirFinal
		);
	}

	std::string NuevoTokenizador::LeerHasta(
		std::istream& in,
		std::function<bool(const std::string&)> pred,
		bool incluirFinal
	)
	{
		std::string resultado;
		while(in)
		{
			char c = this->LeerCaracter(in);
			AsertarNoEOF(c);
			if(incluirFinal)
			{
				resultado += c;
				if(pred(resultado))
				{
					return resultado;
				}
			}
			else
			{
				if(pred(resultado + c))
				{
					return resultado;
				}
				resultado += c;
			}
		}
		throw PDvar::ErrorDeSintaxis(
			"Fin del achivo inesperado."
		);
	}

	std::vector<Token> NuevoTokenizador::TokensParaCaracter(std::istream& in, char c)
	{
		AsertarNoEOF(c);

		while(std::isspace(c))
		{
			c = this->LeerCaracter(in);
			if(EsEOF(c))
			{
				return std::vector<Token>();
			}
		}

		switch(c)
		{
			case '{':
				{
					std::string texto = this->LeerHasta(in, '}', false);
					return std::vector<Token> {this->TokenCadena(texto)};
				}
			case '\\':
				{
					std::string texto = this->LeerHasta(in, '\\');
					return std::vector<Token> {Token(
						Token::ValorLiteral {
							texto.substr(0, texto.size() - 1),
							Token::ValorLiteral::Identificador
						},
						this->lugar
					)};
				}
			case '[':
				if(this->producirComentarios)
				{
					std::string texto = this->LeerHasta(in, ']', false);
					return std::vector<Token> {this->TokenComentario(texto)};
				}
				else
				{
					this->LeerHasta(in, ']');
					return std::vector<Token>();
				}
			case '(': case ')': case '#': case '%':
			case '.': case ':': case ',': case '&':
				return std::vector<Token> {this->TokenOperador(c)};
			case '-':
				return this->TokenNumero(in, c);
			default:
				if(std::isdigit(c))
				{
					return this->TokenNumero(in, c);
				}
				else if(c != '\xC2')
				{
					return this->TokenIdentificador(in, std::string(1, c));
				}
				else
				{
					char c2 = this->LeerCaracter(in);
					AsertarNoEOF(c2);

					if(c == '\xC2' && c2 == '\xAB')
					{
						std::string texto = this->LeerHasta(in, "\xC2\xBB");
						texto = texto.substr(0, texto.size() - 2);
						return std::vector<Token> {this->TokenCadena(texto)};
					}
					else
					{
						return this->TokenIdentificador(in, std::string {c, c2});
					}
				}
		}

		assert(0);
	}

	Token NuevoTokenizador::TokenCadena(const std::string& texto)
	{
		return Token(
			Token::ValorLiteral {texto, Token::ValorLiteral::Cadena},
			this->lugar
		);
	}

	Token NuevoTokenizador::TokenComentario(const std::string& texto)
	{
		return Token(
			Token::ValorLiteral {texto, Token::ValorLiteral::Comentario},
			this->lugar
		);
	}

	Token NuevoTokenizador::TokenOperador(char op)
	{
		return Token(ConvertirCadenaANMemonico(std::string(1, op)), this->lugar);
	}

	std::vector<Token> NuevoTokenizador::LeerMulticaracter(
		std::function<bool(char)> continuar,
		std::function<Token(std::string)> finalizar,
		std::istream& in,
		std::string inicio
	)
	{
		std::string acumulador = inicio;
		while(in)
		{
			char c = this->LeerCaracter(in);
			if(EsEOF(c))
			{
				return std::vector<Token> { finalizar(acumulador) };
			}

			if(continuar(c))
			{
				acumulador += c;
			}
			else
			{
				std::vector<Token> tk = this->TokensParaCaracter(in, c);
				tk.insert(tk.begin(), finalizar(acumulador));
				return tk;
			}
		}
		return std::vector<Token> { finalizar(acumulador) };
	}

	std::vector<Token> NuevoTokenizador::TokenNumero(std::istream& in, char primerCar)
	{
		return LeerMulticaracter(
			[](char c) -> bool
			{
				switch(c)
				{
					case '(': case ')': case '#':
					case ':': case ',': case '&':
					case '%':
						return false;
					default:
						return std::isdigit(c) || c == '.';
				}
			},
			[this](std::string buff) -> Token
			{
				std::regex patronNumero("-?[0-9]+(\\.[0-9]+)?");
				std::smatch res;

				if(!std::regex_match(buff, res, patronNumero))
				{
					throw PDvar::ErrorDeSintaxis(
						"Literal numérica inválida: " + buff
					);
				}

				return Token(
					Token::ValorLiteral {
						buff,
						Token::ValorLiteral::Numero
					},
					this->lugar
				);
			},
			in,
			std::string(1, primerCar)
		);
	}

	std::vector<Token> NuevoTokenizador::TokenIdentificador(std::istream& in, std::string inicio)
	{
		return LeerMulticaracter(
			[](char c) -> bool
			{
				switch(c)
				{
					case '(': case ')': case '#': case '.':
					case ':': case ',': case '&': case '%':
						return false;
					default:
						return !std::isspace(c);
				}
			},
			[this](std::string buff) -> Token
			{
				auto proxy = ConvertirCadenaANMemonico(buff);
				if(proxy.matched)
				{
					return Token(proxy, this->lugar);
				}
				else
				{
					return Token(
						Token::ValorLiteral {
							buff,
							Token::ValorLiteral::Identificador
						},
						this->lugar
					);
				}
			},
			in,
			inicio
		);
	}

	Backtracker::RestauradorAutomatico::RestauradorAutomatico(Backtracker& b)
		: referencia(b.ObtenerIteradorActual()), backtracker(b), activo(true)
	{}

	Backtracker::RestauradorAutomatico::~RestauradorAutomatico(void)
	{
		if(this->activo)
		{
			this->backtracker.IrAIterador(this->referencia);
		}
	}

	void Backtracker::RestauradorAutomatico::Desactivar(void)
	{
		this->activo = false;
	}

	Backtracker::Backtracker(NuevoTokenizador tok)
		: tokens(), indice(0), tokenizador(tok)
	{}

	Backtracker::Backtracker(std::vector<Token> tokens, NuevoTokenizador tok)
		: tokens(tokens), indice(0), tokenizador(tok)
	{}

	Token Backtracker::LeerToken(std::istream& in)
	{
		if(this->FinDelPrograma())
		{
			while(true)
			{
				if(!in)
				{
					return Token();
				}
				auto tokens = this->tokenizador.LeerToken(in);
				if(!tokens.second)
				{
					return Token();
				}
				else if(!tokens.first.empty())
				{
					this->tokens.insert(
						this->tokens.end(),
						tokens.first.begin(),
						tokens.first.end()
					);
					break;
				}
			}
		}
		return this->tokens.at(this->indice++);
	}

	Token Backtracker::LeerToken(void)
	{
		if(this->FinDelPrograma())
		{
			return Token();
		}
		else
		{
			return this->tokens.at(this->indice++);
		}
	}

	Backtracker::Iterador Backtracker::ObtenerIteradorActual(void) const
	{
		return this->tokens.begin() + this->indice;
	}

	void Backtracker::IrAIterador(Backtracker::Iterador ref)
	{
		this->indice = ref - this->tokens.begin();
	}

	Backtracker::Iterador Backtracker::IteradorAlInicio(void) const
	{
		return this->tokens.begin();
	}

	Backtracker::Iterador Backtracker::IteradorAlFinal(void) const
	{
		return this->tokens.end();
	}

	void Backtracker::TokenizarFlujo(std::istream& in)
	{
		auto indiceAnterior = this->indice;
		while(true)
		{
			Token tk = this->LeerToken(in);
			if(tk.ObtenerTipo() == Token::SinTipo)
			{
				break;
			}
		}
		this->indice = indiceAnterior;
	}

	bool Backtracker::FinDelPrograma(void) const
	{
		return this->indice >= this->tokens.size();
	}

	bool Backtracker::CasiFinDelPrograma(void) const
	{
		return this->indice == (this->tokens.size() - 1);
	}

	Backtracker::RestauradorAutomatico Backtracker::CrearRestaurador(void)
	{
		return Backtracker::RestauradorAutomatico(*this);
	}

	Token::DatosFuente Backtracker::ObtenerLugarActual(void) const
	{
		return this->tokenizador.ObtenerLugarActual();
	}
}
