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

	void procesar(
		PDCadena o,
		std::istream& e,
		PDFuncionNEA FUNCION
	)
	{
		if(!Ejecutar)
			return;

		NMemonicoProxy proxy = ConvertirCadenaANMemonico(o);

		if(proxy == NMemonico::PD_ADQUIRIR)
		{
			PDCadena a;
			e >> a;
			a = DATOS_INT.ResolverNombre(a);
			DATOS_INT.CrearVariable(a, true, 0, a);
			DATOS_INT.CrearVariable(a + "#Tipo", true, 0, "PseudoVariable");
			DATOS_INT.CrearVariable(a + "#NOMBRE", true, 0, a);
		}
		else if(proxy == NMemonico::PD_FIN_FUNCION)
		{
		}
		else if(proxy == NMemonico::PD_SINO)
		{
		}
		else if(proxy == NMemonico::PD_INSTANCIA) // alias de Importar.Tipos.Instancia
		{
			(*FUNCION)("Importar.Tipos.Instancia", e);
		}
		else if(proxy == NMemonico::PD_CLASE)
		{
			(*FUNCION)("Importar.Tipos.Estructura", e);
		}
		else if(proxy == NMemonico::PD_HEREDAR)
		{
			(*FUNCION)("Importar.Tipos.Heredar", e);
		}
		else if(o == "contiene")
		{
			(*FUNCION)("Importar.Tipos.EstrucEstruc", e);
		}
		else if(proxy == NMemonico::PD_REDIRECCIONAR)
		{
			(*FUNCION)("Importar.Tipos.Redireccionar", e);
		}
		else if(proxy == NMemonico::PD_MIENTRAS)
		{
			(*FUNCION)("Importar.PseudoD.Mientras", e);
		}
		else if(proxy == NMemonico::PD_LIBERAR)
		{
			(*FUNCION)("Importar.Tipos.Borrar", e);
		}
		else if(proxy == NMemonico::PD_PUNTERO)
		{
			PDCadena var = "", val = "";
			e >> var >> val;
			var = DATOS_INT.ResolverNombre(var);
			DATOS_INT.CrearVariable(
				var,
				false,
				DATOS_INT.BuscarIndice(true, val)
			);
		}
		else if(proxy == NMemonico::PD_INCREMENTAR_PUNTERO)
		{
			PDCadena h = "";
			e >> h;
			DATOS_INT.ObtenerIndicePuntero(h)++;
			if(DATOS_INT.ObtenerIndicePuntero(h) >= (*DATOS_INT.nombrev).size())
			{
				throw PDvar::ErrorDelNucleo(
					PDCadena("Error en incrementar_p: 'incrementar_p ptr' alias ") +
					"'incrementar_puntero ptr': Puntero invalido por bloqueo de memoria"
				);
			}
		}
		else if(proxy == NMemonico::PD_DECREMENTAR_PUNTERO)
		{
			PDCadena h = "";
			e >> h;
			DATOS_INT.ObtenerIndicePuntero(h)--;
			if(DATOS_INT.ObtenerIndicePuntero(h) < 0)
			{
				throw PDvar::ErrorDelNucleo(
					PDCadena("Error en decrementar_p: 'decrementar_p ptr' alias ") +
					"'decrementar_puntero ptr': Puntero invalido por bloqueo de memoria"
				);
			}
		}
		else if(proxy == NMemonico::PD_ESCRIBIR)
		{
			PDCadena h = "";
			e >> h;
			std::cout << PDvar::ValorDelToken(h, e, &DATOS_INT);
		}
		else if(proxy == NMemonico::PD_EJECUTAR)
		{
			PDCadena h = "";
			e >> h;
			std::istringstream entrada(DATOS_INT.ObtenerVariable(h));
			while(entrada >> h)
				procesar(h, entrada, FUNCION);
		}
		else if(proxy == NMemonico::PD_NUEVALINEA)
		{
			std::cout << std::endl;
		}
		else if(proxy == NMemonico::PD_FIJAR)
		{
			if(o != "fijar")
				std::cerr << "Advertencia: oper/operador estan obsoletos" << std::endl;

			PDCadena variable1 = "", oper = "", h = "", linea = "";
			e >> variable1 >> oper >> h;
			PDCadena& a = DATOS_INT.ObtenerVariable(variable1);
			NMemonicoProxy pr = ConvertirCadenaANMemonico(oper);
			if(pr == NMemonico::PD_OPERADOR_FIJAR) // fijar A =* hola
			{
				std::getline(e, linea, '\n');
				h += linea;
				a = h;
			}
			else if(pr == NMemonico::PD_OPERADOR_IGUAL) // fijar A =? B
			{
				a = DATOS_INT.ObtenerVariable(h);
			}
			else if(pr == NMemonico::PD_OPERADOR_A) // fijar A a B
			{
				// B puede ser:
				// * Variable (igual a fijar a =? b)
				// * Cadena con delimitadores:
				//   Como fue dicho en las anteriores consultas, PseudoD soportara 3
				//   tipos de cadenas: {a} "a" y «a»
				// Realmente debe utilizar `ValorDelToken`.
				a = PDvar::ValorDelToken(h, e, &DATOS_INT);
			}
			else
			{
				throw PDvar::ErrorDeSintaxis("Error en fijar: 'fijar a OP b': no se detecta 'OP' como " + oper);
			}
		}
		else if(proxy == NMemonico::PD_LEER)
		{
			PDCadena variable1 = "";
			e >> variable1;
			PDCadena& a = DATOS_INT.ObtenerVariable(variable1);
			std::cin >> a;
		}
		else if(proxy == NMemonico::PD_UTILIZAR)
		{
			PDCadena func = "", a = "", h = "";
			e >> func;

			std::ifstream en(func.c_str());

			if(!en)
			{
				// el archivo no esta en una ruta actual, ejecutalo desde BEPD
				func = DATOS_INT.ObtenerVariable("__LIB__") + func;
				en.close();
				en.open(func.c_str());
				if(!en)
				{
					throw PDvar::ErrorDelNucleo("Error en utilizar: 'utilizar fname': El archivo no existe");
				}
			}
			if(DATOS_INT.BuscarIndice(true, "utilizar " + func) != -1)
			{
				if(DATOS_INT.adver > 1)
				{
					throw PDvar::ErrorDelNucleo("Error menor en utilizar: 'utilizar fname': El archivo ya se ha importado");
				}
				return;
			}
			a = DATOS_INT.ObtenerVariable("__ARCH__");
			DATOS_INT.ObtenerVariable("__ARCH__") = func;
			h = "";
			while(en >> h)
			{
				procesar(h, en, FUNCION);
			}
			DATOS_INT.ObtenerVariable("__ARCH__") = a;
			DATOS_INT.CrearVariable("utilizar " + func, true, 0, "nulo");
		}
		else if(proxy == NMemonico::PD_LLAMAR)
		{
			PDCadena var = "", bpalab = "", nombre_var = "", tipo_var = "", h = "";
			std::vector<PDCadena> param;
			std::string::size_type numeral = 0;

			e >> var >> bpalab;
			var = DATOS_INT.ResolverNombre(var);
			PDCadena a = DATOS_INT.ObtenerVariable(var);
			while((bpalab != "#(Final).") && (bpalab != "finargs"))
			{
				param.push_back(PDvar::ValorDelToken(bpalab, e, &DATOS_INT));
				if(!(e >> bpalab))
				{
					throw PDvar::ErrorDeSintaxis("Error en llamar: 'llamar fn args... FIN': EOF inesperado");
				}
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
			std::istringstream st(a);
			if(st >> h)
			{
				NMemonicoProxy px = ConvertirCadenaANMemonico(h);
				if(px == NMemonico::PD_CON)
				{
					// La funcion utiliza argumentos del nuevo estilo:
					// funcion <fname> (de <argname>( y <argname>)*)?
					if(!(st >> h))
					{
						throw PDvar::ErrorDeSintaxis(
							"Error en llamar: llamar FN args.. FIN -> funcion X de Y... ... finfun: EOF inesperado en la funcion"
						);
					}
					param.push_back(h);
					DATOS_INT.Ejecutar("adquirir " + h);
					DATOS_INT.Ejecutar("sacar " + h);
					bool depY = true; // Si la orden deberia ser un "y"
					NMemonicoProxy px2;
					while(true)
					{
						if(!(st >> h))
						{
							throw PDvar::ErrorDeSintaxis(
								"Error en llamar: llamar FN args.. FIN -> funcion X de Y... ... finfun: EOF inesperado en la funcion"
							);
						}
						px2 = ConvertirCadenaANMemonico(h);
						if((depY) && (px2 != NMemonico::PD_OPERADOR_Y))
						{
							break;
						}
						if(!depY)
						{
							DATOS_INT.Ejecutar("adquirir " + h);
							DATOS_INT.Ejecutar("sacar " + h);
							param.push_back(h);
						}
						depY = !depY;
					}
				}
				st.seekg(0);
				while(st >> h)
				{
					procesar(h, st, FUNCION);
				}
				for(auto iter = param.begin(); iter != param.end(); iter++)
				{
					DATOS_INT.Ejecutar("liberar " + (*iter));
				}
			}
		}
		else if(proxy == NMemonico::PD_CON)
		{
			PDCadena args = "";
			e >> args;
			std::istream::pos_type p = e.tellg();
			NMemonicoProxy px = ConvertirCadenaANMemonico(args);
			if((e >> args) && (px == NMemonico::PD_OPERADOR_Y))
			{
				do
				{
					e >> args;
					p = e.tellg();
					e >> args;
					px = ConvertirCadenaANMemonico(args);
				} while(px == NMemonico::PD_OPERADOR_Y);
				e.seekg(p);
			}
			else
			{
				e.seekg(p);
			}
		}
		else if(proxy == NMemonico::PD_FUNCION)
		{
			PDCadena nom = "", arg = "", lin = "", func = "";

			e >> nom;

			NMemonicoProxy px = ConvertirCadenaANMemonico(lin);

			while(px != NMemonico::PD_FIN_FUNCION)
			{
				std::getline(e, lin, '\n');
				func += lin + '\n';
				lin.erase(std::remove_if(lin.begin(),
					lin.end(),
					[](char x){return std::isspace(x);}),
					lin.end()
				);
				px = ConvertirCadenaANMemonico(lin);
			}
			DATOS_INT.CrearVariable(nom, true, 0, func);
			DATOS_INT.CrearVariable(nom + "#NOMBRE", true, 0, nom);
			DATOS_INT.CrearVariable(nom + "#Tipo", true, 0, "PseudoFuncion");
			DATOS_INT.CrearVariable(nom + "#cod", false, DATOS_INT.BuscarIndice(true, nom), "NULO");
		}
		else if(o.front() == '[')
		{
			std::string h = "";
			std::getline(e, h, ']');
		}
		else if(proxy == NMemonico::PD_EMPUJAR)
		{
			PDCadena variable1;
			e >> variable1;
			// PDCadena& a = DATOS_INT.ObtenerVariable(variable1);
			DATOS_INT.Empujar(
				PDvar::ValorDelToken(variable1, e, &DATOS_INT),
				cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL"))
			);
		}
		else if(proxy == NMemonico::PD_SACAR)
		{
			PDCadena variable1;
			e >> variable1;
			PDCadena& a = DATOS_INT.ObtenerVariable(variable1);
			a = DATOS_INT.Sacar(cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL")));
		}
		else if(proxy == NMemonico::PD_USAR_PILA)
		{
			e >> DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL");
		}
		else if(proxy == NMemonico::PD_CREAR_PILA)
		{
			DATOS_INT.CrearPila();
			DATOS_INT.ObtenerVariable("VG_NUMERO_PILAS") = eas((*DATOS_INT.pilas).size());
		}
		else if(proxy == NMemonico::PD_NECESITAS)
		{
			PDCadena var;
			e >> var;
			if(PDvar::ValorDelToken(var, e, &DATOS_INT) != DATOS_INT.VERDADERO)
			{
				throw PDvar::ErrorDeSemantica("Error en necesitas: 'necesitas expr': la expresion es falsa");
			}
		}
		else if(proxy == NMemonico::PD_SI)
		{
			PDCadena variable1 = "", val = "";
			bool cond = false;
			decltype(AMBITO)::size_type i = 0;
			NMemonicoProxy ord;

			e >> variable1;
			val = ValorDelToken(variable1, e, &DATOS_INT);
			cond = (val == DATOS_INT.VERDADERO);
			i = AMBITO.size();
			AMBITO.push_back(0);
			while(AMBITO.size() != i)
			{
				e >> ord;
				if(ord == NMemonico::PD_OPERADOR_FIJAR)
				{
					PDCadena line = "";
					getline(e, line, '\n');
					e >> ord;
				}
				if(cond)
				{
					procesar(ord, e, FUNCION);
				}
				else if(ord == NMemonico::PD_FIN_SI)
				{
					AMBITO.pop_back();
				}
				else if((ord == NMemonico::PD_SI) || (ord == NMemonico::PD_SI_NO))
				{
					AMBITO.push_back(0);
				}
				if((ord == NMemonico::PD_SINO) && (AMBITO.size() == (i + 1)))
				{
					cond = !cond;
				}
			}
		}
		else if(proxy == NMemonico::PD_SI_NO)
		{
			std::cerr << "Advertencia: la orden si_no se considera obsoleta" << std::endl;
			PDCadena variable1 = "", val = "";
			e >> variable1;
			if(variable1 == "llamar")
			{
				procesar("llamar",e,FUNCION);
				val = DATOS_INT.Sacar(cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL")));
			}
			else if(variable1 == "¿son_iguales?")
			{
				PDCadena var1 = "", var2 = "";
				e >> var1 >> var2;
				val = ((DATOS_INT.ObtenerVariable(var1) == DATOS_INT.ObtenerVariable(var2))? DATOS_INT.VERDADERO : DATOS_INT.FALSO);
			}
			else if(variable1 == "ejecutar")
			{
				PDCadena var1,var2,ord,es,valor;
				e >> ord >> var1 >> var2 >> es >> valor;
				if(es != "es")
					throw PDvar::ErrorDelNucleo("Error en si_no: 'si_no expr cdg...': La expresion es invalida");
				std::istringstream in(var1 + " " + var2 + " ___codigo_pseudod_buffer_interno___");
				procesar(ord,in,FUNCION);
				val = ((DATOS_INT.ObtenerVariable("___codigo_pseudod_buffer_interno___") == DATOS_INT.ObtenerVariable(valor))? DATOS_INT.VERDADERO : DATOS_INT.FALSO);
			}
			else if(variable1 == "comparar")
			{
				PDCadena var1,op,var2,ord;
				e >> ord >> var1 >> op >> var2;
				std::istringstream in(var1 + " " + op + " " + var2 + " ___codigo_pseudod_buffer_interno___");
				procesar(ord,in,FUNCION);
				val = DATOS_INT.ObtenerVariable("___codigo_pseudod_buffer_interno___");
			}
			else
				val = DATOS_INT.ObtenerVariable(variable1);
			PDCadena cond = ((val == DATOS_INT.FALSO)? "si" : "no");
			PDCadena ord = "";
			int i = 0;
			i = AMBITO.size();
			AMBITO.push_back(0);
			while(AMBITO.size() != i)
			{
				e >> ord;
				if(ord == "=*")
				{
					getline(e,ord,'\n');
					e >> ord;
				}
				if(cond == "si")
				{
					procesar(ord, e, FUNCION);
				}
				else if(ord == "fin")
				{
					AMBITO.pop_back();
				}
				else if((ord == "si")||(ord == "si_no"))
				{
					AMBITO.push_back(0);
				}
			}
		}
		else if(proxy == NMemonico::PD_FIN_SI)
		{
			AMBITO.pop_back();
		}
		else if(proxy == NMemonico::PD_SON_IGUALES) // var1 var2 res
		{
			std::cerr << "Advertencia: la orden ¿son_iguales? (comparar_i) se considera obsoleta" << std::endl;
			PDCadena var1 = "", var2 = "";
			e >> var1 >> var2;
			PDCadena variable1 = "";
			e >> variable1;
			PDCadena& a = DATOS_INT.ObtenerVariable(variable1);
			PDCadena& b = DATOS_INT.ObtenerVariable(var1);
			PDCadena& c = DATOS_INT.ObtenerVariable(var2);
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
		}
		else
		{
			(*FUNCION)(o,e);
		}
	}

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
		PDCadena p;
		std::istringstream in(linea);
		while(Ejecutar && (in >> p))
			procesar(p, in, ejecutar_nea);
	}

	void ejecutar(std::istream& entrada)
	{
		PDCadena p;
		while(Ejecutar && (entrada >> p))
			procesar(p, entrada, ejecutar_nea);
	}
}
