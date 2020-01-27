/* Manejo de memoria en PseudoD -- Posee las clases que representan valores
 * y entornos léxicos.
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

#ifndef HDR_PSEUDOD_MEMORY_HH
#define HDR_PSEUDOD_MEMORY_HH

#include <memory>
#include <map>
#include <functional>

#include "data.hh"
#include "token.hh"
#include "tokenizer.hh"

namespace pseudod
{
	class Valor;
	using ValorPtr = std::shared_ptr<Valor>;

	class Valor : public std::enable_shared_from_this<Valor>
	{
		public:
			explicit Valor(void);
			virtual ~Valor(void) = 0;

			virtual ValorPtr RecibirMensaje(
				std::string,
				const std::vector<ValorPtr>&
			) = 0;
	};

	template<typename T, typename... Args>
	ValorPtr CrearValor(Args... args)
	{
		return std::dynamic_pointer_cast<Valor>(
			std::make_shared<T>(args...)
		);
	}

	template<typename T>
	std::shared_ptr<T> ValorComo(ValorPtr val)
	{
		auto ptr = std::dynamic_pointer_cast<T>(val);
		if(!ptr)
		{
			throw PDvar::ErrorDelNucleo("No se puede convertir el valor.");
		}
		return ptr;
	}

	template<typename T>
	bool ValorEs(ValorPtr val)
	{
		return std::dynamic_pointer_cast<T>(val) != nullptr;
	}

	class Ambito;
	using AmbitoPtr = std::shared_ptr<Ambito>;

	class Ambito
	{
		public:
			struct DatosDeVariable
			{
				ValorPtr valor;
				bool autoEjecutable;
			};

			struct EtiquetaObtenerDatos {} ObtenerDatos;

			using Desambiguador =
				std::function<DatosDeVariable(DatosDeVariable, DatosDeVariable)>;

			explicit Ambito(void);
			explicit Ambito(AmbitoPtr);

			ValorPtr operator[](PDCadena) const;

			ValorPtr ObtenerVariable(PDCadena nombre) const;
			const DatosDeVariable ObtenerVariable(
				PDCadena nombre,
				EtiquetaObtenerDatos
			) const;
			void FijarVariable(PDCadena nombre, ValorPtr val);
			void CrearVariable(PDCadena nombre, ValorPtr valorInicial);
			AmbitoPtr ObtenerAmbitoPadre(void) const;
			void EliminarVariable(PDCadena nombre);
			bool Existe(PDCadena) const;

			void Insertar(Ambito otro, Desambiguador desambiguador);

			bool DebeAutoEjecutarse(PDCadena nombre) const;
			void MarcarComoAutoEjecutable(PDCadena nombre);
		private:
			bool ExisteAqui(PDCadena) const;

			std::map<PDCadena, DatosDeVariable> variables;
			AmbitoPtr padre;
	};
}

#endif /* end of include guard: HDR_PSEUDOD_MEMORY_HH */
