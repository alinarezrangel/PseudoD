/* Nuevo intérprete.
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

#ifndef HDR_PSEUDOD_NEW_INTERPRETE_HH
#define HDR_PSEUDOD_NEW_INTERPRETE_HH

#include <memory>
#include <utility>

#include "data.hh"
#include "memory.hh"
#include "token.hh"
#include "tokenizer.hh"
#include "new_tokenizer.hh"

#ifndef PSEUDOD_CALLBACK_ALIASES
#define PSEUDOD_CALLBACK_ALIASES
typedef void (*PDFuncionNEA)(pseudod::Token, pseudod::Tokenizador&);
typedef void (*PDFuncionNIA)(pseudod::Token, pseudod::Tokenizador&, PDFuncionNEA);
#endif

namespace pseudod
{
	class Procedimiento;
	using ProcedimientoPtr = std::shared_ptr<Procedimiento>;
	struct ParametroProcedimiento
	{
		std::string nombre = "";
		bool variadic = false;
	};

	class ManejadorDeModulos;
	using ManejadorDeModulosPtr = std::shared_ptr<ManejadorDeModulos>;

	struct ConfInterprete
	{
		ValorPtr ClaseObjeto = nullptr;
		bool ConvertirTextoABoole = false;
		AmbitoPtr AmbitoBase = nullptr;
	};

	class Interprete
	{
		public:
			explicit Interprete(
				ConfInterprete,
				AmbitoPtr = std::make_shared<Ambito>(),
				ManejadorDeModulosPtr = std::make_shared<ManejadorDeModulos>()
			);

			const ConfInterprete& ObtenerConfiguracion(void) const;
			ConfInterprete& ObtenerConfiguracion(void);

			AmbitoPtr ObtenerAmbito(void) const;
			ManejadorDeModulosPtr ObtenerManejadorDeModulos(void) const;

			Interprete CrearSubinterprete(void);

			void Ejecutar(Backtracker&);
			void Ejecutar(
				const std::vector<Token>&,
				Token::DatosFuente = Token::DatosFuente()
			);
			void Ejecutar(
				Backtracker::Iterador,
				Backtracker::Iterador,
				Token::DatosFuente = Token::DatosFuente()
			);

			ValorPtr Evaluar(Backtracker&);
			ValorPtr Evaluar(
				const std::vector<Token>&,
				Token::DatosFuente = Token::DatosFuente()
			);
			ValorPtr Evaluar(
				Backtracker::Iterador,
				Backtracker::Iterador,
				Token::DatosFuente = Token::DatosFuente()
			);

			bool EsVerdadero(ValorPtr);
		protected:
			void EjecutarSiguiente(Backtracker&);
			ValorPtr EvaluarSiguiente(Backtracker&);
		private:
			void LanzaErrorDeSintaxis(Token::DatosFuente, const std::string&);
			void LanzaErrorDeSintaxis(const Token&, const std::string&);
			void FallaSiFinDelFlujo(Backtracker&);
			Token LeerToken(Backtracker&);
			Token EsperarIgual(Backtracker&, NMemonico::Palabra);
			Token EsperarIgual(Backtracker&, Token::ValorLiteral);
			Token EsperarIgual(Backtracker&, Token::ValorLiteral::TipoValor);
			Token LeerNMemonico(Backtracker&);
			Token SiguienteToken(Backtracker&);
			bool SiguienteTokenEs(Backtracker&, NMemonico::Palabra);
			bool SiguienteTokenEs(Backtracker&, Token::ValorLiteral);
			bool SiguienteTokenEs(Backtracker&, Token::ValorLiteral::TipoValor);

			bool LeerTokenProcedimiento(Backtracker&);

			ValorPtr LeerYEnviarMensajes(Backtracker&, ValorPtr);

			void EjecutarAdquirir(Backtracker&);
			void EjecutarInstancia(Backtracker&);
			void EjecutarLiberar(Backtracker&);
			void EjecutarFijar(Backtracker&);
			void EjecutarEscribir(Backtracker&);
			void EjecutarNl(Backtracker&);
			void EjecutarLeer(Backtracker&);
			void EjecutarSi(Backtracker&);
			void EjecutarMientras(Backtracker&);
			void EjecutarProcedimiento(Backtracker&);
			void EjecutarDevolver(Backtracker&);
			void EjecutarUtilizar(Backtracker&);
			void EjecutarClase(Backtracker&);
			void EjecutarNecesitas(Backtracker&);
			void EjecutarAtributo(Backtracker&);

			ValorPtr EvaluarVariable(Backtracker&);
			ValorPtr EvaluarParentesis(Backtracker&);
			ValorPtr EvaluarLiteral(Backtracker&);
			ValorPtr EvaluarSon(Backtracker&);
			ValorPtr EvaluarNo(Backtracker&);
			ValorPtr EvaluarLlamar(Backtracker&);
			ValorPtr EvaluarProcedimiento(Backtracker&);
			ValorPtr EvaluarReferenciaVariable(Backtracker&);
			ValorPtr EvaluarAutoejecucionForzada(Backtracker&);
			ValorPtr EvaluarClonar(Backtracker&);

			ValorPtr LlamaConParametros(Backtracker&, ValorPtr);

			std::vector<ParametroProcedimiento>
			LeerParametrosProcedimiento(Backtracker&, bool);

			std::vector<ValorPtr> LeerArgumentosProcedimiento(Backtracker&);

			std::vector<Token> LeerCuerpoDeProcedimiento(Backtracker&);

			ValorPtr ObtenerVariable(PDCadena, Token::DatosFuente);

			AmbitoPtr ambito;
			ConfInterprete conf;
			ManejadorDeModulosPtr manejadorDeModulos;
	};

	class Procedimiento final : public Valor
	{
		public:
			explicit Procedimiento(
				Interprete,
				const std::vector<ParametroProcedimiento>&,
				const std::pair<ParametroProcedimiento, bool>,
				const std::vector<Token>&
			);
			explicit Procedimiento(
				Interprete,
				const std::vector<ParametroProcedimiento>&,
				const std::vector<Token>&
			);
			virtual ~Procedimiento(void);

			virtual ValorPtr RecibirMensaje(
				std::string,
				const std::vector<ValorPtr>&
			) override;

			const std::vector<ParametroProcedimiento>
			ObtenerParametrosFijos(void) const;
			const std::pair<ParametroProcedimiento, bool>
			ObtenerParametroVariadic(void) const;
			const std::vector<Token>& ObtenerTokens(void) const;
			Interprete ObtenerInterprete(void) const;

			virtual ValorPtr Ejecutar(const std::vector<ValorPtr>&);
		private:
			std::vector<ParametroProcedimiento> parametrosFijos;
			ParametroProcedimiento parametroVariadic;
			bool tieneParametroVariadic;
			std::vector<Token> cuerpo;
			Interprete interprete;
	};

	struct ConfManejadorDeModulos
	{
		std::vector<std::string> rutasABuscar = {};
	};

	class ManejadorDeModulos final
	{
		public:
			explicit ManejadorDeModulos(
				ConfManejadorDeModulos = ConfManejadorDeModulos()
			);

			const ConfManejadorDeModulos& ObtenerConfiguracion(void) const;
			ConfManejadorDeModulos& ObtenerConfiguracion(void);

			AmbitoPtr ImportarModulo(
				std::string,
				ConfInterprete,
				ManejadorDeModulosPtr manejadorDeModulos
			);
		private:
			struct InfoModulo
			{
				AmbitoPtr ambito;
			};

			ConfManejadorDeModulos conf;
			std::map<std::string, InfoModulo> modulosImportados;
	};
}

#endif /* end of include guard: HDR_PSEUDOD_NEW_INTERPRETE_HH */
