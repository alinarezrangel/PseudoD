/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*** Este es PseudoD version 2.1.0                                      *****
*** Log de este archivo:                                               *****
*** Formato: DD/MM/YYYY: txt                                           *****
*** **** 02/01/2016: Se creo el archivo.                               *****
*** **** 22/03/2016: Agregadas advertencias.                           *****
****************************************************************************
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
		std::string o,
		std::istream& e,
		void(*FUNCION)(
			std::string,
			std::istream&
		)
	)
	{
		if(!Ejecutar)
			return;
		NMemonicoProxy proxy = ConvertirCadenaANMemonico(o);
		if(proxy == NMemonico::PD_ADQUIRIR)
		{
			std::string a;
			e >> a;
			DATOS_INT.CrearVariable(a, "Variable", 0, a);
			DATOS_INT.CrearVariable(a + "#Tipo", "Variable", 0, "PseudoVariable");
			DATOS_INT.CrearVariable(a + "#NOMBRE", "Variable", 0, a);
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
			std::string var = "", val = "";
			bool existe = false;
			e >> var >> val;
			for (int i = 0; i < (*DATOS_INT.nombrep).size(); i += 1)
			{
				if((*DATOS_INT.nombrep)[i] == var)
					existe = true;
			}
			if(existe)
			{
				throw PDvar::ErrorDelNucleo("Error en puntero: 'puntero ptr nval': El puntero ya existe");
			}
			DATOS_INT.CrearVariable(var, "Puntero", DATOS_INT.BuscarIndice("Variable",val));
		}
		else if(proxy == NMemonico::PD_INCREMENTAR_PUNTERO)
		{
			std::string h = "";
			e >> h;
			DATOS_INT.ObtenerIndicePuntero(h)++;
			if(DATOS_INT.ObtenerIndicePuntero(h) >= (*DATOS_INT.nombrev).size())
			{
				throw PDvar::ErrorDelNucleo("Error en incrementar_p: 'incrementar_p ptr' alias 'incrementar_puntero ptr': Puntero invalido por bloqueo de memoria");
			}
		}
		else if(proxy == NMemonico::PD_DECREMENTAR_PUNTERO)
		{
			std::string h = "";
			e >> h;
			DATOS_INT.ObtenerIndicePuntero(h)--;
			if(DATOS_INT.ObtenerIndicePuntero(h) < 0)
			{
				throw PDvar::ErrorDelNucleo("Error en decrementar_p: 'decrementar_p ptr' alias 'decrementar_puntero ptr': Puntero onvalido por bloqueo de memoria");
			}
		}
		else if(proxy == NMemonico::PD_ESCRIBIR)
		{
			std::string h = "";
			e >> h;
			std::cout << DATOS_INT.ObtenerVariable(h);
		}
		else if(proxy == NMemonico::PD_EJECUTAR)
		{
			std::string h = "";
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
			std::string variable1 = "", oper = "", h = "", linea = "";
			e  >> variable1 >> oper >> h;
			std::string& a = DATOS_INT.ObtenerVariable(variable1);
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
			std::string variable1 = "";
			e >> variable1;
			std::string& a = DATOS_INT.ObtenerVariable(variable1);
			std::cin >> a;
		}
		else if(proxy == NMemonico::PD_UTILIZAR)
		{
			std::string func = "", a = "", h = "";
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
			if(DATOS_INT.BuscarIndice("Variable", "utilizar " + func) != -1)
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
			DATOS_INT.CrearVariable("utilizar " + func, "Variable", 0, "nulo");
		}
		else if(proxy == NMemonico::PD_LLAMAR)
		{
			std::string var = "", bpalab = "", nombre_var = "", tipo_var = "", h = "";
			std::vector<std::string> param;
			int numeral = 0;

			e >> var >> bpalab;
			std::string a = DATOS_INT.ObtenerVariable(var);
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
			std::istringstream st(a);
			while(st >> h)
			{
				procesar(h, st, FUNCION);
			}
		}
		else if(proxy == NMemonico::PD_FUNCION)
		{
			std::string nom = "", arg = "", lin = "", func = "";
			e >> nom;
			while(lin != "finfun")
			{
				std::getline(e, lin, '\n');
				func += lin + '\n';
				lin.erase(std::remove_if(lin.begin(),
					lin.end(),
					[](char x){return std::isspace(x);}),
					lin.end()
				);
			}
			DATOS_INT.CrearVariable(nom, "Variable", 0, func);
			DATOS_INT.CrearVariable(nom + "#NOMBRE", "Variable", 0, nom);
			DATOS_INT.CrearVariable(nom + "#Tipo", "Variable", 0, "PseudoFuncion");
			DATOS_INT.CrearVariable(nom + "#cod", "Puntero", DATOS_INT.BuscarIndice("Variable", nom), "NULO");
		}
		else if(proxy == NMemonico::PD_FIN_FUNCION)
		{
			// Nada
		}
		else if(o == "sistema")
		{
			std::cerr << "Advertencia: sistema esta altamente desaprobado porque es malvado" << std::endl;
			std::string h = "";
			std::getline(e, h, '\n');
			system(h.c_str());
		}
		else if(o.front() == '[')
		{
			std::string h = "";
			std::getline(e, h, ']');
		}
		else if(proxy == NMemonico::PD_EMPUJAR)
		{
			std::string variable1;
			e >> variable1;
			std::string& a = DATOS_INT.ObtenerVariable(variable1);
			DATOS_INT.Empujar(a, cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL")));
		}
		else if(proxy == NMemonico::PD_SACAR)
		{
			std::string variable1;
			e >> variable1;
			std::string& a = DATOS_INT.ObtenerVariable(variable1);
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
			std::string var;
			e >> var;
			if(PDvar::ValorDelToken(var, e, &DATOS_INT) != DATOS_INT.VERDADERO)
			{
				throw PDvar::ErrorDeSemantica("Error en necesitas: 'necesitas expr': la expresion es falsa");
			}
		}
		else if(proxy == NMemonico::PD_SI)
		{
			std::string variable1 = "", val = "";
			bool cond = false;
			int i = 0;
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
					std::string line = "";
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
			std::string variable1 = "", val = "";
			e >> variable1;
			if(variable1 == "llamar")
			{
				procesar("llamar",e,FUNCION);
				val = DATOS_INT.Sacar(cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL")));
			}
			else if(variable1 == "¿son_iguales?")
			{
				std::string var1 = "", var2 = "";
				e >> var1 >> var2;
				val = ((DATOS_INT.ObtenerVariable(var1) == DATOS_INT.ObtenerVariable(var2))? DATOS_INT.VERDADERO : DATOS_INT.FALSO);
			}
			else if(variable1 == "ejecutar")
			{
				std::string var1,var2,ord,es,valor;
				e >> ord >> var1 >> var2 >> es >> valor;
				if(es != "es")
					throw PDvar::ErrorDelNucleo("Error en si_no: 'si_no expr cdg...': La expresion es invalida");
				std::istringstream in(var1 + " " + var2 + " ___codigo_pseudod_buffer_interno___");
				procesar(ord,in,FUNCION);
				val = ((DATOS_INT.ObtenerVariable("___codigo_pseudod_buffer_interno___") == DATOS_INT.ObtenerVariable(valor))? DATOS_INT.VERDADERO : DATOS_INT.FALSO);
			}
			else if(variable1 == "comparar")
			{
				std::string var1,op,var2,ord;
				e >> ord >> var1 >> op >> var2;
				std::istringstream in(var1 + " " + op + " " + var2 + " ___codigo_pseudod_buffer_interno___");
				procesar(ord,in,FUNCION);
				val = DATOS_INT.ObtenerVariable("___codigo_pseudod_buffer_interno___");
			}
			else
				val = DATOS_INT.ObtenerVariable(variable1);
			std::string cond = ((val == DATOS_INT.FALSO)? "si" : "no");
			std::string ord;
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
			std::string var1 = "", var2 = "";
			e >> var1 >> var2;
			std::string variable1;
			e >> variable1;
			std::string& a = DATOS_INT.ObtenerVariable(variable1);
			std::string& b = DATOS_INT.ObtenerVariable(var1);
			std::string& c = DATOS_INT.ObtenerVariable(var2);
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
	std::string iniciar(std::string nea, std::string bepd, std::string main)
	{
		coneccion_nea = dlopen(nea.c_str(), RTLD_LAZY);
		if(coneccion_nea == NULL)
		{
			return "Error al conectar con el NEA";
		}
		iniciar_nea = (inic_nea) dlsym(coneccion_nea, "PDInicializar");
		ejecutar_nea = (ejec_nea) dlsym(coneccion_nea, "PDEjecutar");
		liberar_nea = (liber_nea) dlsym(coneccion_nea, "PDFinal");
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
		DATOS_INT.CrearVariable("__MAIN__","Variable", 0, main);
		DATOS_INT.CrearVariable("__LIB__","Variable", 0, bepd);
		DATOS_INT.CrearVariable("__ARCH__","Variable", 0, main);
		DATOS_INT.CrearVariable("VG_PILA_ACTUAL", "Variable", 0, "0");
		DATOS_INT.CrearVariable("VG_NUMERO_PILAS", "Variable", 0, "0");
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
			if((e.what() == "stoi")||(e.what() == "stoll"))
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
	void ejecutar(std::string linea)
	{
		std::string p;
		std::istringstream in(linea);
		while(Ejecutar && (in >> p))
			procesar(p, in, ejecutar_nea);
	}
	void ejecutar(std::istream& entrada)
	{
		std::string p;
		while(Ejecutar && (entrada >> p))
			procesar(p, entrada, ejecutar_nea);
	}
}

