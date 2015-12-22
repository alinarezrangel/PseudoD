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
			data->CrearVariable(this->nm + string("#(") + eas(i) + string(")"));
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
		data->ObtenerVariable(this->nm) = "";
		for(int i = 0;i < this->methods.size();i++)
		{
			data->ObtenerVariable(this->nm) += this->methods[i] + " ";
		}
		data->CrearVariable(this->nm + string("#Tipo"));
		data->ObtenerVariable(this->nm + string("#Tipo")) = this->nm;
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
		/*int lg = cae(data->ObtenerVariable(this->nm));
		
		for(int i = 0;i < lg;i++)
		{
			this->methods.push_back(data->ObtenerVariable(this->nm+string("#(")+eas(i)+string(").")));
		}*/
		
		string buff = "";
		for(int i = 0;i < data->ObtenerVariable(this->nm).size();i++)
		{
			if(data->ObtenerVariable(this->nm)[i] == ' ')
			{
				if(buff != "")
					this->methods.push_back(buff);
				buff = "";
				continue;
			}
			buff += data->ObtenerVariable(this->nm)[i];
		}
		if(buff != "")
			this->methods.push_back(buff);
		buff = "";
		
		for(int i = 0;i < this->methods.size();i++)
		{
			if(this->methods[i][0] == ';')
			{
				this->methods[i].replace(0,1,"");
				data->CrearVariable(this->ni + string("#") + this->methods[i],"Puntero",0);
			}
			else if(this->methods[i][0] == ':')
			{
				this->methods[i].replace(0,1,"");
				data->CrearVariable(this->ni + string("#") + this->methods[i],"Puntero",0);
				data->CrearVariable(this->ni + string("#") + this->methods[i] + string("#cod"),"Puntero",0);
				// No tiene sentido tener una copia de cada metodo de la instancia,
				// En cambio, puedes tener un puntero al metodo, que ocupa menos
				// espacio.
				data->ObtenerIndicePuntero(this->ni + string("#") + this->methods[i])
				= data->BuscarIndice("Variable",this->nm + string("#")
				                     + this->methods[i]);
				data->ObtenerIndicePuntero(this->ni + string("#") + this->methods[i] + "#cod")
				= data->BuscarIndice("Variable",this->nm + string("#")
				                     + this->methods[i] + string("#cod"));
				/*data->ObtenerVariable(this->ni+string("#")+this->methods[i])
	      = data->ObtenerVariable(this->nm+string("#")+this->methods[i]+string("#cod"));*/
			}
			else
			{
				data->CrearVariable(this->ni+string("#")+this->methods[i]);
			}
		}
		data->ObtenerVariable(this->ni+"#NOMBRE") = this->ni;
		data->ObtenerVariable(this->ni+"#Tipo") = this->nm;
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
		// Reconstuccion total del depurador
		while(i != "salir")
		{
			try
			{
				cout << "DEBUG>> ";
				cin >> i;
				if(i == "variable")
				{
					string var;
					cin >> var;
					string val = data->ObtenerVariable(var);
					cout << "La variable,(o puntero) " << var << " posee el valor \"" << val << "\"" << endl;
				}
				else if(i == "puntero")
				{
					string ptr,vptr,nvar;
					int iptr;
					cin >> ptr;
					vptr = data->ObtenerVariable(ptr);
					iptr = data->ObtenerIndicePuntero(ptr);
					if(iptr != -1)
						nvar = (*data->nombrev)[iptr];
					cout << "El puntero " << ptr << " posee el valor,(apuntado) \"" << vptr << "\"" << endl
					 << " y apunta a la direccion [" << iptr << "] del campo de variables." << endl
					 << "La variable apuntada es {" << nvar << "}" << endl;
				}
				else if(i == "pila")
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
				else if(i == "numero-de")
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
				else if(i == "ejecutar")
				{
					string ord;
					getline(cin,ord,'\n');
					data->Ejecutar(ord);
				}
				else if(i == "instancia")
				{
					cout << "Advertencia: la instancia debe poseer los atributos fundamentales..." << endl;
					string var;
					cin >> var;
					string tipo = data->ObtenerVariable(var + string("#Tipo"));
					cout << "La instancia del tipo " << tipo << " nombrada " << var << " tiene los campos:" << endl;
					int met = cae(data->ObtenerVariable(tipo));
					for (int i = 0; i < met; i += 1)
					{
						string campo = data->ObtenerVariable(tipo + "#(" + eas(i) + ").");
						cout << "     ";
						bool b = false,p = false;
						if((campo[0] == ':')||(campo[0] == ';'))
						{
							if(campo[0] == ':')
							{
								cout << "[METODO]";
								b = true;
							}
							else
							{
								cout << "[PUNTERO]";
								p = true;
							}
							campo.replace(0,1,"");
						}
						string valor = data->ObtenerVariable(var+"#"+campo);
						if(b)
							valor = tipo + "#" + campo;
						if(p)
							valor = "DIR[" + eas(data->ObtenerIndicePuntero(var+"#"+campo)) + "] VAL[" + valor + "] NVARPTR[" + (*data->nombrev)[data->ObtenerIndicePuntero(var+"#"+campo)] + "]";
						cout << campo << "  =  " << valor << endl;
					}
					cout << "    [VALOR BRUTO]  =  " << data->ObtenerVariable(var) << endl;
				}
				else if(i == "clase")
				{
					string est;
					cin >> est;
					cout << "La clase " << est << " tiene los siguientes campos:" << endl;
					for(int i = 0;i < cae(data->ObtenerVariable(est));i++)
					{
						cout << "    " << data->ObtenerVariable(est+"#("+eas(i)+").") << endl;
					}
				}
				else if(i != "salir")
				{
					data->Ejecutar(i,cin);
				}
			}
			catch(const string& e)
			{
				cerr << "PseudoD lanzo un error fatal: " << e << endl;
				cerr << "EN " << data->ObtenerVariable("__ARCH__") << endl;
				cerr << "Captado por el debugger" << endl;
			}
		}
		// Anterior depurador
		/* while(i != "salir")
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
					string tipo = data->ObtenerVariable(var+"#Tipo");
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
						string valor = data->ObtenerVariable(var+"#"+campo);
						if(b)
							valor = tipo+"#"+campo;
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
		} */
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
			(*data->nombrep).erase((*data->nombrep).begin() + i);
			(*data->nvapunt).erase((*data->nvapunt).begin() + i);
		/*cout << "Borrando bloque de memoria 6" << endl;
		cout << "I:" << (i+1) << ";bkls:" << (*data->nombrev).size() << endl;
			for (int j = i+1; j < (*data->nombrev).size(); j += 1)
			{
		cout << "Overloading el bloque " << (j-1) << " para " << j << ":[" << (*data->nombrep)[j-1] << "," << (*data->nombrep)[j] << "]" << endl;
				(*data->nombrep)[j-1] = (*data->nombrep)[j];
				(*data->nvapunt)[j-1] = (*data->nvapunt)[j];
			}
		cout << "Borrando bloque de memoria 7" << endl;
			(*data->nombrep).pop_back();
			(*data->nvapunt).pop_back();
		cout << "Borrando bloque de memoria 8" << endl;*/
		}
		else
		{
			(*data->nombrev).erase((*data->nombrev).begin() + i);
			(*data->valorv).erase((*data->valorv).begin() + i);
			/*
		cout << "Borrando bloque de memoria 9" << endl;
			for (int j = i+1; j < (*data->nombrev).size(); j += 1)
			{
				(*data->nombrev)[j-1] = (*data->nombrev)[j];
				(*data->valorv)[j-1] = (*data->valorv)[j];
			}
		cout << "Borrando bloque de memoria 10" << endl;
			(*data->nombrev).pop_back();
			(*data->valorv).pop_back();
		cout << "Borrando bloque de memoria 11" << endl;*/
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
		/*int tmb,tmh,tmha;
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
		}*/
		data->ObtenerVariable(this->nmh) += " " + data->ObtenerVariable(this->nmb);
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
		int oi;
		if(data->BuscarIndice("Puntero",this->nmp) == -1)
		{
			cerr << "Error en " << this->ObtenerClave() << " ,No existe el puntero " << this->nmp << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " No existe el puntero");
		}
		oi = data->BuscarIndice("Variable",this->nmv);
		if(oi == -1)
		{
			cerr << "Error en " << this->ObtenerClave() << " ,No existe la variable,(NOTA:variable no puntero) " << this->nmv << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " No existe la variable(no puntero)");
		}
		data->ObtenerIndicePuntero(this->nmp) = oi;
	}
	
	PseudoMientras::PseudoMientras(string v,string o,string f) : PDInstancia()
	{
		this->nmv = v;
		this->func = f;
		this->orden = o;
		this->FijarClave(string("Mientras"),string("PseudoD"));
	}
	PseudoMientras::~PseudoMientras()
	{
		
	}
	
	void PseudoMientras::LeerParametros(istream& in)
	{
		// FIXME: Codigo problematico:
		/*if(!(in >> this->nmv))
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
				throw string("Error de sintaxis en " + this->ObtenerClave() + ": mientras ejecutar orden var var  \"es\" var: no se escribio es, se escribio "+es);
			this->ejecutar = true;
		}
		else if(this->nmv == "comparar")
		{
			in >> this->orden >> this->var1 >> this->oper >> this->var2;
			this->comparar = true;
		}
		vector<string> lineas;
		string lin = "";
		int mientras = 1;
		while(mientras != 0)
		{
			getline(in,lin,'\n');
			lineas.push_back(lin);
			lin.erase(std::remove_if(lin.begin(), 
                              lin.end(),
                              [](char x){return std::isspace(x);}),
                lin.end());
      if((lin == "mientras") || (lin == "Importar.PseudoD.mientras"))
      {
      	mientras++;
      }
      if(lin == "finbucle")
      {
      	mientras--;
      }
		}
		this->func = "";
		for(int i = 0;i < lineas.size();i++)
		{
			this->func += lineas[i] + "\n";
		}*/
		// TODO: Adaptado a usar ValorDelToken
		in >> this->nmv;
		getline(in,this->orden,'\n');
		vector<string> lineas;
		string lin = "";
		int mientras = 1;
		while(mientras > 0)
		{
			getline(in,lin,'\n');
			lineas.push_back(lin);
			lin.erase(std::remove_if(lin.begin(), 
                              lin.end(),
                              [](char x){return std::isspace(x);}),
                lin.end());
      if((string(lin.substr(0,string("mientras").size())) == "mientras") || (string(lin.substr(0,string("Importar.PseudoD.mientras").size())) == "Importar.PseudoD.mientras"))
      {
      	mientras++;
      }
      if(lin == "finbucle")
      {
      	mientras--;
      }
		}
		this->func = "";
		for(int i = 0;i < lineas.size();i++)
		{
			this->func += lineas[i] + "\n";
		}
	}
	
	void PseudoMientras::InscribirInstancia(PDDatos* data)
	{
		// FIXME: Codigo problematico:
		/*bool exec = false;
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
		}*/
		istringstream sin(this->orden);
		string res = ValorDelToken(this->nmv,sin,data);
		while(res == "verdadero")
		{
			string pal;
			istringstream sin2(this->func);
			while(sin2 >> pal)
			{
				data->Ejecutar(pal,sin2);
			}
			istringstream sin3(this->orden);
			res = ValorDelToken(this->nmv,sin3,data);
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
		tipo = data->ObtenerVariable(this->var+string("#Tipo"));
		nombre = data->ObtenerVariable(this->var+string("#NOMBRE"));
		PseudoBorrarVariable bv(this->var);
		bv.InscribirInstancia(data);
		/*
		PseudoBorrarVariable bn(this->var+string("#NOMBRE."));
		bn.InscribirInstancia(data);
		PseudoBorrarVariable bt(this->var+string("#Tipo."));
		bt.InscribirInstancia(data);
		//*/
		vector<string> metodos;
		string buff = "";
		for(int i = 0;i < data->ObtenerVariable(tipo).size();i++)
		{
			if(data->ObtenerVariable(tipo)[i] == ' ')
			{
				if(buff != "")
				{
					metodos.push_back(buff);
					buff = "";
				}
				continue;
			}
			buff += data->ObtenerVariable(tipo)[i];
		}
		for (int i = 0; i < metodos.size(); i += 1)
		{
			if((metodos[i][0] == ';')
				||(metodos[i][0] == ':'))
			{
				string G = metodos[i];
				G.replace(0,1,"");
				metodos[i] = G;
			}
		}
		for (int i = 0; i < metodos.size(); i += 1)
		{
			PseudoBorrarVariable a(this->var+string("#")+metodos[i]);
			a.InscribirInstancia(data);
		}
	}
}
