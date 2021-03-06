/* Variables, datos y procedimientos generales en PseudoD.
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

#include <sstream>
#include <iostream>
#include <memory>

#include "memory_types.hh"
#include "nmemoic.hh"
#include "token.hh"
#include "new_tokenizer.hh"
#include "new_interpreter.hh"

#include "builtins.hh"

namespace pseudod
{
	namespace
	{
		class ErrorParaCapturar : public PDvar::Error
		{
		public:
			ErrorParaCapturar(void) noexcept
				: Error("ErrorParaCapturar", ""), idDeCaptura(nullptr)
			{}
			ErrorParaCapturar(PDCadena message) noexcept
				: Error("ErrorParaCapturar", message), idDeCaptura(nullptr)
			{}
			ErrorParaCapturar(const Error& other) noexcept
				: Error(other), idDeCaptura(nullptr)
			{}
			virtual ~ErrorParaCapturar(void) noexcept
			{}

			void FijarIDDeCaptura(std::shared_ptr<bool> cap)
			{
				this->idDeCaptura = cap;
			}

			std::shared_ptr<bool> ObtenerIDDeCaptura() const
			{
				return this->idDeCaptura;
			}
		private:
			std::shared_ptr<bool> idDeCaptura;
		};
	}

	ValorPtr CrearClaseObjeto(void)
	{
		/* Crear una clase desde cero para PseudoD es más fácil en PseudoD que
		** en C++, así que la siguiente función ejecuta un pequeño programa
		** y obtiene el objeto creado.
		*/

		const std::string programa = R"EOF(
adquirir Objeto
fijar Objeto a __CrearObjeto

[
	atributosDeInstancia es un arreglo de textos: los nombres de los atributos.
]
__AgregarAtributo: Objeto, {atributosDeInstancia}, __ArregloCon
[
	metodosDeInstancia es un arreglo de arreglos de dos elementos: el primer
	elemento es el nombre del método y el segundo elemento es el procedimiento
	mismo.
]
__AgregarAtributo: Objeto, {metodosDeInstancia}, (__ArregloCon:
	(__ArregloCon: {inicializar}, metodo finmetodo)
)
[
	La clase base de esta clase. NULO es un valor especial que indica que no
	posee clase base.
]
__AgregarAtributo: Objeto, {claseBase}, NULO
[
	nombreDeClase contiene el nombre de la clase como un Texto. Para ver el
	comportamiento de este atributo con respecto al método #subclase véase
	su comentario más abajo.
]
__AgregarAtributo: Objeto, {nombreDeClase}, {Objeto}

__AgregarMetodo: Objeto, {__agregarAtributo}, metodo: nombre
	(__ObtenerAtributo: yo, {atributosDeInstancia})#agregarAlFinal: nombre
finmetodo

__AgregarMetodo: Objeto, {agregarMetodo}, metodo: nombre, proc
	(__ObtenerAtributo: yo, {metodosDeInstancia})#agregarAlFinal: (__ArregloCon: nombre, proc)
finmetodo

__AgregarMetodo: Objeto, {fijar_nombre}, metodo: nuevoNombre
	__FijarAtributo: yo, {nombreDeClase}, nuevoNombre
finmetodo

__AgregarMetodo: Objeto, {nombre}, metodo
	devolver __ObtenerAtributo: yo, {nombreDeClase}
finmetodo

[
	#agregarAtributo no solo agrega un atributo: también agregar dos métodos
	para obtener y fijar el valor de dicho.
]
__AgregarMetodo: Objeto, {agregarAtributo}, metodo: nombre
	yo#__agregarAtributo: nombre
	yo#agregarMetodo: nombre, metodo
		devolver __ObtenerAtributo: yo, nombre
	finmetodo
	yo#agregarMetodo: ({fijar_}#concatenar: nombre), metodo: val
		__FijarAtributo: yo, nombre, val
	finmetodo
finmetodo

__AgregarMetodo: Objeto, {claseBase}, metodo
	devolver __ObtenerAtributo: yo, {claseBase}
finmetodo

[
	#subclase crea y devuelve una clase que es hija de esta. El atributo
	nombreDeClase es fijado a {<clase anónima>} pero puede ser cambiado
	mediante el método {fijar_nombre} de la subclase.
]
__AgregarMetodo: Objeto, {subclase}, metodo
	adquirir clon
	fijar clon a yo#\clonar\
	__FijarAtributo: clon, {claseBase}, yo
	__FijarAtributo: clon, {nombreDeClase}, {<clase anónima>}
	devolver clon
finmetodo

[
	Este método debe ser compartido por todas las instancias: de otra forma,
	las instancias serían "diferentes" (debido a que sus métodos #tipo) y
	haría el método #igualA por defecto que ellas tienen inútil.
]
adquirir MetodoTipo
fijar MetodoTipo a metodo
	devolver __ObtenerAtributo: yo, {__tipo}
finmetodo

__AgregarMetodo: Objeto, {_crear}, metodo
	adquirir inst
	fijar inst a __CrearObjeto

	[
		Todas las instancias poseen un atributo "__tipo" con una referencia a
		la clase de la cual se creó.
	]
	__AgregarAtributo: inst, {__tipo}, yo
	__AgregarMetodo: inst, {__tipo}, MetodoTipo

	(__ObtenerAtributo: yo, {atributosDeInstancia})#mapear: procedimiento: attr
		__AgregarAtributo: inst, attr, NULO
		devolver attr
	finmetodo

	(__ObtenerAtributo: yo, {metodosDeInstancia})#mapear: procedimiento: met
		adquirir nombre
		adquirir proc
		fijar nombre a met#en: 0
		fijar proc a met#en: 1
		si __PoseeMetodo: inst, nombre
			__FijarMetodo: inst, nombre, proc
		sino
			__AgregarMetodo: inst, nombre, proc
		finsi
		devolver met
	finmetodo

	devolver inst
finmetodo

__AgregarMetodo: Objeto, {_crearConYo}, metodo: nuevoYo
	adquirir inst
	fijar inst a __CrearObjeto

	(__ObtenerAtributo: yo, {metodosDeInstancia})#mapear: procedimiento: met
		adquirir nombre
		adquirir proc
		adquirir oldproc
		fijar nombre a met#en: 0
		fijar oldproc a met#en: 1
		fijar proc a procedimiento: _yo, ...args
			adquirir nargs
			fijar nargs a __ArregloCon
			nargs#agregarAlFinal: nuevoYo
			args#mapear: procedimiento: x
				nargs#agregarAlFinal: x
				devolver x
			finprocedimiento
			devolver __Aplicar: oldproc, nargs
		finprocedimiento
		si __PoseeMetodo: inst, nombre
			__FijarMetodo: inst, nombre, proc
		sino
			__AgregarMetodo: inst, nombre, proc
		finsi
		devolver met
	finmetodo

	devolver inst
finmetodo

__AgregarMetodo: Objeto, {crear}, metodo: ...valores
	adquirir inst
	fijar inst a yo#_crear
	__EnviarMensaje: inst, {inicializar}, valores
	devolver inst
finmetodo

__AgregarMetodo: Objeto, {comoTexto}, metodo
	devolver {Clase ~T}#formatear: yo#nombre
finmetodo

		)EOF";
		std::istringstream in(programa);

		ConfInterprete conf;
		auto ambito = std::make_shared<Ambito>();

		ambito->CrearVariable("VERDADERO", CrearValor<Boole>(true));
		ambito->CrearVariable("FALSO", CrearValor<Boole>(false));
		ambito->CrearVariable("NULO", CrearNulo());

		RegistrarProcedimiento(ambito, "__ArregloCon", [](auto args)
		{
			return CrearValor<Arreglo>(args);
		});

		RegistrarProcedimiento(ambito, "__CrearObjeto", [](auto args)
		{
			EsperarNingunArgumento(args);
			return CrearValor<ObjetoEnPseudoD>();
		});

		RegistrarProcedimiento(ambito, "__AgregarMetodo", [](auto args)
		{
			auto targs = AceptarArgumentos<ObjetoEnPseudoD, Texto, Valor>(args);
			std::get<0>(targs)->AgregarMetodo(
				std::get<1>(targs)->ObtenerTexto(),
				std::get<2>(targs)
			);
			return CrearNulo();
		});

		RegistrarProcedimiento(ambito, "__PoseeMetodo", [](auto args)
		{
			auto targs = AceptarArgumentos<ObjetoEnPseudoD, Texto>(args);
			bool posee = std::get<0>(targs)->PoseeMetodo(std::get<1>(targs)->ObtenerTexto());
			return CrearValor<Boole>(posee);
		});

		RegistrarProcedimiento(ambito, "__FijarMetodo", [](auto args)
		{
			auto targs = AceptarArgumentos<ObjetoEnPseudoD, Texto, Valor>(args);
			std::get<0>(targs)->FijarMetodo(
				std::get<1>(targs)->ObtenerTexto(),
				std::get<2>(targs)
			);
			return CrearNulo();
		});

		RegistrarProcedimiento(ambito, "__AgregarAtributo", [](auto args)
		{
			auto targs = AceptarArgumentos<ObjetoEnPseudoD, Texto, Valor>(args);
			std::get<0>(targs)->AgregarAtributo(
				std::get<1>(targs)->ObtenerTexto(),
				std::get<2>(targs)
			);
			return CrearNulo();
		});

		RegistrarProcedimiento(ambito, "__ObtenerAtributo", [](auto args)
		{
			auto targs = AceptarArgumentos<ObjetoEnPseudoD, Texto>(args);
			return std::get<0>(targs)->ObtenerAtributo(
				std::get<1>(targs)->ObtenerTexto()
			);
		});

		RegistrarProcedimiento(ambito, "__FijarAtributo", [](auto args)
		{
			auto targs = AceptarArgumentos<ObjetoEnPseudoD, Texto, Valor>(args);
			std::get<0>(targs)->FijarAtributo(
				std::get<1>(targs)->ObtenerTexto(),
				std::get<2>(targs)
			);
			return CrearNulo();
		});

		RegistrarProcedimiento(ambito, "__Aplicar", [](auto args)
		{
			auto targs = AceptarArgumentos<Valor, Arreglo>(args);
			return std::get<0>(targs)->RecibirMensaje(
				"llamar",
				std::get<1>(targs)->ObtenerArreglo()
			);
		});

		RegistrarProcedimiento(ambito, "__EnviarMensaje", [](auto args) -> ValorPtr
		{
			auto targs = AceptarArgumentos<Valor, Texto, Arreglo>(args);
			return std::get<0>(targs)->RecibirMensaje(
				std::get<1>(targs)->ObtenerTexto(),
				std::get<2>(targs)->ObtenerArreglo()
			);
		});

		Interprete inter(conf, ambito);
		Backtracker tok(NuevoTokenizador(Token::DatosFuente(1, 1, "<builtins.cpp::CrearClaseObjeto>")));
		tok.TokenizarFlujo(in);
		inter.Ejecutar(tok);
		return ambito->ObtenerVariable("Objeto");
	}

	void RegistrarBuiltins(
		AmbitoPtr ambito,
		ValorPtr claseObjeto,
		std::vector<PDCadena>& argumentosDelCLI
	)
	{
		const std::string programa = R"EOF(

clase Boole
	metodo estatico crear
	metodo estatico verdadero
	metodo estatico falso
finclase

metodo estatico Boole#crear
	devolver VERDADERO
finmetodo

metodo estatico Boole#verdadero
	devolver VERDADERO
finmetodo

metodo estatico Boole#falso
	devolver FALSO
finmetodo

clase Numero
	metodo estatico crear
finclase

metodo estatico Numero#crear
	devolver 0
finmetodo

clase Arreglo
	metodo estatico crear
	metodo estatico vacio
	metodo estatico crearCon
finclase

metodo estatico Arreglo#crear
	devolver yo#vacio
finmetodo

metodo estatico Arreglo#vacio
	devolver yo#crearCon
finmetodo

__AgregarMetodo: Arreglo, {crearCon}, ProcedimientoVarargs: 1, metodo: args
	devolver Aplicar: &__ArregloCon, args
finmetodo

clase Procedimiento
	metodo estatico crear
finclase

metodo estatico Procedimiento#crear
	__FallarConMensaje: {No se puede crear un procedimiento con #crear.}
finmetodo

clase Texto
	metodo estatico crear
finclase

metodo estatico Texto#crear
	devolver ""
finmetodo

clase EspacioDeNombres
	metodo estatico crear
finclase

metodo estatico EspacioDeNombres#crear
	devolver __CrearEspacioDeNombres
finmetodo

clase Referencia
	metodo estatico crear
finclase

metodo estatico Referencia#crear
	__FallarConMensaje: {No se puede crear una referencia con #crear.}
finmetodo

clase TipoNulo
	metodo estatico crear
finclase

metodo estatico TipoNulo#crear
	devolver NULO
finmetodo

		)EOF";

		ambito->CrearVariable("VERDADERO", CrearValor<Boole>(true));
		ambito->CrearVariable("FALSO", CrearValor<Boole>(false));
		ambito->CrearVariable("NULO", CrearNulo());

		RegistrarProcedimiento(ambito, "Aplicar", [](auto args)
		{
			auto targs = AceptarArgumentos<Valor, Arreglo>(args);
			return std::get<0>(targs)->RecibirMensaje(
				"llamar",
				std::get<1>(targs)->ObtenerArreglo()
			);
		});

		RegistrarProcedimiento(ambito, "ProcedimientoVarargs", [](auto args)
		{
			using size_type = typename decltype(args)::size_type;

			auto targs = AceptarArgumentos<Numero, Valor>(args);
			size_type baseArgs = static_cast<size_type>(
				std::get<0>(targs)->ObtenerEntero()
			);
			auto proc = std::get<1>(targs);
			return CrearValor<EnvolturaDeFuncion>([baseArgs, proc](auto args)
			{
				if(args.size() < baseArgs)
				{
					throw PDvar::ErrorDeSemantica("Se esperaban más argumentos");
				}
				std::vector<ValorPtr> normalArgs, variadic;
				for(size_type i = 0; i < baseArgs; i++)
				{
					normalArgs.push_back(args[i]);
				}
				for(size_type i = baseArgs; i < args.size(); i++)
				{
					variadic.push_back(args[i]);
				}
				normalArgs.push_back(CrearValor<Arreglo>(variadic));
				return proc->RecibirMensaje("llamar", normalArgs);
			});
		});

		RegistrarProcedimiento(ambito, "TipoDe", [ambito](auto args)
		{
			AceptarArgumentos<Valor>(args);
			auto valor = args[0];

			if(ValorEs<Boole>(valor))
			{
				return ambito->ObtenerVariable("Boole");
			}
			else if(ValorEs<Numero>(valor))
			{
				return ambito->ObtenerVariable("Numero");
			}
			else if(ValorEs<Arreglo>(valor))
			{
				return ambito->ObtenerVariable("Arreglo");
			}
			else if(ValorEs<Texto>(valor))
			{
				return ambito->ObtenerVariable("Texto");
			}
			else if(ValorEs<Referencia>(valor))
			{
				return ambito->ObtenerVariable("Referencia");
			}
			else if(ValorEs<Procedimiento>(valor)
				|| ValorEs<EnvolturaDeMetodo>(valor)
				|| ValorEs<EnvolturaDeFuncion>(valor))
			{
				return ambito->ObtenerVariable("Procedimiento");
			}
			else if(ValorEs<TipoNulo>(valor))
			{
				return ambito->ObtenerVariable("TipoNulo");
			}
			else
			{
				return valor->RecibirMensaje("__tipo", std::vector<ValorPtr> {});
			}
		});

		RegistrarProcedimiento(ambito, "__EnviarMensaje", [](auto args) -> ValorPtr
		{
			auto targs = AceptarArgumentos<Valor, Texto, Arreglo>(args);
			return std::get<0>(targs)->RecibirMensaje(
				std::get<1>(targs)->ObtenerTexto(),
				std::get<2>(targs)->ObtenerArreglo()
			);
		});

		RegistrarProcedimiento(ambito, "__AgregarMetodo", [](auto args)
		{
			auto targs = AceptarArgumentos<ObjetoEnPseudoD, Texto, Valor>(args);
			std::get<0>(targs)->AgregarMetodo(
				std::get<1>(targs)->ObtenerTexto(),
				std::get<2>(targs)
			);
			return CrearNulo();
		});

		RegistrarProcedimiento(ambito, "__ArregloCon", [](auto args)
		{
			return CrearValor<Arreglo>(args);
		});

		RegistrarProcedimiento(ambito, "__CrearEspacioDeNombres", [ambito](auto args)
		{
			return CrearValor<EspacioDeNombres>(ambito);
		});

		RegistrarProcedimiento(ambito, "__FallarConMensaje", [](auto args) -> ValorPtr
		{
			auto targs = AceptarArgumentos<Texto>(args);
			throw PDvar::ErrorDelNucleo(std::get<0>(targs)->ObtenerTexto());
		});

		RegistrarProcedimiento(ambito, "__ClonarObjeto", [](auto args) -> ValorPtr
		{
			auto targs = AceptarArgumentos<ObjetoEnPseudoD>(args);
			return std::get<0>(targs)->ClonarObjeto();
		});

		RegistrarProcedimiento(ambito, "__CompararObjeto", [](auto args) -> ValorPtr
		{
			auto targs = AceptarArgumentos<ObjetoEnPseudoD, ObjetoEnPseudoD>(args);
			return std::get<0>(targs)->CompararObjeto(std::get<1>(targs));
		});

		RegistrarProcedimiento(ambito, "__AbrirArchivo", [](auto args) -> ValorPtr
		{
			auto targs = AceptarArgumentos<Texto, Numero>(args);
			return CrearValor<Archivo>(
				std::get<0>(targs)->ObtenerTexto(),
				std::get<1>(targs)->ObtenerEntero()
			);
		});

		RegistrarProcedimiento(ambito, "__ByteATexto", [](auto args) -> ValorPtr
		{
			auto targs = AceptarArgumentos<Numero>(args);
			return CrearValor<Texto>(std::string(1, std::get<0>(targs)->ObtenerEntero()));
		});

		RegistrarProcedimiento(ambito, "__TextoAByte", [](auto args) -> ValorPtr
		{
			auto targs = AceptarArgumentos<Texto>(args);
			return CrearValor<Numero>(
				std::get<0>(targs)->ObtenerTexto()[0],
				Numero::MARCA_TIPO_ENTERO
			);
		});

		RegistrarProcedimiento(ambito, "__ByteEof", [](auto args) -> ValorPtr
		{
			return CrearValor<Numero>(
				std::char_traits<char>::eof(),
				Numero::MARCA_TIPO_ENTERO
			);
		});

		RegistrarProcedimiento(ambito, "__LeerCaracter", [](auto args) -> ValorPtr
		{
			EsperarNingunArgumento(args);
			return CrearValor<Numero>(
				std::cin.get(),
				Numero::MARCA_TIPO_ENTERO
			);
		});

		RegistrarProcedimiento(ambito, "__Capturar", [](auto args) -> ValorPtr
		{
			auto targs = AceptarArgumentos<Valor>(args);
			auto proc = std::get<0>(targs);

			auto inScope = std::make_shared<bool>(true);
			std::function<EnvolturaDeFuncion::TipoFuncion> f = [inScope](auto args) -> ValorPtr
			{
				EsperarNingunArgumento(args);
				if(*inScope)
				{
					ErrorParaCapturar err("Para Capturar");
					err.FijarIDDeCaptura(inScope);
					throw err;
				}
				else
				{
					throw PDvar::ErrorDelNucleo("No se puede escapar fuera de un bloque __Capturar");
				}
			};
			auto escape = CrearValor<EnvolturaDeFuncion>(f);

			std::vector<ValorPtr> cargs { escape };
			try
			{
				proc->RecibirMensaje("llamar", cargs);
				*inScope = false;
				return CrearValor<Boole>(true);
			}
			catch(const ErrorParaCapturar& err)
			{
				*inScope = false;
				if(err.ObtenerIDDeCaptura() == inScope)
				{
					return CrearValor<Boole>(false);
				}
				else
				{
					throw err;
				}
			}
		});

		auto arr = std::make_shared<pseudod::Arreglo>();
		for(const auto& argumento : argumentosDelCLI)
		{
			arr->ObtenerArreglo().push_back(
				pseudod::CrearValor<pseudod::Texto>(argumento)
			);
		}
		ambito->CrearVariable("__Argv", arr);

		ambito->CrearVariable("__Impl", CrearValor<pseudod::Texto>("C++ Original"));

		Interprete interp(ConfInterprete{claseObjeto}, ambito);
		Backtracker tok(NuevoTokenizador(Token::DatosFuente(1, 1, "<builtins.cpp::RegistrarBuiltins>")));
		std::istringstream in(programa);
		tok.TokenizarFlujo(in);
		interp.Ejecutar(tok);
	}
}
