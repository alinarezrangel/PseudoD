/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*****                                                                  *****
***** Página oficial de PseudoD en:                                    *****
*****     http://pseudod.sourceforge.net/                              *****
*****                                                                  *****
****************************************************************************
Copyright 2016-2018 Alejandro Linarez Rangel

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

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <map>
#include <cstdlib>
#include <cassert>
#include <tuple>

#include "NEA/interno/data.hh"
#include "NEA/interno/token.hh"
#include "NEA/interno/tokenizer.hh"

#include "NEA/interno/memory.hh"
#include "NEA/interno/memory_types.hh"
#include "NEA/interno/new_interpreter.hh"
#include "NEA/interno/new_tokenizer.hh"
#include "NEA/interno/builtins.hh"

#include "interprete.hh"

enum class OpcionesDelTokenizador : unsigned int
{
	Nada = 0,
	EscaparComoC = 1,
	UnaSolaLinea = 2,
	EmitirComentarios = 4,
};

OpcionesDelTokenizador operator|(OpcionesDelTokenizador a, OpcionesDelTokenizador b)
{
	return static_cast<OpcionesDelTokenizador>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

bool operator&(OpcionesDelTokenizador a, OpcionesDelTokenizador b)
{
	return static_cast<bool>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

OpcionesDelTokenizador& operator|=(OpcionesDelTokenizador& a, OpcionesDelTokenizador b)
{
	a = a | b;
	return a;
}

struct ContextoCLI
{
	PDCadena rutaBEPD = "";
	PDCadena rutaNEA = "";
	PDCadena texto = "";
	bool usarV3 = false;
	OpcionesDelTokenizador opcionesTokenizador = OpcionesDelTokenizador::Nada;
};

struct OpcionCLI
{
	int (*manejador)(PDCadena, ContextoCLI&);
	bool aceptaArgumento;
};

int OptMostrarAyuda(PDCadena arg, ContextoCLI&)
{
	PDCadena ayuda = "PseudoD versión " PSEUDOD_VERSION R"delim(
Uso:  pseudod (<archivo>|-c <programa>) [-l <ruta-NEA>] [(-b|-B) <ruta-BEPD>]
      pseudod -v
      pseudod (--help|-h|-a|--ayuda|-help)

Opciones:

-c <programa>             : Ejecuta el texto <programa>
-l <ruta-NEA>             : Fija <ruta-NEA> como el NEA de PseudoD
-b <ruta-BEPD>            : Fija <ruta-BEPD> como la ruta a la BEPD de PseudoD. Si
                            <ruta-BEPD> no termina en barra (`/`) entonces lo agrega.
-B <ruta-BEPD>            : Fija <ruta-BEPD> como la ruta a la BEPD de PseudoD.
-X expr                   : Activa el experimento expr.
--tokenizar-v3 <archivo>  : Tokeniza el archivo <archivo>
--Mtokenizar-v3 <opt>     : Activa la opción <opt> del tokenizador.
-v                        : Muestra la versión de PseudoD y termina.
-h                        : Muestra esta ayuda y termina (también --help, -a y --ayuda)

Si la opción `-l` no es especificada, entonces <ruta-NEA> toma el valor de:

  1. La variable de entorno `$RUTA_PSEUDOD_LIB`, o si no existe:
  2. El archivo `/opt/pseudod/lib/libpseudod.so`.

Similarmente, si la opción `-b` no es especificada, entonces <ruta-BEPD> toma
el valor de:

  1. La variable de entorno `$RUTA_PSEUDOD`, o si no existe:
  2. La variable de entorno `$RUTA_PSEUDOD_BEPD`, o si no existe:
  3. El directorio `/opt/pseudod/bepd/bepd/`.

El sistema de experimentos te permite probar características experimentales o
en etapa de prueba. Actualmente están implementados los siguientes experimentos:

-X v3x   : Ejecuta el programa utilizando la siguiente versión del lenguaje.
)delim";

	std::cout << ayuda << std::flush;

	return 2;
}

int OptHabilitarExperimento(PDCadena arg, ContextoCLI& contexto)
{
	if(arg == "v3x")
	{
		contexto.usarV3 = true;
		return 0;
	}
	else
	{
		std::cerr << "Error: experimento \"" << arg << "\" no detectado" << std::endl;
	}

	return 1;
}

int OptRutaBEPD(PDCadena arg, ContextoCLI& ctx)
{
	if((arg.size() > 0) && (arg.back() != '/'))
	{
		arg += '/';
	}

	ctx.rutaBEPD = arg;

	return 0;
}

int OptRutaBEPDPura(PDCadena arg, ContextoCLI& ctx)
{
	ctx.rutaBEPD = arg;

	return 0;
}

int OptRutaNEA(PDCadena arg, ContextoCLI& ctx)
{
	ctx.rutaNEA = arg;

	return 0;
}

int OptMostrarVersion(PDCadena arg, ContextoCLI& ctx)
{
	std::cout << "PseudoD " PSEUDOD_VERSION << std::endl;

	return 2;
}

int OptEjecutarTexto(PDCadena arg, ContextoCLI& ctx)
{
	ctx.texto = arg;

	return 0;
}

PDCadena LeerBloque(std::istream& in, PDCadena prompt)
{
	PDCadena buffer = "";
	PDCadena linea = " ";

	while(in && (linea != ""))
	{
		std::cout << prompt << std::flush;

		std::getline(std::cin, linea, '\n');

		buffer += linea + '\n';
	}

	return buffer;
}

struct EstadoPD3
{
	pseudod::AmbitoPtr ambito;
	std::shared_ptr<pseudod::Interprete> interprete;
};

EstadoPD3 InicializarPseudoD3(std::string nombreDelArchivo, ContextoCLI contexto)
{
	pseudod::ConfInterprete conf;
	conf.ClaseObjeto = pseudod::CrearClaseObjeto();

	auto ambito = std::make_shared<pseudod::Ambito>();
	ambito->CrearVariable("Objeto", conf.ClaseObjeto);
	pseudod::RegistrarBuiltins(ambito, conf.ClaseObjeto);

	return EstadoPD3 {
		ambito,
		std::make_shared<pseudod::Interprete>(
			conf,
			std::make_shared<pseudod::Ambito>(ambito)
		)
	};
}

void EjecutarConPseudoD3(std::istream& in, EstadoPD3 estado)
{
	pseudod::Backtracker tok;
	tok.TokenizarFlujo(in);
	estado.interprete->Ejecutar(tok);
}

void InicializarPseudoD2(std::string nombreDelArchivo, ContextoCLI contexto)
{
	try
	{
		pseudod::iniciar(
			contexto.rutaNEA,
			contexto.rutaBEPD,
			nombreDelArchivo
		);
	}
	catch(const PDvar::Error& err)
	{
		throw PDvar::ErrorDelNucleo(
			"Error: No se pudo iniciar el intérprete PseudoD: " + err.Mensaje()
		);
	}
	catch(...)
	{
		throw PDvar::ErrorDelNucleo(
			"Error: No se pudo iniciar el intérprete PseudoD"
		);
	}
}

void TokenizarFlujoV3(std::istream& in, OpcionesDelTokenizador opciones)
{
	auto EscaparS = [opciones](std::string& txt) -> std::string
	{
		std::string res;
		for(char c : txt)
		{
			if(std::isprint(c) && c != '\\' && c != '\"' && c != '|')
			{
				res += c;
			}
			else if(c == '\\' || c == '\"' || c == '|')
			{
				res += '\\';
				res += c;
			}
			else if(c == '\n' || c == '\r' || c == '\t')
			{
				switch(c)
				{
					case '\n':
						res += "\\n";
						break;
					case '\r':
						res += "\\r";
						break;
					case '\t':
						res += "\\r";
						break;
				}
			}
			else
			{
				std::stringstream hexstream;
				int byte = static_cast<int>(static_cast<unsigned char>(c));
				hexstream << "\\x" << std::hex << std::setfill('0') << std::setw(2) << byte;
				if(!(opciones & OpcionesDelTokenizador::EscaparComoC))
				{
					hexstream << ";";
				}
				res += hexstream.str();
			}
		}
		return res;
	};

	auto TextoExprS = [EscaparS](std::string txt) -> std::string
	{
		return "\"" + EscaparS(txt) + "\"";
	};

	auto SimboloExprS = [EscaparS](std::string sym) -> std::string
	{
		return "|" + EscaparS(sym) + "|";
	};

	auto NumeroExprS = [](long long v) -> std::string
	{
		std::stringstream stream;
		stream << std::dec << v;
		return stream.str();
	};

	auto PalabraExprS = [NumeroExprS](pseudod::NMemonicoProxy proxy) -> std::string
	{
		std::stringstream stream;
		stream << "(";
		for(auto iter = proxy.begin; iter != proxy.end; iter++)
		{
			pseudod::NMemonico::Palabra pal = iter->second;
			switch(pal)
			{
				case pseudod::NMemonico::PD_ADQUIRIR:
					stream << "adquirir";
					break;
				case pseudod::NMemonico::PD_PUNTERO:
					stream << "puntero";
					break;
				case pseudod::NMemonico::PD_LIBERAR:
					stream << "liberar";
					break;
				case pseudod::NMemonico::PD_INSTANCIA:
					stream << "instancia";
					break;
				case pseudod::NMemonico::PD_CLASE:
					stream << "clase";
					break;
				case pseudod::NMemonico::PD_HEREDAR:
					stream << "heredar";
					break;
				case pseudod::NMemonico::PD_REDIRECCIONAR:
					stream << "redireccionar";
					break;
				case pseudod::NMemonico::PD_MIENTRAS:
					stream << "mientras";
					break;
				case pseudod::NMemonico::PD_INCREMENTAR_PUNTERO:
					stream << "incrementar_puntero";
					break;
				case pseudod::NMemonico::PD_DECREMENTAR_PUNTERO:
					stream << "decrementar_puntero";
					break;
				case pseudod::NMemonico::PD_ESCRIBIR:
					stream << "escribir";
					break;
				case pseudod::NMemonico::PD_EJECUTAR:
					stream << "ejecutar";
					break;
				case pseudod::NMemonico::PD_NUEVALINEA:
					stream << "nl";
					break;
				case pseudod::NMemonico::PD_FIJAR:
					stream << "fijar";
					break;
				case pseudod::NMemonico::PD_CLONAR:
					stream << "clonar";
					break;
				case pseudod::NMemonico::PD_CLONAR_CON:
					stream << "con-clonar";
					break;
				case pseudod::NMemonico::PD_FIN_CLONAR:
					stream << "finclonar";
					break;
				case pseudod::NMemonico::PD_OPERADOR_A:
					stream << "a";
					break;
				case pseudod::NMemonico::PD_OPERADOR_IGUAL:
					stream << "=?";
					break;
				case pseudod::NMemonico::PD_OPERADOR_FIJAR:
					stream << "=*";
					break;
				case pseudod::NMemonico::PD_OPERADOR_SON:
					stream << "son";
					break;
				case pseudod::NMemonico::PD_OPERADOR_IGUALES:
					stream << "iguales";
					break;
				case pseudod::NMemonico::PD_OPERADOR_DIFERENTES:
					stream << "diferentes";
					break;
				case pseudod::NMemonico::PD_OPERADOR_Y:
					stream << "y";
					break;
				case pseudod::NMemonico::PD_OPERADOR_COMPARAR:
					stream << "comparar";
					break;
				case pseudod::NMemonico::PD_OPERADOR_NO:
					stream << "no";
					break;
				case pseudod::NMemonico::PD_OPERADOR_EJECUTAR:
					stream << "ejecutar-op";
					break;
				case pseudod::NMemonico::PD_OPERADOR_SON_IGUALES:
					stream << "¿son_iguales?";
					break;
				case pseudod::NMemonico::PD_OPERADOR_TANTO:
					stream << "tanto";
					break;
				case pseudod::NMemonico::PD_OPERADOR_COMO:
					stream << "como";
					break;
				case pseudod::NMemonico::PD_OPERADOR_ALGUN:
					stream << "algun";
					break;
				case pseudod::NMemonico::PD_OPERADOR_O:
					stream << "o";
					break;
				case pseudod::NMemonico::PD_LEER:
					stream << "leer";
					break;
				case pseudod::NMemonico::PD_UTILIZAR:
					stream << "utilizar";
					break;
				case pseudod::NMemonico::PD_LLAMAR:
					stream << "llamar";
					break;
				case pseudod::NMemonico::PD_SEPARADOR_DE_ARGUMENTOS:
					stream << "|,|";
					break;
				case pseudod::NMemonico::PD_CON:
					stream << "con";
					break;
				case pseudod::NMemonico::PD_FUNCION:
					stream << "funcion";
					break;
				case pseudod::NMemonico::PD_FIN_FUNCION:
					stream << "finfuncion";
					break;
				case pseudod::NMemonico::PD_FIN_SI:
					stream << "finsi";
					break;
				case pseudod::NMemonico::PD_FIN_BUCLE:
					stream << "finmientras";
					break;
				case pseudod::NMemonico::PD_FIN_CLASE:
					stream << "finclase";
					break;
				case pseudod::NMemonico::PD_EMPUJAR:
					stream << "empujar";
					break;
				case pseudod::NMemonico::PD_DEVOLVER:
					stream << "devolver";
					break;
				case pseudod::NMemonico::PD_SACAR:
					stream << "sacar";
					break;
				case pseudod::NMemonico::PD_USAR_PILA:
					stream << "usar_pila";
					break;
				case pseudod::NMemonico::PD_CREAR_PILA:
					stream << "crear_pila";
					break;
				case pseudod::NMemonico::PD_NECESITAS:
					stream << "necesitas";
					break;
				case pseudod::NMemonico::PD_SI:
					stream << "si";
					break;
				case pseudod::NMemonico::PD_SINO:
					stream << "sino";
					break;
				case pseudod::NMemonico::PD_SI_NO:
					stream << "si_no";
					break;
				case pseudod::NMemonico::PD_SON_IGUALES:
					stream << "comparar_i";
					break;
				case pseudod::NMemonico::PD_ESCRIBIR_ESPACIO:
					stream << "escribir_esp";
					break;
				case pseudod::NMemonico::PD_SALIR:
					stream << "salir";
					break;
				case pseudod::NMemonico::PD_COMENTARIO:
					stream << "com";
					break;
				case pseudod::NMemonico::PD_INTENTA:
					stream << "intenta";
					break;
				case pseudod::NMemonico::PD_ATRAPA_ERROR:
					stream << "atrapar";
					break;
				case pseudod::NMemonico::PD_FIN_INTENTA:
					stream << "finintenta";
					break;
				case pseudod::NMemonico::PD_IMPLEMENTAR:
					stream << "implementa";
					break;
				case pseudod::NMemonico::PD_CLASE_ATRIBUTO:
					stream << "atributo-cls";
					break;
				case pseudod::NMemonico::PD_CLASE_METODO:
					stream << "metodo-cls";
					break;
				case pseudod::NMemonico::PD_CLASE_PUNTERO:
					stream << "puntero-cls";
					break;
				case pseudod::NMemonico::PD_CLASE_METODO_ESTATICO:
					stream << "estatico";
					break;
				case pseudod::NMemonico::PD_FIN_ARGUMENTOS:
					stream << "finargs";
					break;
				case pseudod::NMemonico::PD_COMPARAR_IGUAL:
					stream << "=";
					break;
				case pseudod::NMemonico::PD_COMPARAR_MENOR:
					stream << "<";
					break;
				case pseudod::NMemonico::PD_COMPARAR_MENOR_O_IGUAL:
					stream << "<=";
					break;
				case pseudod::NMemonico::PD_COMPARAR_MAYOR:
					stream << ">";
					break;
				case pseudod::NMemonico::PD_COMPARAR_MAYOR_O_IGUAL:
					stream << ">=";
					break;
				case pseudod::NMemonico::PD_COMPARAR_DISTINTO:
					stream << "!=";
					break;
				case pseudod::NMemonico::PD_COMPARAR_MINIMO:
					stream << "<?";
					break;
				case pseudod::NMemonico::PD_COMPARAR_MAXIMO:
					stream << "?>";
					break;
				case pseudod::NMemonico::PD_OPERADOR_ES:
					stream << "es";
					break;
				case pseudod::NMemonico::PD_CLASE_ABSTRACTA:
					stream << "abstracta";
					break;
				case pseudod::NMemonico::PD_OPERADOR_REDIRECCIONAR_A:
					stream << "redireccionar-a";
					break;
				case pseudod::NMemonico::PD_ENVIAR_MENSAJE:
					stream << "|#|";
					break;
				case pseudod::NMemonico::PD_OPERADOR_LLAMAR:
					stream << ":";
					break;
				case pseudod::NMemonico::PD_PARENTESIS_IZQUIERDO:
					stream << "|(|";
					break;
				case pseudod::NMemonico::PD_PARENTESIS_DERECHO:
					stream << "|)|";
					break;
				case pseudod::NMemonico::PD_REFERENCIA_VARIABLE:
					stream << "&";
					break;
				case pseudod::NMemonico::PD_AUTOEJECUTA_VARIABLE:
					stream << "%";
					break;
				case pseudod::NMemonico::PD_REFERENCIAR:
					stream << "ref";
					break;
				case pseudod::NMemonico::PD_DESREFERENCIAR:
					stream << "desref";
					break;
				case pseudod::NMemonico::PD_NEA:
					stream << "NEA";
					break;
				case pseudod::NMemonico::PD_ALIAS:
					stream << "ALIAS";
					break;
				case pseudod::NMemonico::PD_OTRO:
					stream << "OTRO";
					break;
			}
			stream << " ";
		}
		stream << ")";
		return stream.str();
	};

	auto ComoExpresionS = [=](pseudod::Token tk) -> std::string
	{
		std::string valor;
		switch(tk.ObtenerTipo())
		{
			case pseudod::Token::SinTipo:
				return "(sin-tipo)";
			case pseudod::Token::NMemonico:
				return
					"(nmemonico " +
					PalabraExprS(tk.ObtenerNMemonico()) +
					" " +
					TextoExprS(tk.ObtenerNMemonico().original) +
					")";
			case pseudod::Token::Literal:
				valor = tk.ObtenerValorLiteral().valor;
				switch(tk.ObtenerValorLiteral().tipo)
				{
					case pseudod::Token::ValorLiteral::Cadena:
						return "(texto " + TextoExprS(valor) + ")";
					case pseudod::Token::ValorLiteral::Numero:
						return "(numero " + NumeroExprS(std::stoll(valor)) + ")";
					case pseudod::Token::ValorLiteral::Identificador:
						return "(id " + SimboloExprS(valor) + ")";
					case pseudod::Token::ValorLiteral::Comentario:
						return "(com " + TextoExprS(valor) + ")";
					case pseudod::Token::ValorLiteral::CuerpoDeCodigo:
						assert(0);
				}
		}
		return "(error)";
	};

	pseudod::NuevoTokenizador tok;
	if(opciones & OpcionesDelTokenizador::EmitirComentarios)
	{
		tok.ProducirComentarios();
	}
	std::cout << "(";
	while(true)
	{
		std::pair<std::vector<pseudod::Token>, bool> res;
		try
		{
			res = tok.LeerToken(in);
		}
		catch(const PDvar::Error& err)
		{
			std::cerr
				<< "Error:\n\t"
				<< err.Mensaje() << std::endl;
			return;
		}
		if(res.second)
		{
			auto tokens = res.first;
			for(auto token : tokens)
			{
				std::cout << ComoExpresionS(token);
				if(opciones & OpcionesDelTokenizador::UnaSolaLinea)
				{
					std::cout << ' ';
				}
				else
				{
					std::cout << '\n';
				}
			}
		}
		else
		{
			break;
		}
	}
	std::cout << ")" << std::endl;
}

int OptTokenizarV3(PDCadena archivo, ContextoCLI& contexto)
{
	std::ifstream in(archivo);
	if(!in)
	{
		std::cerr << "No se pudo abrir el archivo" << std::endl;
		return 1;
	}
	TokenizarFlujoV3(in, contexto.opcionesTokenizador);
	return 2;
}

int OptOpcionTokenizadorV3(PDCadena opcion, ContextoCLI& contexto)
{
	OpcionesDelTokenizador opt = OpcionesDelTokenizador::Nada;
	if(opcion == "EscaparComoC")
	{
		opt = OpcionesDelTokenizador::EscaparComoC;
	}
	else if(opcion == "UnaSolaLinea")
	{
		opt = OpcionesDelTokenizador::UnaSolaLinea;
	}
	else if(opcion == "EmitirComentarios")
	{
		opt = OpcionesDelTokenizador::EmitirComentarios;
	}
	else
	{
		std::cerr << "Opcion del tokenizador no reconocida: " << opcion << std::endl;
		return 1;
	}
	contexto.opcionesTokenizador |= opt;
	return 0;
}

int MainV3(PDCadena archivoPrincipal, bool interactivo, ContextoCLI contexto)
{
	try
	{
		EstadoPD3 estado = InicializarPseudoD3(archivoPrincipal, contexto);

		if(contexto.texto != "")
		{
			std::istringstream in(contexto.texto);
			EjecutarConPseudoD3(in, estado);
		}
		else if(interactivo)
		{
			std::cout << "PseudoD versión " PSEUDOD_VERSION R"delim(
Interprete interactivo de PseudoD v3x. Ingresa el programa para almacenarlo y una
línea en blanco para ejecutarlo.

NOTA: Estás actualmente ejecutando PseudoD 3.x, una versión experimental de la
que será la siguiente gran versión del lenguaje.
)delim";

			while(pseudod::Ejecutar && std::cin)
			{
				PDCadena bloque = LeerBloque(std::cin, "v3 @> ");
				std::istringstream in(bloque);

				try
				{
					EjecutarConPseudoD3(in, estado);
				}
				catch(const PDvar::Error& err)
				{
					std::cerr << "Error:\n\t" << err.Mensaje() << std::endl;
				}
			}

			if(!std::cin)
			{
				std::cout << std::endl;
			}
		}
		else
		{
			std::ifstream in(archivoPrincipal);

			if(!in)
			{
				throw PDvar::ErrorDelNucleo(
					"No se pudo abrir el archivo " + archivoPrincipal
				);
			}

			EjecutarConPseudoD3(in, estado);
		}
	}
	catch(const PDvar::Error& err)
	{
		std::cerr
			<< "Error:\n\t"
			<< err.Mensaje() << std::endl;
		return 1;
	}
	return 0;
}

int MainV2(PDCadena archivoPrincipal, bool interactivo, ContextoCLI contexto)
{
	try
	{
		InicializarPseudoD2(archivoPrincipal, contexto);

		if(contexto.texto != "")
		{
			pseudod::ejecutar(contexto.texto);
		}
		else if(interactivo)
		{
			std::cout << "PseudoD versión " PSEUDOD_VERSION R"delim(
Interprete interactivo de PseudoD. Ingresa el programa para almacenarlo y una
línea en blanco para ejecutarlo.)delim" << std::endl;

			while(pseudod::Ejecutar && std::cin)
			{
				PDCadena bloque = LeerBloque(std::cin, "@> ");

				pseudod::ejecutar(bloque);
			}

			if(!std::cin)
			{
				std::cout << std::endl;
			}
		}
		else
		{
			std::ifstream in(archivoPrincipal);

			if(!in)
			{
				throw PDvar::ErrorDelNucleo(
					"No se pudo abrir el archivo " + archivoPrincipal
				);
			}

			in.imbue(std::locale());

			pseudod::ejecutar(in);
		}
	}
	catch(const PDvar::Error& err)
	{
		std::cerr
			<< "Error en el archivo \""
			<< pseudod::DATOS_INT.ObtenerVariable("__ARCH__")
			<< "\":" << std::endl
			<< err.Mensaje() << std::endl;
		return 1;
	}
	catch(const std::exception& err)
	{
		std::cerr
			<< "Error en el archivo \""
			<< pseudod::DATOS_INT.ObtenerVariable("__ARCH__")
			<< "\":" << std::endl
			<< "(error nativo) " << err.what() << std::endl;
		return 1;
	}
	catch(const std::string& err)
	{
		std::cerr
			<< "Error en el archivo \""
			<< pseudod::DATOS_INT.ObtenerVariable("__ARCH__")
			<< "\":" << std::endl
			<< "(error antiguo) " << err << std::endl;
		return 1;
	}
	catch(...)
	{
		std::cerr
			<< "Error en el archivo \""
			<< pseudod::DATOS_INT.ObtenerVariable("__ARCH__")
			<< "\":" << std::endl
			<< "Error sin detectar!" << std::endl;
		return 1;
	}

	return pseudod::terminar();
}

int main(int argc, char* argv[])
{
	std::map<std::string, OpcionCLI> opciones;

	const std::vector<std::string> entornoRutaBEPD =
	{
		"RUTA_PSEUDOD",
		"RUTA_PSEUDOD_BEPD"
	};
	const PDCadena fallbackRutaBEPD = "/opt/pseudod/bepd/bepd/";

	const std::vector<std::string> entornoRutaNEA =
	{
		"RUTA_PSEUDOD_LIB"
	};
	const PDCadena fallbackRutaNEA = "/opt/pseudod/lib/libpseudod.so";

	bool interactivo = false;

	opciones["-h"] =
	opciones["-a"] =
	opciones["--help"] =
	opciones["-help"] =
	opciones["--ayuda"] = OpcionCLI {&OptMostrarAyuda, false};

	opciones["-X"] =
	opciones["--experimento"] = OpcionCLI {&OptHabilitarExperimento, true};

	opciones["-l"] =
	opciones["--ruta-nea"] = OpcionCLI {&OptRutaNEA, true};

	opciones["-b"] =
	opciones["--ruta-bepd"] = OpcionCLI {&OptRutaBEPD, true};

	opciones["-B"] =
	opciones["--ruta-bepd="] = OpcionCLI {&OptRutaBEPDPura, true};

	opciones["-v"] =
	opciones["--version"] = OpcionCLI {&OptMostrarVersion, false};

	opciones["-c"] =
	opciones["--ejecutar"] = OpcionCLI {&OptEjecutarTexto, true};

	opciones["--tokenizar-v3"] = OpcionCLI {&OptTokenizarV3, true};
	opciones["--Mtokenizar-v3"] = OpcionCLI {&OptOpcionTokenizadorV3, true};

	PDCadena archivoPrincipal = "";

	ContextoCLI contexto;

	contexto.rutaNEA = fallbackRutaNEA;

	for(const PDCadena& var : entornoRutaNEA)
	{
		char* env = std::getenv(var.c_str());

		if(env)
		{
			contexto.rutaNEA = env;

			break;
		}
	}

	contexto.rutaBEPD = fallbackRutaBEPD;

	for(const PDCadena& var : entornoRutaBEPD)
	{
		char* env = std::getenv(var.c_str());

		if(env)
		{
			contexto.rutaBEPD = env;

			break;
		}
	}

	for(int i = 1; i < argc; i++)
	{
		PDCadena arg = argv[i];

		if(opciones.count(arg) > 0)
		{
			OpcionCLI c = opciones[arg];
			PDCadena arg_param = "";

			if(c.aceptaArgumento)
			{
				if((i + 1) >= argc)
				{
					std::cerr
						<< "Error: se esperaba un argumento para "
						<< arg
						<< std::endl;

					return 1;
				}

				arg_param = argv[++i];
			}

			int retval = (*c.manejador)(arg_param, contexto);

			if(retval == 1)
			{
				std::cerr << "Error ejecutando el argumento" << std::endl;

				return 1;
			}

			if(retval == 2)
			{
				return 0;
			}
		}
		else if(archivoPrincipal == "")
		{
			archivoPrincipal = arg;
		}
		else
		{
			std::cerr << "Parametro sin opción inesperado" << std::endl;

			return 1;
		}
	}

	if(archivoPrincipal == "")
	{
		archivoPrincipal = "<entrada>";
		interactivo = true;
	}

	if(contexto.texto != "")
	{
		archivoPrincipal = "<texto>";
	}

	if(contexto.usarV3)
	{
		return MainV3(archivoPrincipal, interactivo, contexto);
	}
	else
	{
		return MainV2(archivoPrincipal, interactivo, contexto);
	}
}
