/* El nuevo tokenizador de PseudoD.
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

#ifndef HDR_PSEUDOD_NEW_TOKENIZER_HH
#define HDR_PSEUDOD_NEW_TOKENIZER_HH 1

#include <tuple>
#include <functional>

#include "data.hh"
#include "nmemoic.hh"
#include "token.hh"

namespace pseudod
{
	class NuevoTokenizador final
	{
		public:
			explicit NuevoTokenizador(void);

			void ProducirComentarios(bool = true);
			bool ProduceComentarios(void) const;

			std::pair<std::vector<Token>, bool> LeerToken(std::istream&);

			Token::DatosFuente ObtenerLugarActual(void) const;
		private:
			char LeerCaracter(std::istream&);
			std::string LeerHasta(std::istream&, char, bool = true);
			std::string LeerHasta(std::istream&, std::string, bool = true);
			std::string LeerHasta(std::istream&, std::function<bool(const std::string&)>, bool = true);
			std::vector<Token> LeerMulticaracter(
				std::function<bool(char)>,
				std::function<Token(std::string)>,
				std::istream&,
				std::string
			);

			std::vector<Token> TokensParaCaracter(std::istream&, char);
			Token TokenCadena(const std::string&);
			Token TokenComentario(const std::string&);
			Token TokenOperador(std::istream& in, char);
			std::vector<Token> TokenNumeroUOperador(std::istream&, char);
			std::vector<Token> TokenIdentificador(std::istream&, std::string);

			Token::DatosFuente lugar;
			bool producirComentarios;
	};

	class Backtracker final
	{
		public:
			using Iterador = std::vector<Token>::const_iterator;

			struct RestauradorAutomatico
			{
				Iterador referencia;
				Backtracker& backtracker;
				bool activo;
				explicit RestauradorAutomatico(Backtracker&);
				~RestauradorAutomatico(void);
				void Desactivar(void);
			};

			explicit Backtracker(NuevoTokenizador = NuevoTokenizador());
			explicit Backtracker(
				std::vector<Token>,
				NuevoTokenizador = NuevoTokenizador()
			);

			Token LeerToken(std::istream&);
			Token LeerToken(void);
			Iterador ObtenerIteradorActual(void) const;
			void IrAIterador(Iterador);

			Iterador IteradorAlInicio(void) const;
			Iterador IteradorAlFinal(void) const;

			void TokenizarFlujo(std::istream&);

			bool FinDelPrograma(void) const;
			bool CasiFinDelPrograma(void) const;

			RestauradorAutomatico CrearRestaurador(void);

			Token::DatosFuente ObtenerLugarActual(void) const;
		private:
			std::vector<Token> tokens;
			std::vector<Token>::size_type indice;
			NuevoTokenizador tokenizador;
	};
}

#endif /* ~HDR_PSEUDOD_NEW_TOKENIZER_HH */
