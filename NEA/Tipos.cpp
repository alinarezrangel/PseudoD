/*
**PseudoD 1.5.0
**Creado por Alejandro Linarez Rangel
*/

namespace PDTipos
{
	PDOrden::PDOrden() : PDObjeto()
	{
		this->clave = "NO FIJADA";
	}
	
	PDOrden::~PDOrden()
	{
		// nada
	}
	
	string PDOrden::ObtenerClave()
	{
		return this->clave;
	}
	
	void PDOrden::LeerParametros(istream& in)
	{
		return;
	}
	
	void PDOrden::FijarClave(string cl,string pack)
	{
		this->clave = pack + string(".") + cl;
	}
	
	PseudoArray::PseudoArray(string nom, int can) : PDInstancia()
	{
		this->cant = can;
		this->nm = nom;
		this->FijarClave(string("Array"),string("Tipos"));
	}
	
	PseudoArray::~PseudoArray()
	{
		//~~~~~~nada
	}
	
	void PseudoArray::InscribirInstancia(PDDatos* data)
	{
		data->CrearVariable(this->nm + string("#longitud."));
		string& r = data->ObtenerVariable(this->nm + string("#longitud."));
		r = eas(this->cant);
		for(int  i = 0; i < this->cant;i++)
		{
			data->CrearVariable(this->nm + string("#(") + eas(i) + string(")."));
		}
	}
	
	void PseudoArray::LeerParametros(istream& in)
	{
		if(!(in >> this->nm >> this->cant))
		{
			cerr << "Error en la creacion del " << this->ObtenerClave() << "," << this->nm << " No se pudo leer bien el fichero fuente." << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " EOF inesperado");
		}
	}
	
	PseudoClase::PseudoClase(string nm,vector<string> mt) : PDInstancia()
	{
		this->nm = nm;
		this->methods = mt; 
		this->FijarClave(string("Estructura"),string("Tipos"));
	}
	
	PseudoClase::~PseudoClase()
	{
		//NADA
	}
	
	void PseudoClase::InscribirInstancia(PDDatos* data)
	{
		data->CrearVariable(this->nm);
		data->ObtenerVariable(this->nm) = eas(this->methods.size());
		for(int i = 0;i < this->methods.size();i++)
		{
			data->CrearVariable(this->nm + string("#(") + eas(i) + string(")."));
			data->ObtenerVariable(this->nm + string("#(") + eas(i) + string(").")) = this->methods[i];
		}
	}
	
	void PseudoClase::LeerParametros(istream& in)
	{
		if(!(in >> this->nm))
		{
			cerr << "Error en la creacion del " << this->ObtenerClave() << "," << "(ERROR,VARIABLE INEXISTENTE)" << " No se pudo leer bien el fichero de entrada." << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " EOF inesperado");
		}
		string b;
		if(!(in >> b))
		{
			cerr << "Error en la creacion del " << this->ObtenerClave() << "," << this->nm << " No se pudo leer bien el fichero de entrada." << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " EOF inesperado");
			return;
		}
		while(b != "#(Final).")
		{
			this->methods.push_back(b);
			if(!(in >> b))
			{
				cerr << "Error en la creacion del " << this->ObtenerClave() << "," << this->nm << " No se pudo leer bien el fichero de entrada." << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " EOF inesperado");
				return;
			}
		}
	}
	
	PseudoReferenciaClase::PseudoReferenciaClase(string n,string i) : PDInstancia()
	{
		this->nm = n;
		this->ni = i;
		this->FijarClave(string("Instancia"),string("Tipos"));
	}
	
	PseudoReferenciaClase::~PseudoReferenciaClase()
	{
		//NADA
	}
	
	void PseudoReferenciaClase::InscribirInstancia(PDDatos* data)
	{
		data->CrearVariable(this->ni);
		data->ObtenerVariable(this->ni)=this->ni;
		int lg = cae(data->ObtenerVariable(this->nm));
		
		for(int i = 0;i < lg;i++)
		{
			this->methods.push_back(data->ObtenerVariable(this->nm+string("#(")+eas(i)+string(").")));
		}
		
		for(int i = 0;i < this->methods.size();i++)
		{
			if(this->methods[i][0] == ';')
			{
				this->methods[i].replace(0,1,"");
				data->CrearVariable(this->ni+string("#")+this->methods[i]+string("."),"Puntero",0);
			}
			else if(this->methods[i][0] == ':')
			{
				this->methods[i].replace(0,1,"");
				data->CrearVariable(this->ni+string("#")+this->methods[i]+string("."));
				data->ObtenerVariable(this->ni+string("#")+this->methods[i]+string("."))
	      = data->ObtenerVariable(this->nm+string("#")+this->methods[i]+string(".#cod."));
			}
			else
			{
				data->CrearVariable(this->ni+string("#")+this->methods[i]+string("."));
			}
		}
		data->ObtenerVariable(this->ni+"#NOMBRE.") = this->ni;
		data->ObtenerVariable(this->ni+"#Tipo.") = this->nm;
	}
	
	void PseudoReferenciaClase::LeerParametros(istream& in)
	{
		if(!(in >> this->nm >> this->ni))
		{
			cerr << "Error en la creacion del " << this->ObtenerClave() << "," << "(ERROR,VARIABLE INEXISTENTE)" << " No se pudo leer bien el fichero de entrada." << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " EOF inesperado");
			return;
		}
	}
	
	void PseudoDebug::InscribirInstancia(PDDatos* data)
	{
		cout << endl << "Debugger de PseudoD" << endl;
		cout << "PseudoD y el NEA de PseudoD fueron creados por Alejandro Linarez Rangel" << endl;
		cout << "junto con este debugger" << endl;
		cout << "Al salir con la orden salir se seguira ejecutando el programa" << endl;
		string i;
		while(i != "salir")
		{
			cout << "DEBUG>> ";
			cin >> i;
			if(i == "variable")
			{
				string var;
				cin >> var;
				int ind = data->BuscarIndice("Variable",var);
				if(ind == -1)
				{
					cout << "NO EXISTE" << endl;
				}
				cout << "Variable [" << var << "]: valor[\"" << data->ObtenerVariable(var) << "\"] indice [" << ind << "]" << endl;
			}
			if(i == "puntero")
			{
				string var,var2 = "NO EXISTE";
				cin >> var;
				int ind = data->BuscarIndice("Puntero",var);
				if(ind == -1)
				{
					cout << "NO EXISTE" << endl;
				}
				else
				{
					var2 = (*data->nombrev)[(*data->nvapunt)[ind]];
				}
				cout << "Puntero [" << var << "]: valor[\"" << data->ObtenerPuntero(var)
					<< "\"] indice [" << ind << "] apunta a " << var2 << endl;
			}
			if(i == "pila")
			{
				int pil;
				cin >> pil;
				if((pil < 0)||(pil > (*data->pilas).size()))
				{
					cout << "NO EXISTE" << endl;
				}
				auto buffer = (*data->pilas)[pil];
				cout << "+----------PILA " << pil << "-------+" << endl;
				while(!buffer.empty())
				{
					cout << "|" << buffer.top() << "|" << endl;
					buffer.pop();
				}
			}
			if(i == "cantidad-de")
			{
				string que;
				cin >> que;
				if(que == "variables")
				{
					cout << "Hay " << (*data->nombrev).size() << " variables" << endl;
				}
				if(que == "punteros")
				{
					cout << "Hay " << (*data->nombrep).size() << " punteros" << endl;
				}
				if(que == "pilas")
				{
					cout << "Hay " << (*data->pilas).size() << " pilas" << endl;
				}
			}
			if((i == "estructura")||(i == "clase"))
			{
				string est;
				cin >> est;
				int ind = data->BuscarIndice("Variable",est);
				if(ind == -1)
				{
					cout << "NO EXISTE" << endl;
				}
				else
				{
					try
					{
						cout << "La estructura " << est << " tiene los siguientes campos:" << endl;
						for(int i = 0;i < cae(data->ObtenerVariable(est));i++)
						{
							cout << "    " << data->ObtenerVariable(est+"#("+eas(i)+").") << endl;
						}
					}
					catch(const exception& e)
					{
						if((string(e.what()) == "stoi")||(string(e.what()) == "stoll"))
						{
							cerr << "Error, la clase no existe." << endl;
						}
						else
						{
							cerr << e.what() << endl;
						}
					}
				}
			}
			if(i == "instancia")
			{
				cout << "ADVERTENCIA: termporalmente se supone que TODAS las estructuras derivan de Objeto..." << endl;
				string var;
				cin >> var;
				try
				{
					string tipo = data->ObtenerVariable(var+"#Tipo.");
					cout << "La instancia del tipo " << tipo << " nombrada " << var << " tiene los campos:" << endl;
					int met = cae(data->ObtenerVariable(tipo));
					for (int i = 0; i < met; i += 1)
					{
						string campo = data->ObtenerVariable(tipo+"#("+eas(i)+").");
						cout << "     ";
						bool b = false;
						if((campo[0] == ':')||(campo[0] == ';'))
						{
							if(campo[0] == ':')
							{
								cout << "[METODO]";
								b = true;
							}
							else
								cout << "[PUNTERO]";
							campo.replace(0,1,"");
						}
						string valor = data->ObtenerVariable(var+"#"+campo+".");
						if(b)
							valor = tipo+"#"+campo+".";
						cout << campo << "  =  " << valor << endl;
					}
					cout << "    [VALOR BRUTO]  =  " << data->ObtenerVariable(var) << endl;
				}
				catch(const exception& e)
				{
					if((string(e.what()) == "stoi")||(string(e.what()) == "stoll"))
					{
						cerr << "No existe la instancia" << endl;
					}
					else
					{
						cerr << e.what() << endl;
					}
				}
			}
			if(i == "ejecutar")
			{
				string ord;
				cin >> ord;
				(*data->PROCESAR)(ord,cin,data->PROCESO);
			}
		}
	}
	
	PseudoArrayEstructura::PseudoArrayEstructura(string na,string ne,int ta) : PDInstancia()
	{
		this->nme=ne;
		this->nma=na;
		this->tma=ta;
		this->FijarClave(string("EstrucArray"),string("Tipos"));
	}
	
	PseudoArrayEstructura::~PseudoArrayEstructura()
	{
		//NADA
	}
	
	void PseudoArrayEstructura::LeerParametros(istream& in)
	{
		if(!(in >> this->nme >> this->nma >> this->tma))
		{
			cerr << "Error en " << this->ObtenerClave() << " No se ha podido leer bien el fichero de entrada." << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " EOF inesperado");
		}
	}
	
	void PseudoArrayEstructura::InscribirInstancia(PDDatos* data)
	{
		int tme = cae(data->ObtenerVariable(this->nme));
		int tmt = tme + this->tma + 1;// El 1 es de estruc#array#longitud
		data->ObtenerVariable(this->nme) = eas(tmt+1);
		int ind = 0;
		for(int i = tme; i < tmt; i++)
		{
			data->CrearVariable(this->nme+string("#(")+eas(i)+string(")."));
			data->ObtenerVariable(this->nme+string("#(")+eas(i)+string(").")) = this->nma+string("#(")+eas(ind)+string(")");
			ind++;
		}
		data->CrearVariable(this->nme+string("#(")+eas(tmt)+string(")."));
		data->ObtenerVariable(this->nme+string("#(")+eas(tmt)+string(")."))=this->nma+string("#longitud");
	}
	
	PseudoBorrarVariable::PseudoBorrarVariable(string n) : PDInstancia()
	{
		this->nm = n;
		this->FijarClave(string("BorrarSimple"),string("Tipos"));
	}
	
	PseudoBorrarVariable::~PseudoBorrarVariable()
	{
		//NADA
	}
	
	void PseudoBorrarVariable::LeerParametros(istream& in)
	{
		if(!(in >> this->nm))
		{
			cerr << "Error en " << this->ObtenerClave() << " ,No se pudo leer bien el fichero de entrada" << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " EOF inesperado");
			return;
		}
	}
	
	void PseudoBorrarVariable::InscribirInstancia(PDDatos* data)
	{
		int i = data->BuscarIndice("Variable",this->nm);
		if(i == -1)
		{
			i = data->BuscarIndice("Puntero",this->nm);
			if(i == -1)
			{
				cerr << "ERROR EN " << this->ObtenerClave() << " ,LA VARIABLE O PUNTERO " << this->nm << " NO EXISTE." << endl;
				throw string("Error en el la parte " + this->ObtenerClave() + " No existe la variable o puntero.");
				return;
			}
			for (int j = i+1; j < (*data->nombrev).size(); j += 1)
			{
				(*data->nombrep)[j-1] = (*data->nombrep)[j];
				(*data->nvapunt)[j-1] = (*data->nvapunt)[j];
			}
			(*data->nombrep).pop_back();
			(*data->nvapunt).pop_back();
		}
		else
		{
			for (int j = i+1; j < (*data->nombrev).size(); j += 1)
			{
				(*data->nombrev)[j-1] = (*data->nombrev)[j];
				(*data->valorv)[j-1] = (*data->valorv)[j];
			}
			(*data->nombrev).pop_back();
			(*data->valorv).pop_back();
		}
		
	}
	
	PseudoHerencia::PseudoHerencia(string b,string h)
	{
		this->nmb = b;
		this->nmh = h;
		this->FijarClave(string("Heredar"),string("Tipos"));
	}
	
	PseudoHerencia::~PseudoHerencia()
	{
		//NADA
	}
	
	void PseudoHerencia::LeerParametros(istream& in)
	{
		if(!(in >> this->nmb >> this->nmh))
		{
			cerr << "Error en " << this->ObtenerClave() << " no se pudo leer bien el fichero fuente." << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " EOF inesperado");
		}
	}
	
	void PseudoHerencia::InscribirInstancia(PDDatos* data)
	{
		int tmb,tmh,tmha;
		tmb = cae(data->ObtenerVariable(this->nmb));
		tmh = cae(data->ObtenerVariable(this->nmh));
		tmha = tmh;
		tmh += tmb;
		data->ObtenerVariable(this->nmh) = eas(tmh);
		int icb=0,ich=tmha;
		for (icb = 0; icb < tmb; icb += 1)
		{
			data->CrearVariable(this->nmh+string("#(")+eas(ich)+string(")."));
			data->ObtenerVariable(this->nmh+string("#(")+eas(ich)+string(").")) = data->ObtenerVariable(this->nmb+string("#(")+eas(icb)+string(")."));
			ich++;
		}
	}
	
	PseudoDireccionarPuntero::PseudoDireccionarPuntero(string p,string v) : PDInstancia()
	{
		this->nmp = p;
		this->nmv = v;
		this->FijarClave(string("Redireccionar"),string("Tipos"));
	}
	
	PseudoDireccionarPuntero::~PseudoDireccionarPuntero()
	{
		
	}
	
	void PseudoDireccionarPuntero::LeerParametros(istream& in)
	{
		if(!(in >> this->nmp >> this->nmv))
		{
			cerr << "Error en " << this->ObtenerClave() << " , no se pudo leer bien el fichero fuente." << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " EOF inesperado");
		}
	}
	
	void PseudoDireccionarPuntero::InscribirInstancia(PDDatos* data)
	{
		int i = data->BuscarIndice("Puntero",this->nmp);
		int oi;
		if(i == -1)
		{
			cerr << "Error en " << this->ObtenerClave() << " ,No existe el puntero " << this->nmp << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " No existe el puntero");
			return;
		}
		oi = data->BuscarIndice("Variable",this->nmv);
		if(oi == -1)
		{
			cerr << "Error en " << this->ObtenerClave() << " ,No existe la variable,(NOTA:variable no puntero) " << this->nmv << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " No existe la variable(no puntero)");
			return;
		}
		(*data->nvapunt)[i] = oi;
	}
	
	PseudoMientras::PseudoMientras(string v,string f) : PDInstancia()
	{
		this->nmv = v;
		this->func = f;
		this->llamada = false;
		this->igualdad = false;
		this->ejecutar = false;
		this->comparar = false;
		this->FijarClave(string("Mientras"),string("PseudoD"));
	}
	PseudoMientras::~PseudoMientras()
	{
		
	}
	
	void PseudoMientras::LeerParametros(istream& in)
	{
		if(!(in >> this->nmv))
		{
			cerr << "Error en " << this->ObtenerClave() << " ,no se pudo leer bien el fichero fuente." << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " EOF inesperado");
		}
		if(this->nmv == "llamar")
		{
			string g;
			while(g != "#(Final).")
			{
				string h;
				in >> h;
				g += " "+h;
			}
			this->var1 = g;
			this->llamada = true;
		}
		else if(this->nmv == "¿son_iguales?")
		{
			in >> this->var1 >> this->var2;
			this->igualdad = true;
		}
		else if(this->nmv == "ejecutar")
		{
			string es;
			in >> this->orden >> this->var1 >> this->var2 >> es >> this->var3;
			if(es != "es")
				throw string("Error de sintaxis en " + this->ObtenerClave() + ": mientras ejecutar oden var var  \"es\" var: no se escribio es, se escribio "+es);
			this->ejecutar = true;
		}
		else if(this->nmv == "comparar")
		{
			in >> this->orden >> this->var1 >> this->oper >> this->var2;
			this->comparar = true;
		}
		vector<string> lineas;
		string lin = "";
		while(lin != "finbucle")
		{
			getline(in,lin,'\n');
			lineas.push_back(lin);
			lin.erase(std::remove_if(lin.begin(), 
                              lin.end(),
                              [](char x){return std::isspace(x);}),
                lin.end());
		}
		this->func = "";
		for(int i = 0;i < lineas.size();i++)
		{
			this->func += lineas[i] + "\n";
		}
	}
	
	void PseudoMientras::InscribirInstancia(PDDatos* data)
	{
		bool exec = false;
		if(this->llamada)
		{
			istringstream in(this->var1+" #(Final).");
			(*data->PROCESAR)("llamar",in,(*data->PROCESO));
			exec = ((((*data->pilas)[cae(data->ObtenerVariable("VG_PILA_ACTUAL"))].top()) == data->VERDADERO)? true : false);
		}
		else if(this->igualdad)
		{
			exec = data->ObtenerVariable(this->var1) == data->ObtenerVariable(this->var2);
		}
		else if(this->comparar)
		{
			istringstream in(this->var1 + " " + this->oper + " " + this->var2
													+ " ___codigo_pseudod_buffer_interno___");
			(*data->PROCESAR)(this->orden,in,(*data->PROCESO));
			exec = ((data->ObtenerVariable("___codigo_pseudod_buffer_interno___") == data->VERDADERO)? true : false);
		}
		else if(this->ejecutar)
		{
			istringstream in(this->var1 + " " + this->var2
													+ " ___codigo_pseudod_buffer_interno___");
			(*data->PROCESAR)(this->orden,in,(*data->PROCESO));
			exec = ((data->ObtenerVariable("___codigo_pseudod_buffer_interno___") == data->ObtenerVariable(this->var3))? true : false);
		}
		else
		{
			exec = ((data->ObtenerVariable(this->nmv) == data->VERDADERO)? true : false);
		}
		while(exec)
		{
			std::istringstream in2(this->func);
			string v;
			while(in2 >> v)
			{
				(*data->PROCESAR)(v,in2,(*data->PROCESO));
			}
			if(this->llamada)
			{
			istringstream in(this->var1+" #(Final).");
				(*data->PROCESAR)("llamar",in,(*data->PROCESO));
	//			exec = ((data->ObtenerVariable("___codigo_pseudod_buffer_interno___")));
					exec = (((*data->pilas)[cae(data->ObtenerVariable("VG_PILA_ACTUAL"))].top() == data->VERDADERO)? true : false);
			}
			else if(this->igualdad)
			{
				exec = data->ObtenerVariable(this->var1) == data->ObtenerVariable(this->var2);
			}
			else if(this->comparar)
			{
				istringstream in(this->var1 + " " + this->oper + " " + this->var2
														+ " ___codigo_pseudod_buffer_interno___");
				(*data->PROCESAR)(this->orden,in,(*data->PROCESO));
				exec = ((data->ObtenerVariable("___codigo_pseudod_buffer_interno___") == data->VERDADERO)? true : false);
			}
			else if(this->ejecutar)
			{
				istringstream in(this->var1 + " " + this->var2
														+ " ___codigo_pseudod_buffer_interno___");
				(*data->PROCESAR)(this->orden,in,(*data->PROCESO));
				exec = ((data->ObtenerVariable("___codigo_pseudod_buffer_interno___") == data->ObtenerVariable(this->var3))? true : false);
			}
			else
			{
				exec = ((data->ObtenerVariable(this->nmv) == data->VERDADERO)? true : false);
			}
		}
		if(this->llamada)
		{
			(*data->pilas)[cae(data->ObtenerVariable("VG_PILA_ACTUAL"))].pop();
		}
	}
	
	PseudoClaseContenida::PseudoClaseContenida(string es,string tp,string cm,bool pn) : PDInstancia()
	{
		this->nme = es; // estructura
		this->tpe = tp; // estructura del campo
		this->nmv = cm; // campo
		this->ptr = pn; // puntero?
		this->FijarClave(string("EstrucEstruc"),string("Tipos"));
	}
	
	PseudoClaseContenida::~PseudoClaseContenida()
	{
		
	}
	
	void PseudoClaseContenida::LeerParametros(istream& in)
	{
		if(!(in >> this->ptr >> this->nme >> this->tpe >> this->nmv))
		{
			cerr << "Error en " << this->ObtenerClave() << " no se pudo leer bien el fichero fuente." << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " EOF inesperado");
		}
	}
	
	void PseudoClaseContenida::InscribirInstancia(PDDatos* data)
	{
		int tme,ttpe,ate;
		tme = cae(data->ObtenerVariable(this->nme));
		ttpe = cae(data->ObtenerVariable(this->tpe));
		ate = tme;
		tme += ttpe;
		data->ObtenerVariable(this->nme) = eas(tme);
		for (int i = ate; i < tme; i += 1)
		{
			data->CrearVariable(this->nme+string("#(")+eas(i)+string(")."));
			if(this->ptr)
				data->ObtenerVariable(this->nme+string("#(")+eas(i)+string(").")) = string(";")+this->nmv+string("#")+data->ObtenerVariable(this->tpe+string("#(")+eas(i-ate)+string(")."));
			else
				data->ObtenerVariable(this->nme+string("#(")+eas(i)+string(").")) = this->nmv+string("#")+data->ObtenerVariable(this->tpe+string("#(")+eas(i-ate)+string(")."));
		}
	}
	
	PseudoBorrarInteligente::PseudoBorrarInteligente(string var)
	{
		this->var = var;
		this->FijarClave(string("Borrar"),string("Tipos"));
	}
	
	PseudoBorrarInteligente::~PseudoBorrarInteligente()
	{
		// nada
	}
	
	void PseudoBorrarInteligente::LeerParametros(istream& i)
	{
		if(!(i >> this->var))
		{
			cerr << "Error en " << this->ObtenerClave() << " no se pude leer bien el fichero fuente" << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " EOF inesperado");
		}
	}
	
	void PseudoBorrarInteligente::InscribirInstancia(PDDatos* data)
	{
		// Primero se determina el tipo de variable y se busca, siempre se borra la
		// variable #NOMBRE. y #Tipo. sin importar su tipo, si no se encuentra
		// ignora, se usa PseudoBorrarVariable
		string tipo,nombre;
		tipo = data->ObtenerVariable(this->var+string("#Tipo."));
		nombre = data->ObtenerVariable(this->var+string("#NOMBRE."));
		PseudoBorrarVariable bv(this->var);
		bv.InscribirInstancia(data);
		/*
		PseudoBorrarVariable bn(this->var+string("#NOMBRE."));
		bn.InscribirInstancia(data);
		PseudoBorrarVariable bt(this->var+string("#Tipo."));
		bt.InscribirInstancia(data);
		//*/
		vector<string> metodos;
		long lg = cae(data->ObtenerVariable(tipo));
		for (int i = 0; i < lg; i += 1)
		{
			if((data->ObtenerVariable(tipo+string("#(")+eas(i)+string(")."))[0] == ';')
				||(data->ObtenerVariable(tipo+string("#(")+eas(i)+string(")."))[0] == ':'))
			{
				string G = data->ObtenerVariable(tipo+string("#(")+eas(i)+string(")."));
				G.replace(0,1,"");
				metodos.push_back(G);
			}
			else
			{
				metodos.push_back(data->ObtenerVariable(tipo+string("#(")+eas(i)+string(").")));
			}
		}
		for (int i = 0; i < metodos.size(); i += 1)
		{
				PseudoBorrarVariable a(this->var+string("#")+metodos[i]+string("."));
				a.InscribirInstancia(data);
		}
	}
}
