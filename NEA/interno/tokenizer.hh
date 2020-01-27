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

#ifndef HDR_PSEUDOD_NIA_TOKENIZER_HH
#define HDR_PSEUDOD_NIA_TOKENIZER_HH 1

#include <iostream>
#include <list>
#include <utility>
#include <locale>
#include <iterator>
#include <type_traits>

#include <cctype>

#include "NEA/interno/data.hh"
#include "NEA/interno/token.hh"

namespace pseudod
{
	class Tokenizador : public PDvar::PDObjeto
	{
		public:
			typedef std::vector<Token> ListaTokens;
			typedef typename ListaTokens::iterator IteradorLT;
			typedef std::size_t Posicion;

			enum TipoTokenizacion
			{
				Agregar,
				Reemplazar
			};

			explicit Tokenizador(void);
			explicit Tokenizador(
				std::istream& stream,
				bool usarCuerpoDeCodigo = true,
				bool producirNumeros = false
			);
			explicit Tokenizador(const ListaTokens& tokens);
			explicit Tokenizador(ListaTokens&& tokens);
			explicit Tokenizador(const Tokenizador& tk);
			explicit Tokenizador(Tokenizador&& tk);
			virtual ~Tokenizador(void);

			Tokenizador& operator=(const Tokenizador& tk);
			Tokenizador& operator=(Tokenizador&& tk);

			bool TokenizarFlujo(std::istream& stream, TipoTokenizacion tktype);
			ListaTokens& ObtenerTokens(void);

			void UsarCuerpoDeCodigo(bool usar = true);
			bool UsandoCuerpoDeCodigo(void) const;

			void ProducirNumeros(bool producir = true);
			bool ProduciendoNumeros(void) const;

			IteradorLT ObtenerIterador(void);
			Posicion ObtenerPosicion(void);

			bool IgnorarHasta(IteradorLT iter);
			bool FijarIteradorA(IteradorLT iter);
			bool IgnorarHasta(Posicion iter);
			bool FijarIteradorA(Posicion iter);

			IteradorLT BuscarToken(
				IteradorLT iter,
				const Token& hasta,
				bool acumulable,
				const Token& acc
			);
			IteradorLT BuscarToken(
				IteradorLT iter,
				const Token& hasta1,
				const Token& hasta2,
				bool acumulable,
				const Token& acc
			);

			const Token& ObtenerTokenActual(void);
			void ExtraerToken(Token& out);

			void Borrar(void);

			bool FinDelFlujo(bool forzar = false);

			explicit operator bool(void);
		private:
			ListaTokens tokens;
			IteradorLT itertk;
			Posicion tkpos;
			bool findelflujo;
			bool usarCuerpoDeCodigo;
			bool producirNumeros;
			bool separarOperadores;
	};

	template<
		class ForwardIterator,
		class DistanceType,
		std::enable_if_t<
			std::is_base_of<
				std::forward_iterator_tag,
				typename std::iterator_traits<ForwardIterator>::iterator_category
			>::value &&
			!std::is_base_of<
				std::random_access_iterator_tag,
				typename std::iterator_traits<ForwardIterator>::iterator_category
			>::value,
			int
		> = 0
	>
	ForwardIterator IncrementaIteradorPor(
		ForwardIterator i,
		DistanceType n
	)
	{
		for(DistanceType j = 0; j < n; j++)
		{
			i ++;
		}

		return i;
	}

	template<
		class RandomIterator,
		class DistanceType,
		std::enable_if_t<
			std::is_base_of<
				std::random_access_iterator_tag,
				typename std::iterator_traits<RandomIterator>::iterator_category
			>::value,
			int
		> = 0
	>
	RandomIterator IncrementaIteradorPor(
		RandomIterator i,
		DistanceType n
	)
	{
		return i + n;
	}

	Tokenizador& operator>>(Tokenizador& tk, Token& tok);

	using TokenizadorPtr = std::shared_ptr<Tokenizador>;
}

#endif /* ~HDR_PSEUDOD_NIA_TOKENIZER_HH */
