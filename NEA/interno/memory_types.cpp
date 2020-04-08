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

#include <utility>
#include <algorithm>
#include <iterator>
#include <iostream>

#include "memory_types.hh"

namespace pseudod
{
	template<typename T>
	using Ptr = std::shared_ptr<T>;

	void EsperaNumArgumentos(
		std::vector<ValorPtr> argumentos,
		std::vector<ValorPtr>::size_type num
	)
	{
		if(argumentos.size() != num)
		{
			throw PDvar::ErrorDeSemantica(
				"Numero de argumentos invalidos"
			);
		}
	}

	void EsperarNingunArgumento(std::vector<ValorPtr> argumentos)
	{
		EsperaNumArgumentos(argumentos, 0);
	}

	TipoNulo::TipoNulo(void) : Valor() {}

	TipoNulo::~TipoNulo(void) {}

	ValorPtr TipoNulo::RecibirMensaje(
		std::string mensaje,
		const std::vector<ValorPtr>& argumentos
	)
	{
		if(mensaje == "clonar")
		{
			EsperarNingunArgumento(argumentos);
			return this->shared_from_this();
		}
		else if(mensaje == "igualA" || mensaje == "operador_=")
		{
			EsperaNumArgumentos(argumentos, 1);
			return CrearValor<Boole>(ValorEs<TipoNulo>(argumentos[0]));
		}
		else if(mensaje == "comoTexto")
		{
			EsperarNingunArgumento(argumentos);
			return CrearValor<Texto>("NULO");
		}

		throw PDvar::ErrorDelNucleo(
			"No se encontro el mensaje " + mensaje + " en la instancia de TipoNulo"
		);
	}

	ValorPtr CrearNulo(void)
	{
		return CrearValor<TipoNulo>();
	}

	Texto::Texto(const std::string& t) : Valor(), texto(t)
	{}

	Texto::Texto(std::string&& t) : Valor(), texto(std::move(t))
	{}

	Texto::~Texto(void)
	{}

	ValorPtr Texto::RecibirMensaje(
		std::string mensaje,
		const std::vector<ValorPtr>& argumentos
	)
	{
		if(mensaje == "igualA" || mensaje == "operador_=")
		{
			EsperaNumArgumentos(argumentos, 1);
			if(!ValorEs<Texto>(argumentos[0])) return CrearValor<Boole>(false);
			auto otro = ValorComo<Texto>(argumentos[0]);
			return CrearValor<Boole>(
				this->ObtenerTexto() == otro->ObtenerTexto()
			);
		}
		else if(mensaje == "clonar")
		{
			EsperarNingunArgumento(argumentos);
			return this->shared_from_this();
		}
		else if(mensaje == "comoEnteroFijo")
		{
			EsperarNingunArgumento(argumentos);
			return CrearValor<EnteroFijo>(std::stoll(this->texto));
		}
		else if(mensaje == "comoTexto")
		{
			EsperarNingunArgumento(argumentos);
			return this->shared_from_this();
		}
		else if(mensaje == "longitud")
		{
			EsperarNingunArgumento(argumentos);
			return CrearValor<EnteroFijo>(this->texto.size());
		}
		else if(mensaje == "en")
		{
			auto targs = AceptarArgumentos<EnteroFijo>(argumentos);
			auto indice = std::get<0>(targs)->ObtenerEntero();
			return CrearValor<Texto>(std::string(1, this->texto.at(indice)));
		}
		else if(mensaje == "concatenar")
		{
			auto targs = AceptarArgumentos<Texto>(argumentos);
			auto otro = std::get<0>(targs)->ObtenerTexto();
			return CrearValor<Texto>(this->texto + otro);
		}
		else if(mensaje == "subTexto")
		{
			auto targs = AceptarArgumentos<EnteroFijo, EnteroFijo>(argumentos);
			auto inicio = std::get<0>(targs)->ObtenerEntero();
			auto longitud = std::get<1>(targs)->ObtenerEntero();
			return CrearValor<Texto>(this->texto.substr(inicio, longitud));
		}
		else if(mensaje == "parteDelTexto")
		{
			auto targs = AceptarArgumentos<EnteroFijo, EnteroFijo>(argumentos);
			auto inicio = std::get<0>(targs)->ObtenerEntero();
			auto final = std::get<1>(targs)->ObtenerEntero();
			return CrearValor<Texto>(this->texto.substr(inicio, final - inicio));
		}
		else if(mensaje == "buscar")
		{
			auto targs = AceptarArgumentos<EnteroFijo, Texto>(argumentos);
			auto inicio = std::get<0>(targs)->ObtenerEntero();
			auto aBuscar = std::get<1>(targs)->ObtenerTexto();
			return CrearValor<EnteroFijo>(this->texto.find(aBuscar, inicio));
		}
		else if(mensaje == "buscarEnReversa")
		{
			auto targs = AceptarArgumentos<EnteroFijo, Texto>(argumentos);
			auto inicio = std::get<0>(targs)->ObtenerEntero();
			auto aBuscar = std::get<1>(targs)->ObtenerTexto();
			return CrearValor<EnteroFijo>(this->texto.rfind(aBuscar, inicio));
		}
		else if(mensaje == "formatear")
		{
			return CrearValor<Texto>(FormatearTexto(
				this->ObtenerTexto(),
				argumentos.begin(),
				argumentos.end()
			));
		}

		throw PDvar::ErrorDelNucleo(
			"No se encontro el mensaje " + mensaje + " en la instancia de Texto"
		);
	}

	Texto::operator const std::string&(void) const
	{
		return this->texto;
	}

	const std::string& Texto::ObtenerTexto(void) const
	{
		return this->texto;
	}

	Referencia::Referencia(AmbitoPtr am, PDCadena var) : ambito(am), nombre(var)
	{}

	Referencia::~Referencia(void)
	{}

	ValorPtr Referencia::RecibirMensaje(
		std::string mensaje,
		const std::vector<ValorPtr>& argumentos
	)
	{
		if(mensaje == "igualA" || mensaje == "operador_=")
		{
			EsperaNumArgumentos(argumentos, 1);
			if(!ValorEs<Referencia>(argumentos[0])) return CrearValor<Boole>(false);
			auto otro = ValorComo<Referencia>(argumentos[0]);
			return CrearValor<Boole>(
				this->ObtenerAmbito() == otro->ObtenerAmbito()
				&& this->ObtenerVariableEnAmbito() == otro->ObtenerVariableEnAmbito()
			);
		}
		else if(mensaje == "clonar")
		{
			EsperarNingunArgumento(argumentos);
			return this->shared_from_this();
		}
		else if(mensaje == "resolver")
		{
			EsperarNingunArgumento(argumentos);
			return this->Resolver();
		}
		else if(mensaje == "fijarA")
		{
			EsperaNumArgumentos(argumentos, 1);
			this->Fijar(argumentos[0]);
			return this->Resolver();
		}

		throw PDvar::ErrorDelNucleo(
			"No se encontro el mensaje " + mensaje + " en la instancia de Referencia"
		);
	}

	ValorPtr Referencia::Resolver(void) const
	{
		return this->ambito->ObtenerVariable(this->nombre);
	}

	void Referencia::Fijar(ValorPtr val)
	{
		this->ambito->FijarVariable(this->nombre, val);
	}

	AmbitoPtr Referencia::ObtenerAmbito(void) const
	{
		return this->ambito;
	}

	PDCadena Referencia::ObtenerVariableEnAmbito(void) const
	{
		return this->nombre;
	}

	Boole::Boole(bool b) : valor(b)
	{}

	Boole::~Boole(void)
	{}

	ValorPtr Boole::RecibirMensaje(
		std::string mensaje,
		const std::vector<ValorPtr>& args
	)
	{
		if(mensaje == "igualA" || mensaje == "operador_=")
		{
			EsperaNumArgumentos(args, 1);
			if(!ValorEs<Boole>(args[0])) return CrearValor<Boole>(false);
			auto otro = ValorComo<Boole>(args[0]);
			return CrearValor<Boole>(
				this->ObtenerBool() == otro->ObtenerBool()
			);
		}
		else if(mensaje == "clonar")
		{
			EsperaNumArgumentos(args, 0);
			return this->shared_from_this();
		}
		else if(mensaje == "escojer")
		{
			EsperaNumArgumentos(args, 2);
			return (*this)? args[0] : args[1];
		}
		else if(mensaje == "llamarSegun")
		{
			EsperaNumArgumentos(args, 2);
			std::vector<ValorPtr> vacio;
			if(*this)
				return args[0]->RecibirMensaje("llamar", vacio);
			else
				return args[1]->RecibirMensaje("llamar", vacio);
		}
		else if(mensaje == "y" || mensaje == "operador_&&")
		{
			auto targs = AceptarArgumentos<Boole>(args);
			if(*this)
				return CrearValor<Boole>(*std::get<0>(targs));
			else
				return CrearValor<Boole>(false);
		}
		else if(mensaje == "o" || mensaje == "operador_||")
		{
			auto targs = AceptarArgumentos<Boole>(args);
			if(*this)
				return CrearValor<Boole>(true);
			else
				return CrearValor<Boole>(*std::get<0>(targs));
		}

		throw PDvar::ErrorDelNucleo(
			"No se encontro el mensaje " + mensaje + " en la instancia de Boole"
		);
	}

	Boole::operator bool(void) const
	{
		return this->valor;
	}

	bool Boole::ObtenerBool(void) const
	{
		return this->valor;
	}

	EnteroFijo::EnteroFijo(long long int v) : Valor(), valor(v)
	{}

	EnteroFijo::~EnteroFijo(void)
	{}

	namespace
	{
		template<typename T, typename Ret, typename C, typename F, typename G>
		ValorPtr BinOp(C val, std::vector<ValorPtr> args, F op, G conv)
		{
			auto targs = AceptarArgumentos<T>(args);
			return CrearValor<Ret>(op(val, conv(std::get<0>(targs))));
		}

		template<typename Ret, typename F>
		ValorPtr BinOpEntero(EnteroFijo* x, std::vector<ValorPtr> args, F op)
		{
			return BinOp<EnteroFijo, Ret>(
				x->ObtenerEntero(),
				args,
				op,
				[](Ptr<EnteroFijo> x) { return x->ObtenerEntero(); }
			);
		}
	}

	ValorPtr EnteroFijo::RecibirMensaje(
		std::string mensaje,
		const std::vector<ValorPtr>& argumentos
	)
	{
		if(mensaje == "comoTexto")
		{
			EsperarNingunArgumento(argumentos);
			return CrearValor<Texto>(std::to_string(this->valor));
		}
		else if(mensaje == "clonar")
		{
			EsperarNingunArgumento(argumentos);
			return this->shared_from_this();
		}
		else if(mensaje == "negar")
		{
			EsperarNingunArgumento(argumentos);
			return CrearValor<EnteroFijo>(-this->valor);
		}
		else if(mensaje == "sumar" || mensaje == "operador_+")
		{
			return BinOpEntero<EnteroFijo>(
				this,
				argumentos,
				[](auto x, auto y) { return x + y; }
			);
		}
		else if(mensaje == "restar" || mensaje == "operador_-")
		{
			return BinOpEntero<EnteroFijo>(
				this,
				argumentos,
				[](auto x, auto y) { return x - y; }
			);
		}
		else if(mensaje == "multiplicar" || mensaje == "operador_*")
		{
			return BinOpEntero<EnteroFijo>(
				this,
				argumentos,
				[](auto x, auto y) { return x * y; }
			);
		}
		else if(mensaje == "dividir" || mensaje == "operador_/")
		{
			return BinOpEntero<EnteroFijo>(
				this,
				argumentos,
				[](auto x, auto y) { return x / y; }
			);
		}
		else if(mensaje == "menorQue" || mensaje == "operador_<")
		{
			return BinOpEntero<Boole>(
				this,
				argumentos,
				[](auto x, auto y) { return x < y; }
			);
		}
		else if(mensaje == "mayorQue" || mensaje == "operador_>")
		{
			return BinOpEntero<Boole>(
				this,
				argumentos,
				[](auto x, auto y) { return x > y; }
			);
		}
		else if(mensaje == "menorOIgualA" || mensaje == "operador_=<")
		{
			return BinOpEntero<Boole>(
				this,
				argumentos,
				[](auto x, auto y) { return x <= y; }
			);
		}
		else if(mensaje == "mayorOIgualA" || mensaje == "operador_>=")
		{
			return BinOpEntero<Boole>(
				this,
				argumentos,
				[](auto x, auto y) { return x >= y; }
			);
		}
		else if(mensaje == "igualA" || mensaje == "operador_=")
		{
			return BinOpEntero<Boole>(
				this,
				argumentos,
				[](auto x, auto y) { return x == y; }
			);
		}

		throw PDvar::ErrorDelNucleo(
			"No se encontro el mensaje " + mensaje + " en la instancia de EnteroFijo"
		);
	}

	EnteroFijo::operator long long int(void) const
	{
		return this->valor;
	}

	long long int EnteroFijo::ObtenerEntero(void) const
	{
		return this->valor;
	}

	Arreglo::Arreglo(const std::vector<ValorPtr>& els) : Valor(), elementos(els)
	{}

	Arreglo::~Arreglo(void)
	{}

	ValorPtr Arreglo::RecibirMensaje(
		std::string mensaje,
		const std::vector<ValorPtr>& argumentos
	)
	{
		if(mensaje == "en")
		{
			auto targs = AceptarArgumentos<EnteroFijo>(argumentos);
			return this->elementos.at(std::get<0>(targs)->ObtenerEntero());
		}
		else if(mensaje == "fijarEn")
		{
			auto targs = AceptarArgumentos<EnteroFijo, Valor>(argumentos);
			this->elementos.at(std::get<0>(targs)->ObtenerEntero()) =
				std::get<1>(targs);
			return CrearNulo();
		}
		else if(mensaje == "redimensionar")
		{
			auto targs = AceptarArgumentos<EnteroFijo>(argumentos);
			this->elementos.resize(
				std::get<0>(targs)->ObtenerEntero(),
				CrearNulo()
			);
			return CrearNulo();
		}
		else if(mensaje == "agregarAlFinal")
		{
			EsperaNumArgumentos(argumentos, 1);
			this->elementos.push_back(argumentos[0]);
			return CrearNulo();
		}
		else if(mensaje == "mapear")
		{
			EsperaNumArgumentos(argumentos, 1);
			for(ValorPtr& el : this->elementos)
			{
				std::vector<ValorPtr> callArgs = {el};
				el = argumentos[0]->RecibirMensaje("llamar", callArgs);
			}
			return CrearNulo();
		}
		else if(mensaje == "reducir")
		{
			EsperaNumArgumentos(argumentos, 2);
			ValorPtr acc = argumentos[0];
			for(ValorPtr el : this->elementos)
			{
				std::vector<ValorPtr> callArgs = {acc, el};
				acc = argumentos[1]->RecibirMensaje("llamar", callArgs);
			}
			return CrearNulo();
		}
		else if(mensaje == "longitud")
		{
			EsperarNingunArgumento(argumentos);
			return CrearValor<EnteroFijo>(this->elementos.size());
		}
		else if(mensaje == "clonar")
		{
			EsperarNingunArgumento(argumentos);
			std::vector<ValorPtr> copia = this->ObtenerArreglo();
			for(auto& el : copia)
			{
				el = el->RecibirMensaje("clonar", std::vector<ValorPtr> {});
			}
			return CrearValor<Arreglo>(copia);
		}
		else if(mensaje == "igualA" || mensaje == "operador_=")
		{
			auto targs = AceptarArgumentos<Arreglo>(argumentos);
			auto arr = std::get<0>(targs);
			if(this->elementos.size() != arr->elementos.size())
			{
				return CrearValor<Boole>(false);
			}
			else
			{
				for(size_t i = 0; i < this->elementos.size(); i++)
				{
					auto otro = arr->elementos[i];
					auto res = this->elementos[i]->RecibirMensaje(
						"igualA",
						std::vector<ValorPtr> {otro}
					);
					if(!ValorComo<Boole>(res)->ObtenerBool())
					{
						return CrearValor<Boole>(false);
					}
				}
				return CrearValor<Boole>(true);
			}
		}

		throw PDvar::ErrorDelNucleo(
			"No se encontro el mensaje <" + mensaje + "> en la instancia de Arreglo"
		);
	}

	Arreglo::operator const std::vector<ValorPtr>&(void) const
	{
		return elementos;
	}

	const std::vector<ValorPtr>& Arreglo::ObtenerArreglo(void) const
	{
		return elementos;
	}

	std::vector<ValorPtr>& Arreglo::ObtenerArreglo(void)
	{
		return elementos;
	}

	Clase::Clase(void) : Valor() {}

	Clase::~Clase(void) {}

	ValorPtr Clase::RecibirMensaje(
		std::string mensaje,
		const std::vector<ValorPtr>& argumentos
	)
	{
		if(mensaje == "crear")
		{
			return this->CrearInstancia(argumentos);
		}

		throw PDvar::ErrorDelNucleo(
			"No se encontro el mensaje " + mensaje + " en la instancia de Clase"
		);
	}

	ClaseBoole::ClaseBoole(void) : Clase() {}

	ClaseBoole::~ClaseBoole(void) {}

	ValorPtr ClaseBoole::CrearInstancia(const std::vector<ValorPtr>& args)
	{
		EsperaNumArgumentos(args, 0);
		return CrearValor<Boole>(false);
	}

	EspacioDeNombres::EspacioDeNombres(AmbitoPtr ambito)
		: Valor(), ambito(ambito)
	{}

	EspacioDeNombres::~EspacioDeNombres(void)
	{}

	ValorPtr EspacioDeNombres::RecibirMensaje(
		std::string mensaje,
		const std::vector<ValorPtr>& argumentos
	)
	{
		if(!this->ambito->Existe(mensaje))
		{
			throw PDvar::ErrorDeSemantica(
				"La variable no existe en el espacio de nombres"
			);
		}

		ValorPtr var = this->ambito->ObtenerVariable(mensaje);

		if(this->ambito->DebeAutoEjecutarse(mensaje))
		{
			return var->RecibirMensaje("llamar", argumentos);
		}
		else
		{
			EsperarNingunArgumento(argumentos);
			return var;
		}
	}

	AmbitoPtr EspacioDeNombres::ObtenerAmbito(void) const
	{
		return this->ambito;
	}

	std::shared_ptr<EspacioDeNombres> EspacioDeNombres::UnionDeEspacios(
		std::shared_ptr<EspacioDeNombres> otro
	) const
	{
		AmbitoPtr nuevoAmbito = std::make_shared<Ambito>(this->ambito);
		Ambito::Desambiguador desambiguador =
			[](auto viejo, auto nuevo)
			{
				return nuevo;
			};
		nuevoAmbito->Insertar(*otro->ambito, desambiguador);
		return std::make_shared<EspacioDeNombres>(nuevoAmbito);
	}

	EnvolturaDeFuncion::EnvolturaDeFuncion(
		std::function<EnvolturaDeFuncion::TipoFuncion> f
	) : Valor(), callback(f)
	{}

	EnvolturaDeFuncion::~EnvolturaDeFuncion(void)
	{}

	ValorPtr EnvolturaDeFuncion::RecibirMensaje(
		std::string mensaje,
		const std::vector<ValorPtr>& argumentos
	)
	{
		if(mensaje == "llamar")
		{
			return callback(argumentos);
		}
		else if(mensaje == "clonar")
		{
			EsperarNingunArgumento(argumentos);
			return this->shared_from_this();
		}
		else if(mensaje == "igualA" || mensaje == "operador_=")
		{
			EsperaNumArgumentos(argumentos, 1);
			return CrearValor<Boole>(this->shared_from_this() == argumentos[0]);
		}

		throw PDvar::ErrorDelNucleo(
			"No se encontro el mensaje " + mensaje + " en el procedimiento"
		);
	}

	std::function<EnvolturaDeFuncion::TipoFuncion>
	EnvolturaDeFuncion::ObtenerFuncion(void) const
	{
		return callback;
	}

	EnvolturaDeMetodo::EnvolturaDeMetodo(
		std::function<EnvolturaDeMetodo::TipoFuncion> f
	) : Valor(), callback(f)
	{}

	EnvolturaDeMetodo::~EnvolturaDeMetodo(void)
	{}

	ValorPtr EnvolturaDeMetodo::RecibirMensaje(
		std::string mensaje,
		const std::vector<ValorPtr>& argumentos
	)
	{
		if(mensaje == "llamar")
		{
			if(argumentos.size() < 1)
			{
				throw PDvar::ErrorDelNucleo("Se esperaba al menos un argumento");
			}

			auto yo = argumentos[0];

			std::vector<ValorPtr> restoDeArgumentos;
			std::copy(
				argumentos.begin() + 1,
				argumentos.end(),
				std::back_inserter(restoDeArgumentos)
			);

			return callback(yo, restoDeArgumentos);
		}
		else if(mensaje == "clonar")
		{
			EsperarNingunArgumento(argumentos);
			return this->shared_from_this();
		}
		else if(mensaje == "igualA" || mensaje == "operador_=")
		{
			EsperaNumArgumentos(argumentos, 1);
			return CrearValor<Boole>(this->shared_from_this() == argumentos[0]);
		}

		throw PDvar::ErrorDelNucleo(
			"No se encontro el mensaje " + mensaje + " en el procedimiento"
		);
	}

	std::function<EnvolturaDeMetodo::TipoFuncion>
	EnvolturaDeMetodo::ObtenerFuncion(void) const
	{
		return callback;
	}

	ObjetoEnPseudoD::ObjetoEnPseudoD(
		std::map<std::string, ValorPtr> atributos,
		std::map<std::string, ValorPtr> metodos
	) : Valor(), atributos(atributos), metodos(metodos)
	{}

	ObjetoEnPseudoD::~ObjetoEnPseudoD(void) {}

	ValorPtr ObjetoEnPseudoD::RecibirMensaje(
		std::string mensaje,
		const std::vector<ValorPtr>& argumentos
	)
	{
		if(!this->PoseeMetodo(mensaje))
		{
			if(this->PoseeMetodo("metodoNoEncontrado"))
			{
				std::vector<ValorPtr> nuevosArgumentos {
					this->shared_from_this(),
					CrearValor<Texto>(mensaje),
					CrearValor<Arreglo>(argumentos),
				};
				return this->ObtenerMetodo("metodoNoEncontrado")->RecibirMensaje(
					"llamar",
					nuevosArgumentos
				);
			}
			else if(mensaje == "clonar")
			{
				return this->ClonarObjeto();
			}
			else if(mensaje == "igualA" || mensaje == "operador_=")
			{
				EsperaNumArgumentos(argumentos, 1);
				return this->CompararObjeto(argumentos[0]);
			}
			else
			{
				throw PDvar::ErrorDelNucleo(
					"No se encontro el mensaje " + mensaje + " en la instancia"
				);
			}
		}

		std::vector<ValorPtr> nuevosArgumentos = argumentos;
		nuevosArgumentos.insert(
			nuevosArgumentos.begin(),
			this->shared_from_this()
		);
		return this->ObtenerMetodo(mensaje)->RecibirMensaje(
			"llamar",
			nuevosArgumentos
		);
	}

	ValorPtr ObjetoEnPseudoD::ClonarObjeto(void)
	{
		auto copia = std::make_shared<ObjetoEnPseudoD>();
		for(const auto& atributo : this->atributos)
		{
			copia->AgregarAtributo(atributo.first, atributo.second->RecibirMensaje("clonar", {}));
		}
		for(const auto& metodo : this->metodos)
		{
			copia->AgregarMetodo(metodo.first, metodo.second->RecibirMensaje("clonar", {}));
		}
		return copia;
	}

	ValorPtr ObjetoEnPseudoD::CompararObjeto(ValorPtr otro)
	{
		if(!ValorEs<ObjetoEnPseudoD>(otro))
		{
			return CrearValor<Boole>(false);
		}

		auto comparar = [](auto mapYo, auto mapOtro)
		{
			for(const auto& campo : mapYo)
			{
				if(mapOtro.count(campo.first) != 1)
				{
					return false;
				}

				if(campo.second == mapOtro[campo.first])
				{
					continue;
				}

				auto res = mapOtro[campo.first]->RecibirMensaje(
					"igualA",
					std::vector<ValorPtr> {campo.second}
				);

				if(!ValorComo<Boole>(res)->ObtenerBool())
				{
					return false;
				}
			}
			return true;
		};

		auto obj = ValorComo<ObjetoEnPseudoD>(otro);

		if(!comparar(this->atributos, obj->atributos))
			return CrearValor<Boole>(false);
		if(!comparar(obj->atributos, this->atributos))
			return CrearValor<Boole>(false);

		if(!comparar(this->metodos, obj->metodos))
			return CrearValor<Boole>(false);
		if(!comparar(obj->metodos, this->metodos))
			return CrearValor<Boole>(false);

		return CrearValor<Boole>(true);
	}

	void ObjetoEnPseudoD::AgregarMetodo(
		std::string mensaje,
		ValorPtr cuerpo
	)
	{
		if(this->PoseeMetodo(mensaje))
		{
			throw PDvar::ErrorDelNucleo(
				"No se puede agregar el metodo " + mensaje + ": ya existe"
			);
		}
		this->metodos.insert({mensaje, cuerpo});
	}

	void ObjetoEnPseudoD::FijarMetodo(std::string nombre, ValorPtr proc)
	{
		if(!this->PoseeMetodo(nombre))
		{
			throw PDvar::ErrorDelNucleo(
				"No se puede fijar el metodo " + nombre + ": no existe"
			);
		}
		this->metodos[nombre] = proc;
	}

	ValorPtr ObjetoEnPseudoD::ObtenerMetodo(std::string mensaje) const
	{
		return this->metodos.at(mensaje);
	}

	bool ObjetoEnPseudoD::PoseeMetodo(std::string mensaje) const
	{
		return this->metodos.count(mensaje) > 0;
	}

	void ObjetoEnPseudoD::AgregarAtributo(std::string nombre, ValorPtr valor)
	{
		if(this->PoseeAtributo(nombre))
		{
			throw PDvar::ErrorDelNucleo(
				"No se puede agregar el atributo " + nombre + ": ya existe"
			);
		}
		this->atributos.insert({nombre, valor});
	}

	void ObjetoEnPseudoD::FijarAtributo(std::string nombre, ValorPtr valor)
	{
		if(!this->PoseeAtributo(nombre))
		{
			throw PDvar::ErrorDelNucleo(
				"No se puede agregar el atributo " + nombre + ": no existe"
			);
		}
		this->atributos.at(nombre) = valor;
	}

	ValorPtr ObjetoEnPseudoD::ObtenerAtributo(std::string nombre) const
	{
		if(!this->PoseeAtributo(nombre))
		{
			throw PDvar::ErrorDelNucleo(
				"El atributo " + nombre + " no existe en el objeto"
			);
		}
		return this->atributos.at(nombre);
	}

	bool ObjetoEnPseudoD::PoseeAtributo(std::string nombre) const
	{
		return this->atributos.count(nombre) > 0;
	}
}