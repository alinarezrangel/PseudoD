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

#ifndef HDR_PSEUDOD_NEA_TOKEN_HH
#define HDR_PSEUDOD_NEA_TOKEN_HH 1

#include <iostream>
#include <memory>
#include <utility>

#include "NEA/interno/data.hh"
#include "NEA/interno/nmemoic.hh"

/**
* @file token.h
* Posee la implementación de tokens.
*/

namespace pseudod
{
	/**
	* @brief La clase token.
	* Representa un token. Un token es la unidad minima necesaria para la
	* tokenizacion, proceso que convierte una entrada literal a tokens.
	*/
	class Token : public PDvar::PDObjeto
	{
		public:
			/**
			* @brief Representa un valor que no es un nmemonico.
			* Básicamente puede ser una cadena ({}), un número, un identificador
			* (var1), un comentario ([]) o un cuerpo de código (funcion ... finfun)
			*/
			struct ValorLiteral
			{
				enum TipoValor
				{
					Cadena,
					Numero,
					Comentario,
					Identificador,
					CuerpoDeCodigo
				};

				PDCadena valor;
				TipoValor tipo;

				bool operator==(const ValorLiteral&) const;
				bool operator!=(const ValorLiteral&) const;
			};

			/**
			* @brief Contiene los datos acerca de la fuente del token.
			*/
			struct DatosFuente
			{
				DatosFuente(int linea = 1, int columna = 1, std::string archivo = "");

				int linea;
				int columna;
				std::string nombreDelArchivo;
			};

			/**
			* @brief Indica el tipo del token.
			* Algunas operaciones solo funciones sobre ciertos tipos, de no ser
			* el tipo de la operación igual al tipo del token, un error será lanzado.
			*/
			enum TipoToken
			{
				SinTipo = 0,
				NMemonico = 1,
				Literal = 2
			};

			/**
			* @brief Representa un puntero a un NMemonico.
			* El puntero es inteligente, lo que significa que no es necesario
			* destruirlo o preocuparse por el manejo de la memoria.
			*/
			typedef std::shared_ptr<NMemonicoProxy> PunteroNMemonicoProxy;
			/**
			* @brief Representa un puntero a un ValorLiteral.
			* El puntero es inteligente, lo que significa que no es necesario
			* destruirlo o preocuparse por el manejo de la memoria.
			*/
			typedef std::shared_ptr<ValorLiteral> PunteroValorLiteral;

			/**
			* @brief Construye un token.
			* El token no posee tipo ni valor.
			*/
			Token(DatosFuente fn = DatosFuente());
			/**
			* @brief Construye un token.
			* El token posee el tipo NMemonico y el valor contiene al nmemonico dado.
			*
			* @param nm NMemonico a almacenar como valor.
			*/
			Token(NMemonicoProxy nm, DatosFuente fn = DatosFuente());
			/**
			* @brief Construye un token.
			* Sobrecarga de utilidad.
			*
			* Equivalente a `Token(ConvertirNMemonicoAProxy(nm))`.
			*
			* @param nm NMemonico a almacenar como valor.
			*/
			Token(pseudod::NMemonico nm, DatosFuente fn = DatosFuente());
			/**
			* @brief Construye un token.
			* Sobrecarga de utilidad.
			*
			* Equivalente a `Token(ConvertirPalabraAProxy(nm))`.
			*
			* @param nm NMemonico a almacenar como valor.
			*/
			Token(pseudod::NMemonico::Palabra nm, DatosFuente fn = DatosFuente());
			/**
			* @brief Construye un token.
			* El token posee el tipo Literal y el valor contiene al ValorLiteral dado.
			*
			* @param lit ValorLiteral a almacenar como valor.
			*/
			Token(ValorLiteral lit, DatosFuente fn = DatosFuente());
			/**
			* @brief Copia un token.
			* Este token será una copia del token utilizado.
			*
			* @param tok Token a copiar en este.
			*/
			Token(const Token& tok);
			/**
			* @brief Mueve un token.
			* Este token será igual al token dado, y el parámetro quedará indefinido.
			*
			* @param tok Token a mover. Luego de moverlo, quedará sin tipo y valor.
			*/
			Token(Token&& tok);
			/**
			* @brief Destruye el token actual.
			*/
			virtual ~Token(void);

			/**
			* @brief Copia un token dado.
			*
			* @param tok Token a copiar.
			* @return El token actual.
			*/
			Token& operator=(const Token& tok);
			/**
			* @brief Mueve un token dado.
			*
			* @param tok Token a mover.
			* @return El token actual.
			*/
			Token& operator=(Token&& tok);

			/**
			* @brief Determina si este token y tok son iguales.
			* @return true si son iguales, false de lo contrario.
			*/
			bool operator==(const Token& tok);
			/**
			* @brief Determina si este token y tok son distintos.
			* @return true si son distintos, false de lo contrario.
			*/
			bool operator!=(const Token& tok);

			/**
			* @brief Obtiene el tipo del token.
			*
			* @return El tipo del token.
			*/
			TipoToken ObtenerTipo(void) const;
			/**
			* @brief Obtiene el NMemonico del token.
			* Si el tipo no es NMemonico, lanzará un error.
			*
			* @return NMemonico del token.
			*/
			const NMemonicoProxy& ObtenerNMemonico(void) const;
			/**
			* @brief Obtiene el ValorLiteral del token.
			* Si el tipo no es ValorLiteral, lanzará un error.
			*
			* @return ValorLiteral del token.
			*/
			const ValorLiteral& ObtenerValorLiteral(void) const;

			/**
			* @brief Obtiene los datos acerca de la fuente del token.
			*
			* @return Los datos de la fuente.
			*/
			const DatosFuente& ObtenerDatosFuente(void) const;
		private:
			TipoToken tipo;
			PunteroNMemonicoProxy nmemonico;
			PunteroValorLiteral valorliteral;
			DatosFuente fuente;
	};

	Token CrearTokenDesde(const NMemonicoProxy& px);
	Token CrearTokenDesde(const Token::ValorLiteral& vl);

	// Funciones de utilidad:

	namespace Utilidades
	{
		namespace Tokens
		{
			bool EsNMemonico(const Token& tk);
			bool EsLiteral(const Token& tk);
			bool EsIdentificador(const Token& tk);
			bool EsCadena(const Token& tk);
			bool EsNumero(const Token& tk);
			bool EsCuerpoDeCodigo(const Token& tk);
			bool EsComentario(const Token& tk);

			const PDCadena& ObtenerValor(const Token& tk);
		}
	}
}

#endif /* ~HDR_PSEUDOD_NEA_TOKEN_HH */
