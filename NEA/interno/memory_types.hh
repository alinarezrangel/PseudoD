/* Tipos adicionales de valores en PseudoD.
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

#ifndef HDR_PSEUDOD_MEMORY_TYPES_HH
#define HDR_PSEUDOD_MEMORY_TYPES_HH

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <tuple>

#include "data.hh"
#include "memory.hh"

namespace pseudod
{
	class TipoNulo : public Valor
	{
		public:
			explicit TipoNulo(void);
			virtual ~TipoNulo(void);

			virtual ValorPtr RecibirMensaje(
				std::string,
				const std::vector<ValorPtr>&
			) override;
	};

	ValorPtr CrearNulo(void);

	class Texto final : public Valor
	{
		public:
			explicit Texto(const std::string&);
			explicit Texto(std::string&&);
			virtual ~Texto(void);

			virtual ValorPtr RecibirMensaje(
				std::string,
				const std::vector<ValorPtr>&
			) override;

			explicit operator const std::string&(void) const;

			const std::string& ObtenerTexto(void) const;
		private:
			std::string texto;
	};

	class Referencia final : public Valor
	{
		public:
			explicit Referencia(AmbitoPtr, PDCadena);
			virtual ~Referencia(void);

			virtual ValorPtr RecibirMensaje(
				std::string,
				const std::vector<ValorPtr>&
			) override;

			ValorPtr Resolver(void) const;
			void Fijar(ValorPtr);
			AmbitoPtr ObtenerAmbito(void) const;
			PDCadena ObtenerVariableEnAmbito(void) const;
		private:
			AmbitoPtr ambito;
			PDCadena nombre;
	};

	class Boole final : public Valor
	{
		public:
			explicit Boole(bool);
			virtual ~Boole(void);

			virtual ValorPtr RecibirMensaje(
				std::string,
				const std::vector<ValorPtr>&
			) override;

			explicit operator bool(void) const;

			bool ObtenerBool(void) const;
		private:
			bool valor;
	};

	class Numero final : public Valor
	{
		public:
			enum class Tipo { ENTERO, REAL };

			struct MarcaTipoEntero {};
			struct MarcaTipoReal {};
			static MarcaTipoEntero MARCA_TIPO_ENTERO;
			static MarcaTipoReal MARCA_TIPO_REAL;

			using Ptr = std::shared_ptr<Numero>;

			explicit Numero(long long int, MarcaTipoEntero);
			explicit Numero(long double, MarcaTipoReal);
			virtual ~Numero(void);

			virtual ValorPtr RecibirMensaje(
				std::string,
				const std::vector<ValorPtr>&
			) override;

			long long int ObtenerEntero(void) const;
			long double ObtenerReal(void) const;

			Ptr Sumar(Ptr rhs);
			Ptr Negar(void);
			Ptr Restar(Ptr rhs);
			Ptr Multiplicar(Ptr rhs);
			Ptr Dividir(Ptr rhs);

			bool MenorQue(Ptr rhs);
			bool MayorQue(Ptr rhs);
			bool MenorOIgualA(Ptr rhs);
			bool MayorOIgualA(Ptr rhs);
			bool IgualA(Ptr rhs);
			bool DistintoDe(Ptr rhs);

			Tipo ObtenerTipo(void) const;
		private:
			void DebeTenerTipo(Tipo) const;

			union Valores
			{
				long long int valorEntero;
				long double valorReal;
			};

			Valores valor;
			Tipo tipoValor;
	};

	class Arreglo final : public Valor
	{
		public:
			explicit Arreglo(const std::vector<ValorPtr>& = std::vector<ValorPtr>());
			virtual ~Arreglo(void);

			virtual ValorPtr RecibirMensaje(
				std::string,
				const std::vector<ValorPtr>&
			) override;

			explicit operator const std::vector<ValorPtr>&(void) const;

			const std::vector<ValorPtr>& ObtenerArreglo(void) const;
			std::vector<ValorPtr>& ObtenerArreglo(void);
		private:
			std::vector<ValorPtr> elementos;
	};

	class Clase : public Valor
	{
		public:
			explicit Clase(void);
			virtual ~Clase(void);

			virtual ValorPtr RecibirMensaje(
				std::string,
				const std::vector<ValorPtr>&
			) override;

			virtual ValorPtr CrearInstancia(const std::vector<ValorPtr>&) = 0;
	};

	class ClaseBoole final : public Clase
	{
		public:
			explicit ClaseBoole(void);
			virtual ~ClaseBoole(void);

			virtual ValorPtr CrearInstancia(const std::vector<ValorPtr>&) override;
	};

	class EspacioDeNombres final : public Valor
	{
		public:
			explicit EspacioDeNombres(
				AmbitoPtr = std::make_shared<Ambito>()
			);
			virtual ~EspacioDeNombres(void);

			virtual ValorPtr RecibirMensaje(
				std::string,
				const std::vector<ValorPtr>&
			) override;

			AmbitoPtr ObtenerAmbito(void) const;

			std::shared_ptr<EspacioDeNombres> UnionDeEspacios(
				std::shared_ptr<EspacioDeNombres>
			) const;
		private:
			AmbitoPtr ambito;
	};

	class EnvolturaDeFuncion final : public Valor
	{
		public:
			using TipoFuncion = ValorPtr(const std::vector<ValorPtr>&);
			explicit EnvolturaDeFuncion(std::function<TipoFuncion>);
			virtual ~EnvolturaDeFuncion(void);

			virtual ValorPtr RecibirMensaje(
				std::string,
				const std::vector<ValorPtr>&
			) override;

			std::function<TipoFuncion> ObtenerFuncion(void) const;
		private:
			std::function<TipoFuncion> callback;
	};

	class EnvolturaDeMetodo final : public Valor
	{
		public:
			using TipoFuncion = ValorPtr(
				ValorPtr,
				const std::vector<ValorPtr>&
			);

			explicit EnvolturaDeMetodo(std::function<TipoFuncion>);
			virtual ~EnvolturaDeMetodo(void);

			virtual ValorPtr RecibirMensaje(
				std::string,
				const std::vector<ValorPtr>&
			) override;

			std::function<TipoFuncion> ObtenerFuncion(void) const;
		private:
			std::function<TipoFuncion> callback;
	};

	class ObjetoEnPseudoD : public Valor
	{
		public:
			explicit ObjetoEnPseudoD(
				std::map<std::string, ValorPtr>
					= std::map<std::string, ValorPtr>(),
				std::map<std::string, ValorPtr>
					= std::map<std::string, ValorPtr>()
			);
			virtual ~ObjetoEnPseudoD(void);

			virtual ValorPtr RecibirMensaje(
				std::string,
				const std::vector<ValorPtr>&
			) override;

			virtual ValorPtr ClonarObjeto(void);
			virtual ValorPtr CompararObjeto(ValorPtr);

			void AgregarMetodo(std::string, ValorPtr);
			void FijarMetodo(std::string, ValorPtr);
			ValorPtr ObtenerMetodo(std::string) const;
			bool PoseeMetodo(std::string) const;

			void AgregarAtributo(std::string, ValorPtr);
			void FijarAtributo(std::string, ValorPtr);
			ValorPtr ObtenerAtributo(std::string) const;
			bool PoseeAtributo(std::string) const;
		private:
			std::map<std::string, ValorPtr> atributos;
			std::map<std::string, ValorPtr> metodos;
	};

	namespace ImplementacionAceptaArgumentos
	{
		using Iterador = std::vector<ValorPtr>::iterator;

		template<typename Tipo>
		std::tuple<std::shared_ptr<Tipo>> AceptarArgumentos(Iterador argumentos)
		{
			return std::make_tuple(ValorComo<Tipo>(*argumentos));
		}

		template<typename Tipo1, typename Tipo2, typename... Resto>
		std::tuple<std::shared_ptr<Tipo1>, std::shared_ptr<Tipo2>, std::shared_ptr<Resto>...>
		AceptarArgumentos(Iterador argumentos)
		{
			auto tpl = std::make_tuple(ValorComo<Tipo1>(*argumentos));
			auto resto = AceptarArgumentos<Tipo2, Resto...>(argumentos + 1);

			return std::tuple_cat(tpl, resto);
		}
	}

	template<typename... Tipos>
	std::tuple<std::shared_ptr<Tipos>...>
	AceptarArgumentos(std::vector<ValorPtr> argumentos)
	{
		static_assert(sizeof...(Tipos) > 0, "Se debe especificar al menos un tipo");

		if(sizeof...(Tipos) != argumentos.size())
		{
			throw PDvar::ErrorDeSemantica(
				"Aridad invalida: se esperaban " +
				std::to_string(sizeof...(Tipos)) +
				" valores pero se obtubieron " +
				std::to_string(argumentos.size())
			);
		}

		return ImplementacionAceptaArgumentos::AceptarArgumentos<Tipos...>(
			argumentos.begin()
		);
	}

	void EsperaNumArgumentos(
		std::vector<ValorPtr> argumentos,
		std::vector<ValorPtr>::size_type num
	);
	void EsperarNingunArgumento(std::vector<ValorPtr> argumentos);

	template<typename F>
	ValorPtr RegistrarProcedimiento(AmbitoPtr ambito, std::string nombre, F func)
	{
		std::function<EnvolturaDeFuncion::TipoFuncion> f = func;
		auto proc = CrearValor<EnvolturaDeFuncion>(f);
		ambito->CrearVariable(nombre, proc);
		ambito->MarcarComoAutoEjecutable(nombre);
		return proc;
	}

	template<typename Iter>
	std::string FormatearTexto(std::string formato, Iter begin, Iter end)
	{
		std::string resultado;
		Iter cur = begin;
		resultado.reserve(formato.size());

		for(auto fiter = formato.begin(); fiter != formato.end(); fiter++)
		{
			if(*fiter != '~')
			{
				resultado += *fiter;
			}
			else
			{
				char fmt = *(++fiter);
				if(fiter == formato.end())
					throw PDvar::ErrorDelNucleo("Fin inesperado del texto");
				switch(fmt)
				{
					// Escapes simples:
					case '~': resultado += '~'; break;
					case '%': resultado += '\n'; break;
					case 'e': resultado += '}'; break;
					case 'E': resultado += "»"; break;
					case 'q': resultado += '\"'; break;

					case 'T':
						// Inserta un Texto.
						if(cur == end)
							throw PDvar::ErrorDelNucleo("No hay valor para el formato ~T");
						resultado += ValorComo<Texto>(*(cur++))->ObtenerTexto();
						break;

					case 't':
						// Como ~T, pero primero convierte el valor a un
						// Texto con #comoTexto
						if(cur == end)
							throw PDvar::ErrorDelNucleo("No hay valor para el formato ~t");
						{
							auto comoTexto = (*(cur++))->RecibirMensaje("comoTexto", std::vector<ValorPtr> {});
							resultado += ValorComo<Texto>(comoTexto)->ObtenerTexto();
						}
						break;
					case '|':
						// ~|% escapa el siguiente fin de línea.
						fiter++;
						if(fiter == formato.end())
							throw PDvar::ErrorDelNucleo("Fin inesperado del texto");
						if(*fiter != '%')
						{
							throw PDvar::ErrorDeSintaxis(
								"Formato ~|% incompleto."
							);
						}
						fiter++;
						if(fiter == formato.end())
							throw PDvar::ErrorDelNucleo("Fin inesperado del texto");
						if(*fiter == '\r' && (fiter + 1) != formato.end() && *(fiter + 1) == '\n')
						{
							fiter++;
						}
						else if(*fiter != '\n')
						{
							throw PDvar::ErrorDeSintaxis(
								"El formato ~|% debe escapar un fin de línea."
							);
						}
						break;
					default:
						throw PDvar::ErrorDeSintaxis(
							"Formato inválido."
						);
				}
			}
		}

		if(cur != end)
			throw PDvar::ErrorDelNucleo(
				"No se utilizaron todos los valores en el texto de formato."
			);

		return resultado;
	}
}

#endif /* end of include guard: HDR_PSEUDOD_MEMORY_TYPES_HH */
