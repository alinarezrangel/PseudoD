/*
**PseudoD 1.9.5
**Creado por Alejandro Linarez Rangel
Define la macro INTERACTIVO a 1 para un interprete en linea de comandos
https://sourceforge.net/projects/pseudod/
*/
#include "interprete.hh"

namespace pseudod
{
	bool Ejecutar = true;
	PDDatos DATOS_INT;
	vector<int> AMBITO(2);
	inic_nea iniciar_nea;
	ejec_nea ejecutar_nea;
	liber_nea liberar_nea;
	void* coneccion_nea;
	void procesar(string o,istream& e, void(*FUNCION)(string,istream&))
	{
		//static string espacio = "";
		//static int indicepi = 0;
		//static string archivo;
		//static vector<int> AMBITO(2);
		if(!Ejecutar)
			return;
		if(o == "adquirir")
		{
			string a;
			e >> a;
			bool existe = false;
			for (int i = 0; i < (*DATOS_INT.nombrev).size(); i += 1)
			{
				if((*DATOS_INT.nombrev)[i] == a)
					existe = true;
			}
			if(existe)
			{
				throw string("¡SISTEMA CORRUPTOOOOO! en adquirir "+a);
			}
				DATOS_INT.CrearVariable(a,"Variable",0,a);
				DATOS_INT.CrearVariable(a+string("#Tipo"),"Variable",0,"PseudoVariable");
				DATOS_INT.CrearVariable(a+string("#NOMBRE"),"Variable",0,a);
		}
		else if(o == "instancia") // alias de Importar.Tipos.Instancia
		{
			(*FUNCION)("Importar.Tipos.Instancia",e);
		}
		else if((o == "estructura")||(o == "clase"))
		{
			(*FUNCION)("Importar.Tipos.Estructura",e);
		}
		else if((o == "heredar"))
		{
			(*FUNCION)("Importar.Tipos.Heredar",e);
		}
		else if((o == "contiene"))
		{
			(*FUNCION)("Importar.Tipos.EstrucEstruc",e);
		}
		else if((o == "redireccionar"))
		{
			(*FUNCION)("Importar.Tipos.Redireccionar",e);
		}
		else if((o == "mientras"))
		{
			(*FUNCION)("Importar.PseudoD.Mientras",e);
		}
		else if(o == "liberar")
		{
			(*FUNCION)("Importar.Tipos.Borrar",e);
		}
		else if(o == "puntero")
		{
			string var,val;
			e >> var >> val;
			bool existe = false;
			for (int i = 0; i < (*DATOS_INT.nombrep).size(); i += 1)
			{
				if((*DATOS_INT.nombrep)[i] == var)
					existe = true;
			}
			if(existe)
			{
				throw string("¡SISTEMA CORRUPTOOOOO! en puntero "+var);
			}
			DATOS_INT.CrearVariable(var,"Puntero",DATOS_INT.BuscarIndice("Variable",val));
		}
		else if((o == "incrementar_p")||(o == "incrementar_puntero"))
		{
			string h;
			e >> h;
			DATOS_INT.ObtenerIndicePuntero(h)++;
			if(DATOS_INT.ObtenerIndicePuntero(h) >= (*DATOS_INT.nombrev).size())
			{
				throw string("Error incrementando el puntero "+h+": acceso denegado a memoria prohibida");
			}
		}
		else if((o == "decrementar_p")||(o == "decrementar_puntero"))
		{
			string h;
			e >> h;
			DATOS_INT.ObtenerIndicePuntero(h)--;
			if(DATOS_INT.ObtenerIndicePuntero(h) < 0)
			{
				throw string("Error decrementando el puntero "+h+": acceso denegado a memoria prohibida");
			}
		}
		else if(o == "escribir")
		{
			string h;
			e >> h;
			cout << DATOS_INT.ObtenerVariable(h);
			#ifdef INTERACTIVO
				cout << endl;
			#endif
		}
		else if(o == "ejecutar")
		{
			string h;
			e >> h;
			istringstream entrada(DATOS_INT.ObtenerVariable(h));
			while(entrada >> h)
				procesar(h,entrada, FUNCION);
		}
		else if(o == "nl")
		{
			cout << endl;
		}
		else if(((o == "oper")||(o == "operador"))||(o == "fijar"))
		{
			string variable1,oper,h;
			e  >> variable1 >> oper >> h;
			string& a = DATOS_INT.ObtenerVariable(variable1);
			if(oper == "=*") // fijar A =* hola
			{
				string B;
				getline(e,B,'\n');
				h += B;
				a = h;
			}
			else if((oper == "=?")||(oper == "=¿?")) // fijar A =? B
			{
				a = DATOS_INT.ObtenerVariable(h);
			}
			else
			{
				throw string("No se detecta el operador "+oper);
			}
		}
		else if(o == "leer")
		{
			string variable1;
			e >> variable1;
			string& a = DATOS_INT.ObtenerVariable(variable1);
			cin >> a;
		}
		else if(o == "utilizar")
		{
			string func;
			e >> func;
			string funcion = func;
			ifstream en(funcion.c_str());
			if(!en)
			{
				// el archivo no esta en una ruta actual, ejecutalo desde BEPD
				funcion = DATOS_INT.ObtenerVariable("__LIB__") + funcion;
				en.close();
				en.open(funcion.c_str());
				if(!en)
				{
					cerr << "ERROR EN \'utilizar " << func << "\' se traduce a \'utilizar " << funcion << "\' no existe el archivo " << funcion << endl;
					throw string("Se intento importar un archivo inexistente");
				}
			}
			if(DATOS_INT.BuscarIndice("Variable","utilizar " + funcion) != -1)
			{
				if(DATOS_INT.adver > 1)
				{
					throw string("Se importo dos veces el mismo archivo");
				}
				return;
			}
			string a = DATOS_INT.ObtenerVariable("__ARCH__");
			DATOS_INT.ObtenerVariable("__ARCH__") = funcion;
			string h;
			while(en >> h)
			{
				procesar(h,en, FUNCION);
			}
			DATOS_INT.ObtenerVariable("__ARCH__") = a;
			DATOS_INT.CrearVariable("utilizar " + funcion,"Variable",0,"nulo");
		}
		else if(o == "llamar")
		{
			string var;
			e >> var;
			string a = DATOS_INT.ObtenerVariable(var);
			//*
			string bpalab = "";
			e >> bpalab;
			vector<string> param;
			while(bpalab != "#(Final).")
			{
				param.push_back(bpalab);
				if(!(e >> bpalab))
				{
					throw string("Error en llamar "+var+" : no se encontro #(Final). pero si un EOF");
				}
			}
			string nombre_var, tipo_var;
			int numeral = var.rfind("#");
			//clog << numeral << endl;
			if(numeral == string::npos)
				numeral = var.size();
			nombre_var = var.substr(0,numeral);
			tipo_var = DATOS_INT.ObtenerVariable(nombre_var+string("#Tipo"));
			//clog << tipo_var << endl;
			for(int i = (param.size()-1);i >= 0;i--)
			{
				string& a = DATOS_INT.ObtenerVariable(param[i]);
				(*DATOS_INT.pilas)[cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL"))].push(a);
			}
			if(tipo_var != "PseudoFuncion")
			{
				DATOS_INT.Empujar(nombre_var,cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL")));
				//clog << nombre_var << endl;
			}
			//*/
			istringstream st(a);
			string h;
			while(st >> h)
			{
				procesar(h,st,FUNCION);
			}
		}
		else if(o == "funcion")
		{
			vector<string> lineas;
			string nom,arg;
			e >> nom;
			string lin = "";
			while(lin != "finfun")
			{
				getline(e,lin,'\n');
				lineas.push_back(lin);
				lin.erase(std::remove_if(lin.begin(), 
		                            lin.end(),
		                            [](char x){return std::isspace(x);}),
		              lin.end());
			}
			string func;
			for(int i = 0;i < lineas.size();i++)
			{
				func += lineas[i] + "\n";
			}
			DATOS_INT.CrearVariable(nom,"Variable",0,func);
			DATOS_INT.CrearVariable(nom+string("#NOMBRE"),"Variable",0,nom);
			DATOS_INT.CrearVariable(nom+string("#Tipo"),"Variable",0,"PseudoFuncion");
			DATOS_INT.CrearVariable(nom+string("#cod"),"Puntero",DATOS_INT.BuscarIndice("Variable",nom),"NULO");
		}
		else if(o == "finfun")
		{
			// Nada
		}
		else if(o == "sistema")
		{
			string h;
			getline(e,h,'\n');
			system(h.c_str());
		}
		else if(o.front() == '[')
		{
			string h;
			e >> h;
			while(!(h.back()==']'))
			{
				e >> h;
			}
		}
		else if((o == "empujar")||(o == "enviar_parametro")||(o == "devolver"))
		{
			string variable1;
			e >> variable1;
			string& a = DATOS_INT.ObtenerVariable(variable1);
			DATOS_INT.Empujar(a,cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL")));
		}
		else if((o == "sacar")||(o == "recibir_resultado")||(o == "recibir_parametro"))
		{
			string variable1;
			e >> variable1;
			string& a = DATOS_INT.ObtenerVariable(variable1);
			a = DATOS_INT.Sacar(cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL")));
		}
		else if(o == "usar_pila")
		{
			e >> DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL");
				//DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL") = eas(indicepi);
		}
		else if(o == "crear_pila")
		{
			DATOS_INT.CrearPila();
			DATOS_INT.ObtenerVariable("VG_NUMERO_PILAS") = eas((*DATOS_INT.pilas).size());
		}
		else if(o == "necesitas")
		{
			string var;
			e >> var;
			cout << ValorDelToken(var,e,&DATOS_INT) << endl;
		}
		else if(o == "si")
		{
			string variable1,val;
			e >> variable1;
			/*if(variable1 == "llamar")
			{
				procesar("llamar",e,FUNCION);
				val = DATOS_INT.Sacar(cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL")));
			}
			else if(variable1 == "¿son_iguales?")
			{
				string var1,var2;
				e >> var1 >> var2;
				val = ((DATOS_INT.ObtenerVariable(var1) == DATOS_INT.ObtenerVariable(var2))? DATOS_INT.VERDADERO : DATOS_INT.FALSO);
			}
			else if(variable1 == "ejecutar")
			{
				string var1,var2,ord,es,valor;
				e >> ord >> var1 >> var2 >> es >> valor;
				if(es != "es")
					throw string("Error en si ejecutar "+ord+": Su sintaxis es: variable variable \"es\" resultado: no se escribio es, se escribio:" + es);
				istringstream in(var1 + " " + var2 + " ___codigo_pseudod_buffer_interno___");
				procesar(ord,in,FUNCION);
				val = ((DATOS_INT.ObtenerVariable("___codigo_pseudod_buffer_interno___") == DATOS_INT.ObtenerVariable(valor))? DATOS_INT.VERDADERO : DATOS_INT.FALSO);
			}
			else if(variable1 == "comparar")
			{
				string var1,op,var2,ord;
				e >> ord >> var1 >> op >> var2;
				istringstream in(var1 + " " + op + " " + var2 + " ___codigo_pseudod_buffer_interno___");
				procesar(ord,in,FUNCION);
				val = DATOS_INT.ObtenerVariable("___codigo_pseudod_buffer_interno___");
			}
			else
				val = DATOS_INT.ObtenerVariable(variable1);*/
			val = ValorDelToken(variable1,e,&DATOS_INT);
			string cond = ((val == DATOS_INT.VERDADERO)? "si" : "no");
			string ord;
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
				if((ord == "sino")&&(AMBITO.size() == (i + 1)))
				{
					cond = (cond == "si")? "no" : "si";
				}
			}
		}
		else if(o == "si_no")
		{
			string variable1,val;
			e >> variable1;
			if(variable1 == "llamar")
			{
				procesar("llamar",e,FUNCION);
				val = DATOS_INT.Sacar(cae(DATOS_INT.ObtenerVariable("VG_PILA_ACTUAL")));
			}
			else if(variable1 == "¿son_iguales?")
			{
				string var1,var2;
				e >> var1 >> var2;
				val = ((DATOS_INT.ObtenerVariable(var1) == DATOS_INT.ObtenerVariable(var2))? DATOS_INT.VERDADERO : DATOS_INT.FALSO);
			}
			else if(variable1 == "ejecutar")
			{
				string var1,var2,ord,es,valor;
				e >> ord >> var1 >> var2 >> es >> valor;
				if(es != "es")
					throw string("Error en si ejecutar "+ord+": Su sintaxis es: variable variable \"es\" resultado: no se escribio es, se escribio:" + es);
				istringstream in(var1 + " " + var2 + " ___codigo_pseudod_buffer_interno___");
				procesar(ord,in,FUNCION);
				val = ((DATOS_INT.ObtenerVariable("___codigo_pseudod_buffer_interno___") == DATOS_INT.ObtenerVariable(valor))? DATOS_INT.VERDADERO : DATOS_INT.FALSO);
			}
			else if(variable1 == "comparar")
			{
				string var1,op,var2,ord;
				e >> ord >> var1 >> op >> var2;
				istringstream in(var1 + " " + op + " " + var2 + " ___codigo_pseudod_buffer_interno___");
				procesar(ord,in,FUNCION);
				val = DATOS_INT.ObtenerVariable("___codigo_pseudod_buffer_interno___");
			}
			else
				val = DATOS_INT.ObtenerVariable(variable1);
			string cond = ((val == DATOS_INT.FALSO)? "si" : "no");
			string ord;
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
		else if(o == "fin")
		{
			AMBITO.pop_back();
		}
		else if((o == "comparar_i")||(o == "¿son_iguales?")) // var1 var2 res
		{
			string var1, var2;
			e >> var1 >> var2;
			string variable1;
			e >> variable1;
			string& a = DATOS_INT.ObtenerVariable(variable1);
			string& b = DATOS_INT.ObtenerVariable(var1);
			string& c = DATOS_INT.ObtenerVariable(var2);
			a = DATOS_INT.FALSO;
			if(b == c)
			{
				a = DATOS_INT.VERDADERO;
			}
		}
		else if(o == "escribir_esp")
		{
			cout << " ";
		}
		else if((o == "sal")||(o == "salir"))
		{
			Ejecutar=false;
		}
		else
		{
				(*FUNCION)(o,e);
		}
	}
	string iniciar(string nea,string bepd,string main)
	{
		coneccion_nea = dlopen(nea.c_str(),RTLD_LAZY);
		if(coneccion_nea == NULL)
		{
			return "Error al conectar con el NEA";
		}
		iniciar_nea = (inic_nea)dlsym(coneccion_nea,"PDInicializar");
		ejecutar_nea = (ejec_nea)dlsym(coneccion_nea,"PDEjecutar");
		liberar_nea = (liber_nea)dlsym(coneccion_nea,"PDFinal");
		if((iniciar_nea == NULL)||(ejecutar_nea == NULL)||(liberar_nea == NULL))
		{
			dlclose(coneccion_nea);
			return "Error al adquirir el NEA";
		}
		iniciar_nea((*DATOS_INT.nombrev),(*DATOS_INT.valorv),(*DATOS_INT.nombrep)
			,(*DATOS_INT.nvapunt),(*DATOS_INT.pilas),procesar);
		DATOS_INT.CrearVariable("__MAIN__","Variable",0,main);
		DATOS_INT.CrearVariable("__LIB__","Variable",0,bepd);
		DATOS_INT.CrearVariable("__ARCH__","Variable",0,main);
		DATOS_INT.CrearVariable("VG_PILA_ACTUAL","Variable",0,"0");
		DATOS_INT.CrearVariable("VG_NUMERO_PILAS","Variable",0,"0");
		DATOS_INT.PROCESAR = procesar;
		DATOS_INT.PROCESO = ejecutar_nea;
		try
		{
			ejecutar("utilizar " + DATOS_INT.ObtenerVariable("__LIB__") + "builtins.pseudo");
		}
		catch(const std::exception& e)
		{
			string errStr = "";
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
		catch(string e)
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
	void ejecutar(string linea)
	{
		string p;
		istringstream in(linea);
		while((Ejecutar)&&(in >> p))
			procesar(p,in,ejecutar_nea);
	}
	void ejecutar(istream& entrada)
	{
		string p;
		while((Ejecutar)&&(entrada >> p))
			procesar(p,entrada,ejecutar_nea);
	}
}
