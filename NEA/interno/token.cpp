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

#include "NEA/interno/token.hh"

#include <iostream>

namespace pseudod
{
	bool Token::ValorLiteral::operator==(const Token::ValorLiteral& x) const
	{
		return this->valor == x.valor && this->tipo == x.tipo;
	}

	bool Token::ValorLiteral::operator!=(const Token::ValorLiteral& x) const
	{
		return !((*this) == x);
	}

	Token::DatosFuente::DatosFuente(int linea)
		: linea(linea)
	{}

	Token::Token(Token::DatosFuente fn)
		: PDvar::PDObjeto(),
			tipo(Token::SinTipo),
			nmemonico(nullptr),
			valorliteral(nullptr),
			fuente(fn)
	{}

	Token::Token(NMemonicoProxy nm, Token::DatosFuente fn)
		: PDvar::PDObjeto(),
			tipo(Token::NMemonico),
			nmemonico(std::make_shared<NMemonicoProxy>(nm)),
			valorliteral(nullptr),
			fuente(fn)
	{}

	Token::Token(pseudod::NMemonico nm, Token::DatosFuente fn)
		: Token(ConvertirNMemonicoAProxy(nm), fn)
	{}

	Token::Token(pseudod::NMemonico::Palabra nm, Token::DatosFuente fn)
		: Token(ConvertirPalabraAProxy(nm), fn)
	{}

	Token::Token(ValorLiteral lit, Token::DatosFuente fn)
		: PDvar::PDObjeto(),
			tipo(Token::Literal),
			nmemonico(nullptr),
			valorliteral(std::make_shared<Token::ValorLiteral>(lit)),
			fuente(fn)
	{}

	Token::Token(const Token& tok)
		: PDvar::PDObjeto(),
			tipo(tok.tipo),
			nmemonico(tok.nmemonico),
			valorliteral(tok.valorliteral),
			fuente(tok.fuente)
	{}

	Token::Token(Token&& tok)
		: PDvar::PDObjeto(),
			tipo(tok.tipo),
			nmemonico(std::move(tok.nmemonico)),
			valorliteral(std::move(tok.valorliteral)),
			fuente(std::move(tok.fuente))
	{
		tok.tipo = Token::SinTipo;
	}

	Token::~Token(void)
	{
		/*
		* Gracias a que usamos std::shared_ptr (punteros inteligentes) no
		* necesitamos liberar nada.
		*/
	}

	Token& Token::operator=(const Token& tok)
	{
		if(&tok != this)
		{
			this->tipo = tok.tipo;

			this->nmemonico = tok.nmemonico;
			this->valorliteral = tok.valorliteral;
			this->fuente = tok.fuente;
		}

		return *this;
	}
	Token& Token::operator=(Token&& tok)
	{
		if(&tok == this)
			return *this;

		this->tipo = tok.tipo;
		tok.tipo = Token::SinTipo;
		this->nmemonico = std::move(tok.nmemonico);
		this->valorliteral = std::move(tok.valorliteral);
		this->fuente = std::move(tok.fuente);

		return *this;
	}

	bool Token::operator==(const Token& tok)
	{
		if(this->ObtenerTipo() != tok.ObtenerTipo())
			return false;

		switch(this->ObtenerTipo())
		{
			case Token::SinTipo:
				return true;
			case Token::NMemonico:
				return this->ObtenerNMemonico() == tok.ObtenerNMemonico();
			case Token::Literal:
				return this->ObtenerValorLiteral().valor == tok.ObtenerValorLiteral().valor;
		}

		return false;
	}
	bool Token::operator!=(const Token& tok)
	{
		return !(*this == tok);
	}

	Token::TipoToken Token::ObtenerTipo(void) const
	{
		return this->tipo;
	}

	const NMemonicoProxy& Token::ObtenerNMemonico(void) const
	{
		if(!this->nmemonico)
		{
			throw PDvar::ErrorDelNucleo(
				"Error en Token::ObtenerNMemonico: token no es un nmemonico"
			);
		}

		return *this->nmemonico;
	}

	const Token::ValorLiteral& Token::ObtenerValorLiteral(void) const
	{
		if(!this->valorliteral)
		{
			throw PDvar::ErrorDelNucleo(
				"Error en Token::ObtenerValorLiteral: token no es un valor literal"
			);
		}

		return *this->valorliteral;
	}

	const Token::DatosFuente& Token::ObtenerDatosFuente(void) const
	{
		return this->fuente;
	}

	Token CrearTokenDesde(const NMemonicoProxy& px)
	{
		return Token(px);
	}

	Token CrearTokenDesde(const Token::ValorLiteral& vl)
	{
		return Token(vl);
	}

	// Funciones de utilidad:

	namespace Utilidades
	{
		namespace Tokens
		{
			bool EsNMemonico(const Token& tk)
			{
				return tk.ObtenerTipo() == Token::NMemonico;
			}
			bool EsLiteral(const Token& tk)
			{
				return tk.ObtenerTipo() == Token::Literal;
			}
			bool EsIdentificador(const Token& tk)
			{
				return (tk.ObtenerTipo() == Token::Literal)
					&& (tk.ObtenerValorLiteral().tipo == Token::ValorLiteral::Identificador);
			}
			bool EsCadena(const Token& tk)
			{
				return (tk.ObtenerTipo() == Token::Literal)
					&& (tk.ObtenerValorLiteral().tipo == Token::ValorLiteral::Cadena);
			}
			bool EsNumero(const Token& tk)
			{
				return (tk.ObtenerTipo() == Token::Literal)
					&& (tk.ObtenerValorLiteral().tipo == Token::ValorLiteral::Numero);
			}
			bool EsComentario(const Token& tk)
			{
				return (tk.ObtenerTipo() == Token::Literal)
					&& (tk.ObtenerValorLiteral().tipo == Token::ValorLiteral::Comentario);
			}
			bool EsCuerpoDeCodigo(const Token& tk)
			{
				return (tk.ObtenerTipo() == Token::Literal)
					&& (tk.ObtenerValorLiteral().tipo == Token::ValorLiteral::CuerpoDeCodigo);
			}

			const PDCadena& ObtenerValor(const Token& tk)
			{
				return tk.ObtenerValorLiteral().valor;
			}
		}
	}
}
