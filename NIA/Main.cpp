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
#include <map>
#include <cstdlib>

#include "NEA/interno/data.hh"
#include "NEA/interno/token.hh"
#include "NEA/interno/tokenizer.hh"

#include "interprete.hh"

struct ContextoCLI
{
	PDCadena rutaBEPD;
	PDCadena rutaNEA;
	PDCadena texto;
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

-c <programa>   : Ejecuta el texto <programa>
-l <ruta-NEA>   : Fija <ruta-NEA> como el NEA de PseudoD
-b <ruta-BEPD>  : Fija <ruta-BEPD> como la ruta a la BEPD de PseudoD. Si
                  <ruta-BEPD> no termina en barra (`/`) entonces lo agrega.
-B <ruta-BEPD>  : Fija <ruta-BEPD> como la ruta a la BEPD de PseudoD.
-v              : Muestra la versión de PseudoD y termina.
-h              : Muestra esta ayuda y termina (también --help, -a y --ayuda)

Si la opción `-l` no es especificada, entonces <ruta-NEA> toma el valor de:

  1. La variable de entorno `$RUTA_PSEUDOD_LIB`, o si no existe:
  2. El archivo `/opt/pseudod/lib/libpseudod.so`.

Similarmente, si la opción `-b` no es especificada, entonces <ruta-BEPD> toma
el valor de:

  1. La variable de entorno `$RUTA_PSEUDOD`, o si no existe:
  2. La variable de entorno `$RUTA_PSEUDOD_BEPD`, o si no existe:
  3. El directorio `/opt/pseudod/bepd/bepd/`.
)delim";

	std::cout << ayuda << std::flush;

	return 2;
}

int OptHabilitarExperimento(PDCadena arg, ContextoCLI&)
{
	std::cerr << "Error: experimento \"" << arg << "\" no detectado" << std::endl;

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

int OptActivarLinter(PDCadena arg, ContextoCLI& ctx)
{
	std::cerr << "Error: Linter no implementado aún" << std::endl;

	return 1;
}

int OptEjecutarTexto(PDCadena arg, ContextoCLI& ctx)
{
	ctx.texto = arg;

	return 1;
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

	opciones["-n"] =
	opciones["--linter"] = OpcionCLI {&OptActivarLinter, false};

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

	try
	{
		pseudod::iniciar(
			contexto.rutaNEA,
			contexto.rutaBEPD,
			archivoPrincipal
		);
	}
	catch(...)
	{
		std::cerr
			<< "Error: No se pudo iniciar el intérprete PseudoD"
			<< std::endl;

		return 1;
	}

	try
	{
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
				std::cerr
					<< "Error: No se pudo abrir el archivo "
					<< archivoPrincipal << std::endl;

				return 1;
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
	}
	catch(const std::exception& err)
	{
		std::cerr
			<< "Error en el archivo \""
			<< pseudod::DATOS_INT.ObtenerVariable("__ARCH__")
			<< "\":" << std::endl
			<< "(error nativo) " << err.what() << std::endl;
	}
	catch(const std::string& err)
	{
		std::cerr
			<< "Error en el archivo \""
			<< pseudod::DATOS_INT.ObtenerVariable("__ARCH__")
			<< "\":" << std::endl
			<< "(error antiguo) " << err << std::endl;
	}
	catch(...)
	{
		std::cerr
			<< "Error en el archivo \""
			<< pseudod::DATOS_INT.ObtenerVariable("__ARCH__")
			<< "\":" << std::endl
			<< "Error sin detectar!" << std::endl;
	}

	return pseudod::terminar();
}
