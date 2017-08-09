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

/*
Define la macro INTERACTIVO a 1 para un interprete en linea de comandos
https://sourceforge.net/projects/pseudod/
*/
#include "interprete.hh"

namespace pseudod
{
	bool Ejecutar = true;
	PDvar::PDDatos DATOS_INT;
	std::vector<int> AMBITO(2);
	inic_nea iniciar_nea;
	ejec_nea ejecutar_nea;
	liber_nea liberar_nea;
	void* coneccion_nea;

	PDCadena iniciar(PDCadena nea, PDCadena bepd, PDCadena main)
	{
		coneccion_nea = dlopen(nea.c_str(), RTLD_LAZY);

		if(coneccion_nea == NULL)
		{
			return "Error al conectar con el NEA";
		}

		*(void**) (&iniciar_nea) = dlsym(coneccion_nea, "PDInicializar");
		*(void**) (&ejecutar_nea) = dlsym(coneccion_nea, "PDEjecutar");
		*(void**) (&liberar_nea) = dlsym(coneccion_nea, "PDFinal");

		if((iniciar_nea == NULL) || (ejecutar_nea == NULL) || (liberar_nea == NULL))
		{
			dlclose(coneccion_nea);

			return "Error al adquirir el NEA";
		}

		iniciar_nea(
			(*DATOS_INT.nombrev),
			(*DATOS_INT.valorv),
			(*DATOS_INT.nombrep),
			(*DATOS_INT.nvapunt),
			(*DATOS_INT.pilas),
			procesar
		);

		DATOS_INT.CrearVariable("__MAIN__", true, 0, main);
		DATOS_INT.CrearVariable("__LIB__", true, 0, bepd);
		DATOS_INT.CrearVariable("__ARCH__", true, 0, main);
		DATOS_INT.CrearVariable("VG_PILA_ACTUAL", true, 0, "0");
		DATOS_INT.CrearVariable("VG_NUMERO_PILAS", true, 0, "0");
		DATOS_INT.CrearVariable("PSEUDOD", true, 0, PSEUDOD_VERSION);

		DATOS_INT.PROCESAR = procesar;
		DATOS_INT.PROCESO = ejecutar_nea;

		try
		{
			ejecutar("utilizar " + DATOS_INT.ObtenerVariable("__LIB__") + "builtins.pseudo");
		}
		catch(const PDvar::Error& e)
		{
			return e.Mensaje();
		}
		catch(const std::exception& e)
		{
			std::string errStr = "";

			if((e.what() == PDCadena("stoi")) || (e.what() == PDCadena("stoll")))
			{
				errStr = "Error al convertir numeros";
			}
			else
			{
				errStr = e.what();
			}

			return errStr;
		}
		catch(std::string e)
		{
			return e;
		}
		catch(...)
		{
			return "Error no identificado";
		}

		return "Ok";
	}

	int terminar(void)
	{
		liberar_nea();

		return dlclose(coneccion_nea);
	}

	void ejecutar(PDCadena linea)
	{
		std::istringstream in(linea);
		Tokenizador tkz(in);

		while(Ejecutar && (!tkz.FinDelFlujo()))
			procesar(tkz, ejecutar_nea);
	}

	void ejecutar(std::istream& entrada)
	{
		Tokenizador tkz(entrada);

		while(Ejecutar && (!tkz.FinDelFlujo()))
			procesar(tkz, ejecutar_nea);
	}

	void procesar(
		Tokenizador& e,
		PDFuncionNEA FUNCION
	)
	{
		using namespace Utilidades;

		if(!Ejecutar)
			return;

		Token tok;

		e >> tok;

		if(e.FinDelFlujo())
		{
			// Se termino el flujo y tratamos de leer mas de lo que contenia,
			// debido a la implementacion de pseudod::Tokenizador::ExtraerToken,
			// en este caso el token tok no sera modificado y por ello su tipo
			// sera Token::SinTipo, se asume que el bucle que esta procesando este
			// flujo termina cuando se detecta un EOF, entonces solo debemos
			// terminar esta llamada:

			return;
		}

		if(tok.ObtenerTipo() == Token::SinTipo)
		{
			throw PDvar::ErrorDelNucleo(
				"Error en el parser: No se pudo extraer el siguiente token"
			);
		}

		if(Tokens::EsComentario(tok))
		{
			return;
		}

		if(!Tokens::EsNMemonico(tok))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en el parser: se esperaba un nmemonico"
			);
		}

		procesar(tok, e, FUNCION);
	}

	void procesar(
		Token tok,
		Tokenizador& e,
		PDFuncionNEA FUNCION
	)
	{
		using namespace Utilidades;

		if(!Ejecutar)
			return;

		NMemonicoProxy proxy = tok.ObtenerNMemonico();

		/*
		std::cout
			<< "@ "
			<< proxy.original
			<< " "
			<< DATOS_INT.ObtenerVariable("__ARCH__")
			<< ":"
			<< tok.ObtenerDatosFuente().linea
			<< std::endl;
		*/

		if(proxy == NMemonico::PD_ADQUIRIR)
		{
			Token a;

			e >> a;

			if(!Tokens::EsIdentificador(a))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en adquirir: 'adquirir a': Se esperaba un identificador"
				);
			}

			PDCadena nombre = DATOS_INT.ResolverNombre(Tokens::ObtenerValor(a));

			DATOS_INT.CrearVariable(nombre, true, 0, nombre);
			DATOS_INT.CrearVariable(nombre + "#Tipo", true, 0, "PseudoVariable");
			DATOS_INT.CrearVariable(nombre + "#NOMBRE", true, 0, nombre);
		}
		else if(proxy == NMemonico::PD_FIN_FUNCION)
		{
		}
		else if(proxy == NMemonico::PD_INSTANCIA) // alias de Importar.Tipos.Instancia
		{
			(*FUNCION)(Token(ConvertirCadenaANMemonico("Importar.Tipos.Instancia")), e);
		}
		else if(proxy == NMemonico::PD_CLASE)
		{
			(*FUNCION)(Token(ConvertirCadenaANMemonico("Importar.Tipos.Estructura")), e);
		}
		else if(proxy == NMemonico::PD_HEREDAR)
		{
			(*FUNCION)(Token(ConvertirCadenaANMemonico("Importar.Tipos.Heredar")), e);
		}
		/*else if(o == "contiene")
		{
			(*FUNCION)(Token(ConvertirCadenaANMemonico("Importar.Tipos.EstrucEstruc")), e);
		}*/
		else if(proxy == NMemonico::PD_REDIRECCIONAR)
		{
			(*FUNCION)(Token(ConvertirCadenaANMemonico("Importar.Tipos.Redireccionar")), e);
		}
		else if(proxy == NMemonico::PD_LIBERAR)
		{
			(*FUNCION)(Token(ConvertirCadenaANMemonico("Importar.Tipos.Borrar")), e);
		}
		else if(proxy == NMemonico::PD_NEA)
		{
			(*FUNCION)(tok, e); // TAG @RECURSION
		}
		else if(proxy == NMemonico::PD_PUNTERO)
		{
			PDCadena var = "", val = "";
			Token vr, vl;

			e >> vr >> vl;

			if(!Tokens::EsIdentificador(vr) || !Tokens::EsIdentificador(vl))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en puntero: 'puntero r l': Se esperaba un identificador"
				);
			}

			var = DATOS_INT.ResolverNombre(Tokens::ObtenerValor(vr));
			val = DATOS_INT.ResolverNombre(Tokens::ObtenerValor(vl));

			DATOS_INT.CrearVariable(
				var,
				false,
				DATOS_INT.BuscarIndice(true, val)
			);
		}
		else if(proxy == NMemonico::PD_INCREMENTAR_PUNTERO)
		{
			PDCadena h = "";
			Token tk;

			e >> tk;

			if(!Tokens::EsIdentificador(tk))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en incrementar_p: 'incrementar_p ptr' alias "
					"'incrementar_puntero ptr': Se esperaba un identificador"
				);
			}

			h = DATOS_INT.ResolverNombre(Tokens::ObtenerValor(tk));

			DATOS_INT.ObtenerIndicePuntero(h)++;

			if(DATOS_INT.ObtenerIndicePuntero(h) >= (*DATOS_INT.nombrev).size())
			{
				throw PDvar::ErrorDelNucleo(
					"Error en incrementar_p: 'incrementar_p ptr' alias "
					"'incrementar_puntero ptr': Puntero invalido por bloqueo de memoria"
				);
			}
		}
		else if(proxy == NMemonico::PD_DECREMENTAR_PUNTERO)
		{
			PDCadena h = "";
			Token tk;

			e >> tk;

			if(!Tokens::EsIdentificador(tk))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en decrementar_p: 'decrementar_p ptr' alias "
					"'decrementar_puntero ptr': Se esperaba un identificador"
				);
			}

			h = DATOS_INT.ResolverNombre(Tokens::ObtenerValor(tk));

			DATOS_INT.ObtenerIndicePuntero(h)--;

			if(DATOS_INT.ObtenerIndicePuntero(h) < 0)
			{
				throw PDvar::ErrorDelNucleo(
					"Error en decrementar_p: 'decrementar_p ptr' alias "
					"'decrementar_puntero ptr': Puntero invalido por bloqueo de memoria"
				);
			}
		}
		else if(proxy == NMemonico::PD_ESCRIBIR)
		{
			Token tk;

			e >> tk;

			std::cout << PDvar::ValorDelToken(tk, e, &DATOS_INT);
		}
		else if(proxy == NMemonico::PD_EJECUTAR)
		{
			Token tk;

			e >> tk;

			std::istringstream entrada(PDvar::ValorDelToken(tk, e, &DATOS_INT));

			Tokenizador tkzr(entrada);

			while(!tkzr.FinDelFlujo())
				procesar(tkzr, FUNCION); // TAG @RECURSION
		}
		else if(proxy == NMemonico::PD_NUEVALINEA)
		{
			std::cout << std::endl;
		}
		else if(proxy == NMemonico::PD_FIJAR)
		{
			Token variable1, oper, h;
			PDCadena nval = "";

			e >> variable1 >> oper >> h;

			if(!Tokens::EsIdentificador(variable1))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en fijar: 'fijar var [oper] expr': Se esperaba un identificador"
				);
			}

			if(!Tokens::EsNMemonico(oper))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en fijar: 'fijar var [oper] expr': Se esperaba un nmemonico"
				);
			}

			PDCadena& a = DATOS_INT.ObtenerVariable(Tokens::ObtenerValor(variable1));

			NMemonicoProxy pr = oper.ObtenerNMemonico();

			if(pr == NMemonico::PD_OPERADOR_FIJAR) // fijar A =* hola
			{
				if(!Tokens::EsCadena(h))
				{
					throw PDvar::ErrorDeSintaxis(
						"Error en fijar: 'fijar A =* B': B no es una cadena"
					);
				}

				a = Tokens::ObtenerValor(h);
			}
			else if(pr == NMemonico::PD_OPERADOR_IGUAL) // fijar A =? B
			{
				if(!Tokens::EsIdentificador(h))
				{
					throw PDvar::ErrorDeSintaxis(
						"Error en fijar: 'fijar A =? B': B no es un identificador"
					);
				}

				a = DATOS_INT.ObtenerVariable(Tokens::ObtenerValor(h));
			}
			else if(pr == NMemonico::PD_OPERADOR_A) // fijar A a B
			{
				// B puede ser:
				// * Variable (igual a fijar a =? b)
				// * Cadena con delimitadores
				// Realmente debe utilizar `ValorDelToken`.
				a = PDvar::ValorDelToken(h, e, &DATOS_INT);
			}
			else
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en fijar: 'fijar a OP b': no se detecta 'OP' como " +
					oper.ObtenerNMemonico().original
				);
			}
		}
		else if(proxy == NMemonico::PD_LEER)
		{
			Token tk;

			e >> tk;

			if(!Tokens::EsIdentificador(tk))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en leer: 'leer var': Se esperaba un identificador"
				);
			}

			PDCadena& a = DATOS_INT.ObtenerVariable(Tokens::ObtenerValor(tk));

			std::cin >> a;
		}
		else if(proxy == NMemonico::PD_UTILIZAR)
		{
			PDCadena func = "", a = "", h = "";
			Token tk;

			e >> tk;

			if(Tokens::EsNMemonico(tk))
			{
				func = tk.ObtenerNMemonico().original;
			}
			else
			{
				func = Tokens::ObtenerValor(tk);
			}

			std::ifstream en(func.c_str());

			if(!en)
			{
				// el archivo no esta en una ruta actual, ejecutalo desde BEPD
				func = DATOS_INT.ObtenerVariable("__LIB__") + func;

				en.close();
				en.open(func.c_str());

				if(!en)
				{
					throw PDvar::ErrorDelNucleo(
						"Error en utilizar: 'utilizar fname': El archivo no existe"
					);
				}
			}

			if(DATOS_INT.BuscarIndice(true, "utilizar " + func) != -1)
			{
				if(DATOS_INT.adver > 1)
				{
					throw PDvar::ErrorDelNucleo(
						"Error menor en utilizar: 'utilizar fname': El archivo ya se ha "
						"importado"
					);
				}
				return;
			}

			a = DATOS_INT.ObtenerVariable("__ARCH__");
			DATOS_INT.ObtenerVariable("__ARCH__") = func;
			h = "";

			Tokenizador tkz(en);

			while(!tkz.FinDelFlujo())
			{
				procesar(tkz, FUNCION); // TAG @RECURSION
			}

			DATOS_INT.ObtenerVariable("__ARCH__") = a;
			DATOS_INT.CrearVariable("utilizar " + func, true, 0, "nulo");
		}
		else if(proxy == NMemonico::PD_LLAMAR)
		{
			PDCadena var = "", bpalab = "", nombre_var = "", tipo_var = "", h = "";
			std::vector<PDCadena> param;
			std::string::size_type numeral = 0;

			Token vr, bp;

			e >> vr >> bp;

			if(!Tokens::EsIdentificador(vr))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en llamar: 'llamar fn args... FIN': Se esperaba un "
					"identificador como nombre de funcion/metodo"
				);
			}

			var = DATOS_INT.ResolverNombre(Tokens::ObtenerValor(vr));
			const PDCadena& a = DATOS_INT.ObtenerVariable(var);

			while(true)
			{
				if(e.FinDelFlujo())
				{
					throw PDvar::ErrorDeSintaxis(
						"Error en llamar: 'llamar fn args... FIN': EOF inesperado"
					);
				}

				if(Tokens::EsIdentificador(bp))
				{
					param.push_back(DATOS_INT.ObtenerVariable(Tokens::ObtenerValor(bp)));
				}

				if(Tokens::EsCadena(bp))
				{
					param.push_back(Tokens::ObtenerValor(bp));
				}

				if(Tokens::EsNMemonico(bp))
				{
					if(bp.ObtenerNMemonico() == NMemonico::PD_FIN_ARGUMENTOS)
					{
						break;
					}
					else if(bp.ObtenerNMemonico() == NMemonico::PD_SEPARADOR_DE_ARGUMENTOS)
					{
					}
					else
					{
						param.push_back(PDvar::ValorDelToken(bp, e, &DATOS_INT));
					}
				}

				e >> bp;
			}

			numeral = var.rfind("#");

			if(numeral == std::string::npos)
				numeral = var.size();

			nombre_var = var.substr(0, numeral);
			tipo_var = DATOS_INT.ObtenerVariable(nombre_var + "#Tipo");

			for(int i = (param.size() - 1);i >= 0;i--)
			{
				DATOS_INT.Empujar(param[i], cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL")));
			}

			if(tipo_var != "PseudoFuncion")
			{
				DATOS_INT.Empujar(nombre_var, cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL")));
			}

			param.clear();

			//std::cout << "!!!!!!!!!!!!!!!!!!!!! Llamando a " << var << std::endl;

			std::istringstream st(a);
			Tokenizador tkz(st);
			Token tkb;

			if(tkz.FinDelFlujo())
			{
				return;
			}

			tkz >> tkb;

			if(
				(tkb.ObtenerTipo() == Token::NMemonico) &&
				(tkb.ObtenerNMemonico() == NMemonico::PD_CON)
			)
			{
				NMemonicoProxy px = tkb.ObtenerNMemonico();

				// La funcion utiliza argumentos del nuevo estilo:
				// funcion <fname> (de <argname>( y <argname>)*)?
				if(!(tkz >> tkb))
				{
					throw PDvar::ErrorDeSintaxis(
						"Error en llamar: 'llamar FN args.. FIN' -> 'funcion X de Y... ... "
						"finfun': EOF inesperado en la funcion"
					);
				}

				if(!Tokens::EsIdentificador(tkb))
				{
					throw PDvar::ErrorDeSintaxis(
						"Error en llamar: 'llamar fn args... FIN' -> 'funcion X de Y... ... "
						"finfun' -> 'de Y...': Se esperaba un identificador"
					);
				}

				h = Tokens::ObtenerValor(tkb);

				param.push_back(h);

				DATOS_INT.Ejecutar("adquirir " + h);
				DATOS_INT.Ejecutar("sacar " + h);

				bool depY = true; // Si la orden deberia ser un "y"

				NMemonicoProxy px2;

				while(true)
				{
					if(!(tkz >> tkb))
					{
						throw PDvar::ErrorDeSintaxis(
							"Error en llamar: llamar FN args.. FIN -> funcion X de Y... ... "
							"finfun: EOF inesperado en la funcion"
						);
					}

					if(depY)
					{
						if(!Tokens::EsNMemonico(tkb))
						{
							break;
						}

						px2 = tkb.ObtenerNMemonico();

						if(px2 != NMemonico::PD_OPERADOR_Y)
						{
							break;
						}
					}

					if(!depY)
					{
						if(!Tokens::EsIdentificador(tkb))
						{
							throw PDvar::ErrorDeSintaxis(
								"Error en llamar: 'llamar fn args.. FIN' -> funcion X de Y... "
								"... finfun: Se esperaba el nombre de un argumento"
							);
						}

						h = Tokens::ObtenerValor(tkb);

						DATOS_INT.Ejecutar("adquirir " + h);
						DATOS_INT.Ejecutar("sacar " + h);

						param.push_back(h);
					}

					depY = !depY;
				}
			}

			tkz.FijarIteradorA(0);

			while(!tkz.FinDelFlujo())
			{
				procesar(tkz, FUNCION); // TAG @RECURSION
			}

			for(auto iter = param.begin(); iter != param.end(); iter++)
			{
				DATOS_INT.Ejecutar("liberar " + (*iter));
			}
		}
		else if(proxy == NMemonico::PD_CON)
		{
			Token args;

			e >> args;

			if(!Tokens::EsIdentificador(args))
			{
				return;
			}

			auto p = e.ObtenerIterador();
			NMemonicoProxy px;

			do
			{
				p = e.ObtenerIterador();

				e >> args;

				if(!Tokens::EsNMemonico(args))
				{
					break;
				}

				px = args.ObtenerNMemonico();

				e >> args;
			} while(px == NMemonico::PD_OPERADOR_Y);

			e.FijarIteradorA(p);
		}
		else if(proxy == NMemonico::PD_FUNCION)
		{
			PDCadena nom = "", func = "";
			Token nm, cp;

			e >> nm >> cp;

			if(!Tokens::EsIdentificador(nm))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en funcion: 'funcion X ... finfun': Se esperaba un identificador"
				);
			}

			if(!Tokens::EsCuerpoDeCodigo(cp))
			{
				throw PDvar::ErrorDelNucleo(
					"Error en funcion: el tokenizador no agrego el cuerpo de la funcion "
					"como token justo despues de la declaracion"
				);
			}

			nom = DATOS_INT.ResolverNombre(Tokens::ObtenerValor(nm));

			func = cp.ObtenerValorLiteral().valor;

			DATOS_INT.CrearVariable(nom, true, 0, func);
			DATOS_INT.CrearVariable(nom + "#NOMBRE", true, 0, nom);
			DATOS_INT.CrearVariable(nom + "#Tipo", true, 0, "PseudoFuncion");
			DATOS_INT.CrearVariable(nom + "#cod", false, DATOS_INT.BuscarIndice(true, nom), "NULO");
		}
		else if(proxy == NMemonico::PD_EMPUJAR)
		{
			Token nm;

			e >> nm;

			DATOS_INT.Empujar(
				PDvar::ValorDelToken(nm, e, &DATOS_INT),
				cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL"))
			);
		}
		else if(proxy == NMemonico::PD_SACAR)
		{
			PDCadena variable1 = "";
			Token nm;

			e >> nm;

			if(!Tokens::EsIdentificador(nm))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en sacar: 'sacar X' alias 'recibir_parametro X' alias "
					"'recibir_resultado X': Se esperaba un identificador"
				);
			}

			variable1 = Tokens::ObtenerValor(nm);

			PDCadena& a = DATOS_INT.ObtenerVariable(variable1);

			int n = cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL"));

			if(DATOS_INT.PilaVacia(n))
			{
				a = DATOS_INT.ERROR;

				throw PDvar::ErrorDeSemantica(
					"Error en sacar: La pila esta vacia"
				);
			}
			else
			{
				a = DATOS_INT.Sacar(n);
			}
		}
		else if(proxy == NMemonico::PD_USAR_PILA)
		{
			Token pl;

			e >> pl;

			if(!Tokens::EsCadena(pl))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en usar_pila: 'usar_pila (numero)': El numero de pila no es"
					" una literal de cadena numerica"
				);
			}

			PDCadena vl = std::move(Tokens::ObtenerValor(pl));

			DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL") = vl;
		}
		else if(proxy == NMemonico::PD_CREAR_PILA)
		{
			DATOS_INT.CrearPila();
			DATOS_INT.ObtenerVariable("VG_NUMERO_PILAS") = eas((*DATOS_INT.pilas).size());
		}
		else if(proxy == NMemonico::PD_NECESITAS)
		{
			Token var;

			e >> var;

			if(PDvar::ValorDelToken(var, e, &DATOS_INT) != DATOS_INT.VERDADERO)
			{
				throw PDvar::ErrorDeSemantica("Error en necesitas: 'necesitas expr': la expresion es falsa");
			}
		}
		else if(proxy == NMemonico::PD_SI)
		{
			Token variable1;
			PDCadena val = "";
			bool cond = false;

			e >> variable1;

			val = ValorDelToken(variable1, e, &DATOS_INT);
			cond = (val == DATOS_INT.VERDADERO);
			AMBITO.push_back(cond);

			if(cond)
			{
				return;
			}

			auto iter = e.BuscarToken(
				e.ObtenerIterador(),
				Token(NMemonico::PD_SINO),
				Token(NMemonico::PD_FIN_SI),
				true,
				Token(NMemonico::PD_SI)
			);

			if(iter != e.ObtenerTokens().end())
			{
				e.IgnorarHasta(iter);
			}
			else
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en si: 'si expr ... FIN': EOF inesperado"
				);
			}
		}
		else if(proxy == NMemonico::PD_MIENTRAS)
		{
			Token expr;

			e >> expr;

			pseudod::Tokenizador::IteradorLT iter = e.ObtenerIterador();

			auto finiter = e.BuscarToken(
				e.ObtenerIterador(),
				Token(NMemonico::PD_FIN_BUCLE),
				true,
				Token(NMemonico::PD_MIENTRAS)
			);

			while(ValorDelToken(expr, e, &DATOS_INT) == DATOS_INT.VERDADERO)
			{
				pseudod::Tokenizador::IteradorLT iitr = e.ObtenerIterador();

				for(;; iitr = e.ObtenerIterador())
				{
					if(iitr == finiter)
						break;

					procesar(e, FUNCION); // TAG @RECURSION
				}

				e.FijarIteradorA(iter);
			}

			e.FijarIteradorA(finiter);
		}
		else if(proxy == NMemonico::PD_FIN_BUCLE)
		{
			// nada
		}
		else if(proxy == NMemonico::PD_SI_NO)
		{
			std::cerr << "Advertencia: la orden si_no se considera obsoleta" << std::endl;
			Token variable1;
			PDCadena val = "";
			bool cond = false;

			e >> variable1;

			val = ValorDelToken(variable1, e, &DATOS_INT);
			cond = (val == DATOS_INT.FALSO);
			AMBITO.push_back(cond);

			if(cond)
			{
				return;
			}

			auto iter = e.BuscarToken(
				e.ObtenerIterador(),
				Token(NMemonico::PD_SINO),
				Token(NMemonico::PD_FIN_SI),
				true,
				Token(NMemonico::PD_SI)
			);

			if(iter != e.ObtenerTokens().end())
			{
				e.IgnorarHasta(iter);
			}
			else
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en si_no OBSOLETO: 'si_no expr ... fin': EOF inesperado"
				);
			}
		}
		else if(proxy == NMemonico::PD_FIN_SI)
		{
			AMBITO.pop_back();
		}
		else if(proxy == NMemonico::PD_SINO)
		{
			bool v = AMBITO.back();

			if(!v)
			{
				return;
			}

			auto iter = e.BuscarToken(
				e.ObtenerIterador(),
				Token(NMemonico::PD_FIN_SI),
				true,
				Token(NMemonico::PD_SI)
			);

			if(iter != e.ObtenerTokens().end())
			{
				e.IgnorarHasta(iter);
			}
			else
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en sino: 'si expr ... sino ... FIN': EOF inesperado"
				);
			}
		}
		else if(proxy == NMemonico::PD_SON_IGUALES) // var1 var2 res
		{
			std::cerr << "Advertencia: la orden ¿son_iguales? (comparar_i) se considera obsoleta" << std::endl;

			Token var1, var2, variable1;
			e >> var1 >> var2 >> variable1;

			if(!(
				Tokens::EsIdentificador(var1) ||
				Tokens::EsIdentificador(var2) ||
				Tokens::EsIdentificador(variable1)))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en comparar_i OBSOLETO: 'comparar_i A B R' alias "
					"'¿son_iguales? A B R': Se esperaban todos identificadores (A B y R)"
				);
			}

			PDCadena& a = DATOS_INT.ObtenerVariable(Tokens::ObtenerValor(variable1));
			PDCadena& b = DATOS_INT.ObtenerVariable(Tokens::ObtenerValor(var1));
			PDCadena& c = DATOS_INT.ObtenerVariable(Tokens::ObtenerValor(var2));

			a = DATOS_INT.FALSO;

			if(b == c)
			{
				a = DATOS_INT.VERDADERO;
			}
		}
		else if(proxy == NMemonico::PD_ESCRIBIR_ESPACIO)
		{
			std::cout << " ";
		}
		else if(proxy == NMemonico::PD_SALIR)
		{
			Ejecutar = false;

			// RAISE HERE
		}
	}
}
