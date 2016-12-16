/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*****                                                                  *****
***** Página oficial de PseudoD en:                                    *****
*****     http://pseudod.sourceforge.net/                              *****
*****                                                                  *****
****************************************************************************
**************************************************************************/


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
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " nom len': EOF inesperado"
			);
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
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " nm ... FIN' alias 'clase nm ... FIN': EOF inesperado"
			);
		}
		string b;
		if(!(in >> b))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " nm ... FIN' alias 'clase nm ... FIN': EOF inesperado"
			);
		}
		while((b != "#(Final).")&&(b != "finclase"))
		{
			this->methods.push_back(b);
			if(!(in >> b))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en "
					+ this->ObtenerClave()
					+ ": '"
					+ this->ObtenerClave()
					+ " nm ... FIN' alias 'clase nm ... FIN': EOF inesperado"
				);
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
		data->ObtenerVariable(this->ni) = this->ni;

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
				data->CrearVariable(this->ni + string("#") + this->methods[i], false,0);
			}
			else if(this->methods[i][0] == ':')
			{
				this->methods[i].replace(0,1,"");
				data->CrearVariable(this->ni + string("#") + this->methods[i], false,0);
				data->CrearVariable(this->ni + string("#") + this->methods[i] + string("#cod"), false,0);
				// No tiene sentido tener una copia de cada metodo de la instancia,
				// En cambio, puedes tener un puntero al metodo, que ocupa menos
				// espacio.
				data->ObtenerIndicePuntero(this->ni + string("#") + this->methods[i])
					= data->BuscarIndice(true, this->nm + string("#")
							+ this->methods[i]);
				data->ObtenerIndicePuntero(this->ni + string("#") + this->methods[i] + string("#cod"))
					= data->BuscarIndice(true, this->nm + string("#")
							+ this->methods[i]);
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
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " cls nm' alias 'instancia cls nm': EOF inesperado"
			);
			return;
		}
	}

	void PseudoDebug::InscribirInstancia(PDDatos* data)
	{
		cout << endl << "@ Debugger(depurador) de PseudoD" << endl;
		cout << "@ Al salir con la orden salir se seguira ejecutando el programa" << endl;
		cout << "@ Puedes ver la ayuda con \'ayuda\'" << endl;
		string i;
		// Reconstuccion total del depurador
		while(i != "salir")
		{
			try
			{
				cout << "@ DEBUG>> ";
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
					cout << "+----------PILA " << pil << "--------+" << endl;
					while(!buffer.empty())
					{
						cout << "|" << std::setw(24) << std::setfill(' ') << std::right << buffer.top() << "|" << endl;
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
					string var = "", est = "";
					cin >> var;
					est = data->ObtenerVariable(var + string("#Tipo"));
					cout << "La instancia del tipo " << est << " nombrada " << var << " tiene los campos:" << endl;
					cout << "    [VALOR BRUTO]  =  " << data->ObtenerVariable(var) << endl;
					std::istringstream iss(data->ObtenerVariable(est));
					while(iss >> est)
					{
						bool igcnt = false;
						cout << "    ";
						if(est.front() == ':')
						{
							cout << " [METODO] ";
							igcnt = true;
						}
						if(est.front() == ';')
							cout << " [PUNTERO] ";
						if((est.front() == ':') || (est.front() == ';'))
						{
							est = est.substr(1, est.size());
						}
						string cnt = data->ObtenerVariable(var + "#" + est);
						if(cnt.size() > 80)
						{
							igcnt = true; // Contenido muy largo, ver con <variable a#b>
						}
						cout << est;
						if(!igcnt)
						{
							cout << " = " << cnt;
						}
						cout << endl;
					}
				}
				else if(i == "clase")
				{
					string est = "";
					cin >> est;
					cout << "La clase " << est << " tiene los siguientes campos:" << endl;
					std::istringstream iss(data->ObtenerVariable(est));
					while(iss >> est)
					{
						cout << " " << est << endl;
					}
				}
				else if(i == "ayuda")
				{
					cout << "Ayuda del depurador de PseudoD" << endl;
					cout << " Importar.PseudoD.debug" << endl;
					cout << "Comandos:" << endl;
					cout << " variable [n]: ver la variable [n]" << endl;
					cout << " puntero [p]: ver el puntero [p]" << endl;
					cout << " pila [l]: ver la pila [l]" << endl;
					cout << " numero-de [q]: ver la cantidad de [q], donde [q] puede" << endl;
					cout << "                ser (variables|punteros|pilas)" << endl;
					cout << " instancia [i]: ver la instancia [i](tipo auto-detectado)" << endl;
					cout << " clase [c]: ver la clase [c]" << endl;
					cout << " salir: sale del depurador" << endl;
					cout << " ayuda: muestra esta ayuda" << endl;
					cout << " volcar [q]: volcar todas los [q] de memoria, donde [q] puede ser" << endl;
					cout << "             (variables|punteros|pilas)" << endl;
					cout << " ejecutar [orden]: ejecuta el resto de la línea como código en PseudoD" << endl;
					cout << "Ejecución automatica:" << endl;
					cout << " Si el depurador no comprende alguna de las ordenes, por" << endl;
					cout << " defecto, ejecutará la linea con PseudoD." << endl;
					cout << endl;
					cout << " Si, por ejemplo: deseas llamar a una función," << endl;
					cout << " solo escribes: \'llamar [fnc]... #(Final).\'" << endl;
				}
				else if(i != "salir")
				{
					data->Ejecutar(i, cin);
				}
			}
			catch(const PDvar::Error& e)
			{
				cerr << "PseudoD lanzo un error fatal: " << e.Mensaje() << endl;
				cerr << "EN " << data->ObtenerVariable("__ARCH__") << endl;
				cerr << "Captado por el debugger" << endl;
			}
			catch(const std::exception& e)
			{
				cerr << "PseudoD lanzo un error fatal: " << e.what() << endl;
				cerr << "EN " << data->ObtenerVariable("__ARCH__") << endl;
				cerr << "Captado por el debugger" << endl;
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
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " nme nma tma': EOF inesperado"
			);
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
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " nm': EOF inesperado"
			);
			return;
		}
	}

	void PseudoBorrarVariable::InscribirInstancia(PDDatos* data)
	{
		int i = data->BuscarIndice(true, this->nm);
		if(i == -1)
		{
			i = data->BuscarIndice(false, this->nm);
			if(i == -1)
			{
				throw PDvar::ErrorDelNucleo(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " nm': No existe la variable o puntero '"
				+ this->nm
				+ "'"
			);
			}
			(*data->nombrep).erase((*data->nombrep).begin() + i);
			(*data->nvapunt).erase((*data->nvapunt).begin() + i);
		}
		else
		{
			(*data->nombrev).erase((*data->nombrev).begin() + i);
			(*data->valorv).erase((*data->valorv).begin() + i);
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
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " cls_base cls_hija' alias 'heredar cls_base cls_hija': EOF inesperado"
			);
		}
	}

	void PseudoHerencia::InscribirInstancia(PDDatos* data)
	{
		string& base = data->ObtenerVariable(this->nmb);
		string& hija = data->ObtenerVariable(this->nmh);
		string curr_mtd = "";
		for(int i = 0;i < base.size();i++)
		{
			if(base[i] == ' ')
			{
				if((curr_mtd.front() == ':')&&(hija.find(curr_mtd.c_str()) == string::npos))
				{
					// Crear una funcion llamada Hija#Metodo que sea igual a Base#Metodo
					curr_mtd.replace(0,1,"");
					string hija_mtd = this->nmh + string("#") + curr_mtd;
					string base_mtd = this->nmb + string("#") + curr_mtd;
					data->CrearVariable(hija_mtd, true,0);
					data->CrearVariable(hija_mtd + "#NOMBRE", true,0);
					data->CrearVariable(hija_mtd + "#Tipo", true,0);
					data->CrearVariable(hija_mtd + string("#cod"), true,0);
					data->ObtenerVariable(hija_mtd)
						= data->ObtenerVariable(base_mtd);
					data->ObtenerVariable(hija_mtd + "#NOMBRE") = hija_mtd;
					data->ObtenerVariable(hija_mtd + "#Tipo") = "PseudoFuncion";
					data->ObtenerIndicePuntero(hija_mtd + string("#cod"))
						= data->BuscarIndice(true, hija_mtd);
				}
				curr_mtd = "";
				continue;
			}
			curr_mtd += base[i];
		}
		hija += base;
	}

	PseudoDireccionarPuntero::PseudoDireccionarPuntero(string p,string v) : PDInstancia()
	{
		this->nmp = p;
		this->nmv = v;
		this->FijarClave(string("Redireccionar"),string("Tipos"));
	}

	PseudoDireccionarPuntero::~PseudoDireccionarPuntero()
	{
		// NADA
	}

	void PseudoDireccionarPuntero::LeerParametros(istream& in)
	{
		if(!(in >> this->nmp >> this->nmv))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " ptr nval' alias 'redireccionar ptr nval': EOF inesperado"
			);
		}
	}

	void PseudoDireccionarPuntero::InscribirInstancia(PDDatos* data)
	{
		int oi;
		if(data->BuscarIndice(false, this->nmp) == -1)
		{
			throw PDvar::ErrorDelNucleo(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " ptr nvla' alias 'redireccionar ptr nval': No existe el puntero 'ptr'"
			);
		}
		oi = data->BuscarIndice(true, this->nmv);
		if(oi == -1)
		{
			throw PDvar::ErrorDelNucleo(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " ptr nval' alias 'redireccionar ptr nval': No existe la variable 'nval'"
			);
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
		// NADA
	}

	void PseudoMientras::LeerParametros(istream& in)
	{
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
		// NADA
	}

	void PseudoClaseContenida::LeerParametros(istream& in)
	{
		if(!(in >> this->ptr >> this->nme >> this->tpe >> this->nmv))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " ptr? nme tpe nmv': EOF inesperado"
			);
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
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " var' alias 'liberar var': EOF inesperado"
			);
		}
	}

	void PseudoBorrarInteligente::InscribirInstancia(PDDatos* data)
	{
		// Primero se determina el tipo de variable y se busca, siempre se borra la
		// variable #NOMBRE. y #Tipo. sin importar su tipo, si no se encuentra
		// ignora, se usa PseudoBorrarVariable
		string tipo;
		tipo = data->ObtenerVariable(this->var+string("#Tipo"));
		int index = data->BuscarIndice(
			true,
			this->var
		);
		int todelete = 1;
		PseudoBorrarVariable bv(this->var);
		bv.InscribirInstancia(data);

		string buff = "";
		istringstream iss(data->ObtenerVariable(tipo));
		while(iss >> buff)
		{
			bool ptr = false;
			if((buff.front() == ';')
				||(buff.front() == ':'))
			{
				ptr = true;
				buff.replace(0, 1, "");
			}
			std::string varname = this->var + string("#") + buff;
			PseudoBorrarVariable a(varname);
			a.InscribirInstancia(data);
			if(!ptr)
				todelete++;
		}
		int size = data->nvapunt->size();
		for(int i = 0; i < size; i++)
		{
			int& pval = (*data->nvapunt)[i];
			if(pval >= index)
			{
				pval -= todelete;
			}
		}
	}
}

