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

	PDCadena PDOrden::ObtenerClave()
	{
		return this->clave;
	}

	void PDOrden::LeerParametros(std::istream& in)
	{
		return;
	}

	void PDOrden::FijarClave(PDCadena cl, PDCadena pack)
	{
		this->clave = pack + "." + cl;
	}

	PseudoArray::PseudoArray(PDCadena nom, PDEntero can) : PDInstancia()
	{
		this->cant = can;
		this->nm = nom;
		this->FijarClave("Array", "Tipos");
	}

	PseudoArray::~PseudoArray()
	{
		//~~~~~~nada
	}

	void PseudoArray::InscribirInstancia(PDvar::PDDatos* data)
	{
		data->CrearVariable(this->nm + "#longitud.");
		PDvar::Variante& r = data->ObtenerVariable(this->nm + "#longitud.");
		r = this->cant;
		for(PDEntero i = 0; i < this->cant;i++)
		{
			data->CrearVariable(this->nm + "#(" + eas(i) + ")");
		}
	}

	void PseudoArray::LeerParametros(std::istream& in)
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

	PseudoClase::PseudoClase(PDCadena nm, std::vector<PDCadena> mt) : PDInstancia()
	{
		this->nm = nm;
		this->methods = mt;
		this->FijarClave("Estructura", "Tipos");
	}

	PseudoClase::~PseudoClase()
	{
		//NADA
	}

	void PseudoClase::InscribirInstancia(PDvar::PDDatos* data)
	{
		data->CrearVariable(this->nm);
		PDvar::Variante& vr = data->ObtenerVariable(this->nm);
		vr = PDCadena("");
		for(unsigned int i = 0;i < this->methods.size();i++)
		{
			vr.ObtenerCadena() += this->methods[i] + " ";
		}
		data->CrearVariable(this->nm + "#Tipo");
		data->ObtenerVariable(this->nm + "#Tipo") = this->nm;
	}

	void PseudoClase::LeerParametros(std::istream& in)
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
		PDCadena b;
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
		while((b != "#(Final).") && (b != "finclase"))
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

	PseudoReferenciaClase::PseudoReferenciaClase(PDCadena n, PDCadena i) : PDInstancia()
	{
		this->nm = n;
		this->ni = i;
		this->FijarClave("Instancia", "Tipos");
	}

	PseudoReferenciaClase::~PseudoReferenciaClase()
	{
		//NADA
	}

	void PseudoReferenciaClase::InscribirInstancia(PDvar::PDDatos* data)
	{
		data->CrearVariable(this->ni);
		data->ObtenerVariable(this->ni) = this->ni;

		PDvar::Variante& vr = data->ObtenerVariable(this->nm);

		PDCadena buff = "";
		for(int i = 0;i < vr.ObtenerCadena().size();i++)
		{
			if(vr.ObtenerCadena()[i] == ' ')
			{
				if(buff != "")
					this->methods.push_back(buff);
				buff = "";
				continue;
			}
			buff += vr.ObtenerCadena()[i];
		}
		if(buff != "")
			this->methods.push_back(buff);
		buff = "";

		for(int i = 0;i < this->methods.size();i++)
		{
			if(this->methods[i][0] == ';')
			{
				this->methods[i].replace(0, 1, "");
				data->CrearVariable(this->ni + "#" + this->methods[i], false, 0);
			}
			else if(this->methods[i][0] == ':')
			{
				this->methods[i].replace(0, 1, "");
				data->CrearVariable(this->ni + "#" + this->methods[i], false, 0);
				data->CrearVariable(this->ni + "#" + this->methods[i] + "#cod", false, 0);
				// No tiene sentido tener una copia de cada metodo de la instancia,
				// En cambio, puedes tener un puntero al metodo, que ocupa menos
				// espacio.
				data->ObtenerIndicePuntero(this->ni + "#" + this->methods[i])
					= data->BuscarIndice(true, this->nm + "#" + this->methods[i]);
				data->ObtenerIndicePuntero(this->ni + "#" + this->methods[i] + "#cod")
					= data->BuscarIndice(true, this->nm + "#" + this->methods[i]);
			}
			else
			{
				data->CrearVariable(this->ni + "#" + this->methods[i]);
			}
		}
		data->ObtenerVariable(this->ni + "#NOMBRE") = this->ni;
		data->ObtenerVariable(this->ni + "#Tipo") = this->nm;
	}

	void PseudoReferenciaClase::LeerParametros(std::istream& in)
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

	void PseudoDebug::InscribirInstancia(PDvar::PDDatos* data)
	{
		std::cout << std::endl << "@ Debugger(depurador) de PseudoD" << std::endl;
		std::cout << "@ Al salir con la orden salir se seguira ejecutando el programa" << std::endl;
		std::cout << "@ Puedes ver la ayuda con \'ayuda\'" << std::endl;
		std::string i;
		// Reconstuccion total del depurador
		while(i != "salir")
		{
			try
			{
				std::cout << "@ DEBUG>> ";
				std::cin >> i;
				if(i == "variable")
				{
					PDCadena var;
					std::cin >> var;
					PDvar::Variante& val = data->ObtenerVariable(var);
					std::cout << "La variable (o puntero) " << var << " posee el valor \"" << val.ObtenerCadena() << "\"" << std::endl;
				}
				else if(i == "puntero")
				{
					PDCadena ptr = "", nvar = "";
					PDEntero iptr = 0;
					std::cin >> ptr;
					PDvar::Variante& vptr = data->ObtenerVariable(ptr);
					iptr = data->ObtenerIndicePuntero(ptr);
					if(iptr != -1)
						nvar = (*data->nombrev)[iptr];
					std::cout << "El puntero " << ptr << " posee el valor (apuntado) \"" << vptr.ObtenerCadena() << "\"" << std::endl
						<< " y apunta a la direccion [" << iptr << "] del campo de variables." << std::endl
						<< "La variable apuntada es {" << nvar << "}" << std::endl;
				}
				else if(i == "pila")
				{
					PDEntero pil;
					std::cin >> pil;
					if((pil < 0) || (pil > (*data->pilas).size()))
					{
						std::cout << "NO EXISTE" << std::endl;
					}
					auto buffer = (*data->pilas)[pil];
					std::cout << "+----------PILA " << pil << "--------+" << std::endl;
					while(!buffer.empty())
					{
						std::cout << "|" << std::setw(24) << std::setfill(' ') << std::right << buffer.top().ObtenerCadena() << "|" << std::endl;
						buffer.pop();
					}
				}
				else if(i == "numero-de")
				{
					std::string que;
					std::cin >> que;
					if(que == "variables")
					{
						std::cout << "Hay " << (*data->nombrev).size() << " variables" << std::endl;
					}
					if(que == "punteros")
					{
						std::cout << "Hay " << (*data->nombrep).size() << " punteros" << std::endl;
					}
					if(que == "pilas")
					{
						std::cout << "Hay " << (*data->pilas).size() << " pilas" << std::endl;
					}
				}
				else if(i == "ejecutar")
				{
					std::string ord;
					std::getline(std::cin, ord, '\n');
					data->Ejecutar(ord);
				}
				else if(i == "instancia")
				{
					std::cout << "Advertencia: la instancia debe poseer los atributos fundamentales..." << std::endl;
					std::string var = "";
					std::cin >> var;
					PDCadena est = data->ObtenerVariable(var + "#Tipo");
					std::cout << "La instancia del tipo " << est << " nombrada " << var << " tiene los campos:" << std::endl;
					std::cout << "    [VALOR BRUTO]  =  " << data->ObtenerVariable(var).ObtenerCadena() << std::endl;
					std::istringstream iss(data->ObtenerVariable(est));
					while(iss >> est)
					{
						bool igcnt = false;
						std::cout << "    ";
						if(est.front() == ':')
						{
							std::cout << " [METODO] ";
							igcnt = true;
						}
						if(est.front() == ';')
							std::cout << " [PUNTERO] ";
						if((est.front() == ':') || (est.front() == ';'))
						{
							est = est.substr(1, est.size());
						}
						PDvar::Variante& cnt = data->ObtenerVariable(var + "#" + est);
						if(cnt.ObtenerCadena().size() > 80)
						{
							igcnt = true; // Contenido muy largo, ver con <variable a#b>
						}
						std::cout << est;
						if(!igcnt)
						{
							std::cout << " = " << cnt.ObtenerCadena();
						}
						std::cout << std::endl;
					}
				}
				else if(i == "clase")
				{
					std::string est = "";
					std::cin >> est;
					std::cout << "La clase " << est << " tiene los siguientes campos:" << std::endl;
					std::istringstream iss(data->ObtenerVariable(est));
					while(iss >> est)
					{
						std::cout << "    " << est << std::endl;
					}
				}
				else if(i == "ayuda")
				{
					std::cout << "Ayuda del depurador de PseudoD" << std::endl;
					std::cout << " Importar.PseudoD.debug" << std::endl;
					std::cout << "Comandos:" << std::endl;
					std::cout << " variable [n]: ver la variable [n]" << std::endl;
					std::cout << " puntero [p]: ver el puntero [p]" << std::endl;
					std::cout << " pila [l]: ver la pila [l]" << std::endl;
					std::cout << " numero-de [q]: ver la cantidad de [q], donde [q] puede" << std::endl;
					std::cout << "                ser (variables|punteros|pilas)" << std::endl;
					std::cout << " instancia [i]: ver la instancia [i](tipo auto-detectado)" << std::endl;
					std::cout << " clase [c]: ver la clase [c]" << std::endl;
					std::cout << " salir: sale del depurador" << std::endl;
					std::cout << " ayuda: muestra esta ayuda" << std::endl;
					std::cout << " volcar [q]: volcar todas los [q] de memoria, donde [q] puede ser" << std::endl;
					std::cout << "             (variables|punteros|pilas)" << std::endl;
					std::cout << " ejecutar [orden]: ejecuta el resto de la línea como código en PseudoD" << std::endl;
					std::cout << "Ejecución automatica:" << std::endl;
					std::cout << " Si el depurador no comprende alguna de las ordenes, por" << std::endl;
					std::cout << " defecto, ejecutará la linea con PseudoD." << std::endl;
					std::cout << std::endl;
					std::cout << " Si, por ejemplo: deseas llamar a una función," << std::endl;
					std::cout << " solo escribes: \'llamar [fnc]... #(Final).\'" << std::endl;
				}
				else if(i != "salir")
				{
					data->Ejecutar(i, std::cin);
				}
			}
			catch(const PDvar::Error& e)
			{
				std::cerr << "PseudoD lanzo un error fatal: " << e.Mensaje() << std::endl;
				std::cerr << "EN " << data->ObtenerVariable("__ARCH__").ObtenerCadena() << std::endl;
				std::cerr << "Captado por el debugger" << std::endl;
			}
			catch(const std::exception& e)
			{
				std::cerr << "PseudoD lanzo un error fatal: " << e.what() << std::endl;
				std::cerr << "EN " << data->ObtenerVariable("__ARCH__").ObtenerCadena() << std::endl;
				std::cerr << "Captado por el debugger" << std::endl;
			}
		}
	}

	PseudoArrayEstructura::PseudoArrayEstructura(PDCadena na, PDCadena ne, PDEntero ta) : PDInstancia()
	{
		this->nme = ne;
		this->nma = na;
		this->tma = ta;
		this->FijarClave("EstrucArray", "Tipos");
	}

	PseudoArrayEstructura::~PseudoArrayEstructura()
	{
		//NADA
	}

	void PseudoArrayEstructura::LeerParametros(std::istream& in)
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

	void PseudoArrayEstructura::InscribirInstancia(PDvar::PDDatos* data)
	{
		PDEntero tme = data->ObtenerVariable(this->nme).ObtenerEntero();
		PDEntero tmt = tme + this->tma + 1;// El 1 es de estruc#array#longitud
		data->ObtenerVariable(this->nme) = eas(tmt+1);
		int ind = 0;
		for(int i = tme; i < tmt; i++)
		{
			data->CrearVariable(this->nme+std::string("#(")+eas(i)+std::string(")."));
			data->ObtenerVariable(this->nme+std::string("#(")+eas(i)+std::string(").")).ObtenerCadena() = this->nma+std::string("#(")+eas(ind)+std::string(")");
			ind++;
		}
		data->CrearVariable(this->nme+std::string("#(")+eas(tmt)+std::string(")."));
		data->ObtenerVariable(this->nme+std::string("#(")+eas(tmt)+std::string(")."))=this->nma+std::string("#longitud");
	}

	PseudoBorrarVariable::PseudoBorrarVariable(PDCadena n) : PDInstancia()
	{
		this->nm = n;
		this->FijarClave("BorrarSimple", "Tipos");
	}

	PseudoBorrarVariable::~PseudoBorrarVariable()
	{
		//NADA
	}

	void PseudoBorrarVariable::LeerParametros(std::istream& in)
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

	void PseudoBorrarVariable::InscribirInstancia(PDvar::PDDatos* data)
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

	PseudoHerencia::PseudoHerencia(PDCadena b, PDCadena h) : PDInstancia()
	{
		this->nmb = b;
		this->nmh = h;
		this->FijarClave("Heredar", "Tipos");
	}

	PseudoHerencia::~PseudoHerencia()
	{
		//NADA
	}

	void PseudoHerencia::LeerParametros(std::istream& in)
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

	void PseudoHerencia::InscribirInstancia(PDvar::PDDatos* data)
	{
		PDvar::Variante& base = data->ObtenerVariable(this->nmb);
		PDvar::Variante& hija = data->ObtenerVariable(this->nmh);
		PDCadena curr_mtd = "";
		for(int i = 0;i < base.ObtenerCadena().size();i++)
		{
			if(base.ObtenerCadena()[i] == ' ')
			{
				if((curr_mtd.front() == ':') && (hija.ObtenerCadena().find(curr_mtd.c_str()) == std::string::npos))
				{
					// Crear una funcion llamada Hija#Metodo que sea igual a Base#Metodo
					curr_mtd.replace(0,1,"");
					PDCadena hija_mtd = this->nmh + "#" + curr_mtd;
					PDCadena base_mtd = this->nmb + "#" + curr_mtd;
					data->CrearVariable(hija_mtd, true,0);
					data->CrearVariable(hija_mtd + "#NOMBRE", true,0);
					data->CrearVariable(hija_mtd + "#Tipo", true,0);
					data->CrearVariable(hija_mtd + "#cod", true,0);
					data->ObtenerVariable(hija_mtd)
						= data->ObtenerVariable(base_mtd);
					data->ObtenerVariable(hija_mtd + "#NOMBRE") = hija_mtd;
					data->ObtenerVariable(hija_mtd + "#Tipo") = PDCadena("PseudoFuncion");
					data->ObtenerIndicePuntero(hija_mtd + "#cod")
						= data->BuscarIndice(true, hija_mtd);
				}
				curr_mtd = "";
				continue;
			}
			curr_mtd += base.ObtenerCadena()[i];
		}
		hija.ObtenerCadena() += base.ObtenerCadena();
	}

	PseudoDireccionarPuntero::PseudoDireccionarPuntero(PDCadena p, PDCadena v) : PDInstancia()
	{
		this->nmp = p;
		this->nmv = v;
		this->FijarClave("Redireccionar", "Tipos");
	}

	PseudoDireccionarPuntero::~PseudoDireccionarPuntero()
	{
		// NADA
	}

	void PseudoDireccionarPuntero::LeerParametros(std::istream& in)
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

	void PseudoDireccionarPuntero::InscribirInstancia(PDvar::PDDatos* data)
	{
		PDEntero oi = 0;
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

	PseudoMientras::PseudoMientras(PDCadena v, PDCadena o, PDCadena f) : PDInstancia()
	{
		this->nmv = v;
		this->func = f;
		this->orden = o;
		this->FijarClave("Mientras", "PseudoD");
	}
	PseudoMientras::~PseudoMientras()
	{
		// NADA
	}

	void PseudoMientras::LeerParametros(std::istream& in)
	{
		in >> this->nmv;
		std::getline(in, this->orden, '\n');
		std::vector<PDCadena> lineas;
		PDCadena lin = "";
		PDEntero mientras = 1;
		while(mientras > 0)
		{
			std::getline(in, lin, '\n');
			lineas.push_back(lin);
			lin.erase(std::remove_if(lin.begin(),
				lin.end(),
				[](char x){return std::isspace(x);}),
				lin.end());
			if((lin.substr(0, PDCadena("mientras").size()) == PDCadena("mientras"))
				|| (lin.substr(0, PDCadena("Importar.PseudoD.Mientras").size()) == PDCadena("Importar.PseudoD.Mientras")))
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

	void PseudoMientras::InscribirInstancia(PDvar::PDDatos* data)
	{
		std::istringstream sin(this->orden);
		PDvar::Variante res = PDvar::ValorDelToken(this->nmv, sin, data);
		while(res == data->VERDADERO)
		{
			PDCadena pal = "";
			std::istringstream sin2(this->func);
			while(sin2 >> pal)
			{
				data->Ejecutar(pal,sin2);
			}
			std::istringstream sin3(this->orden);
			res = PDvar::ValorDelToken(this->nmv, sin3, data);
		}
	}

	PseudoClaseContenida::PseudoClaseContenida(PDCadena es, PDCadena tp, PDCadena cm, bool pn) : PDInstancia()
	{
		this->nme = es; // estructura
		this->tpe = tp; // estructura del campo
		this->nmv = cm; // campo
		this->ptr = pn; // puntero?
		this->FijarClave("EstrucEstruc", "Tipos");
	}

	PseudoClaseContenida::~PseudoClaseContenida()
	{
		// NADA
	}

	void PseudoClaseContenida::LeerParametros(std::istream& in)
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

	void PseudoClaseContenida::InscribirInstancia(PDvar::PDDatos* data)
	{
		PDEntero tme = 0, ttpe = 0, ate = 0;
		tme = data->ObtenerVariable(this->nme).ObtenerEntero();
		ttpe = data->ObtenerVariable(this->tpe).ObtenerEntero();
		ate = tme;
		tme += ttpe;
		data->ObtenerVariable(this->nme) = tme;
		for (int i = ate; i < tme; i += 1)
		{
			data->CrearVariable(this->nme+std::string("#(")+eas(i)+std::string(")."));
			if(this->ptr)
				data->ObtenerVariable(this->nme+std::string("#(")+eas(i)+std::string(").")) = std::string(";")+this->nmv+std::string("#")+data->ObtenerVariable(this->tpe+std::string("#(")+eas(i-ate)+std::string(")."));
			else
				data->ObtenerVariable(this->nme+std::string("#(")+eas(i)+std::string(").")) = this->nmv+std::string("#")+data->ObtenerVariable(this->tpe+std::string("#(")+eas(i-ate)+std::string(")."));
		}
	}

	PseudoBorrarInteligente::PseudoBorrarInteligente(PDCadena var)
	{
		this->var = var;
		this->FijarClave("Borrar", "Tipos");
	}

	PseudoBorrarInteligente::~PseudoBorrarInteligente()
	{
		// nada
	}

	void PseudoBorrarInteligente::LeerParametros(std::istream& i)
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

	void PseudoBorrarInteligente::InscribirInstancia(PDvar::PDDatos* data)
	{
		// Primero se determina el tipo de variable y se busca, siempre se borra la
		// variable #NOMBRE. y #Tipo. sin importar su tipo, si no se encuentra
		// ignora, se usa PseudoBorrarVariable
		PDCadena tipo = data->ObtenerVariable(this->var + "#Tipo").ObtenerCadena();
		PDEntero index = data->BuscarIndice(
			true,
			this->var
		);
		PDEntero todelete = 1;
		PseudoBorrarVariable bv(this->var);
		bv.InscribirInstancia(data);

		PDCadena buff = "";
		std::istringstream iss(data->ObtenerVariable(tipo));
		while(iss >> buff)
		{
			bool ptr = false;
			if((buff.front() == ';')
				||(buff.front() == ':'))
			{
				ptr = true;
				buff.replace(0, 1, "");
			}
			PDCadena varname = this->var + "#" + buff;
			PseudoBorrarVariable a(varname);
			a.InscribirInstancia(data);
			if(!ptr)
				todelete++;
		}
		PDEntero size = data->nvapunt->size();
		for(PDEntero i = 0; i < size; i++)
		{
			int& pval = (*data->nvapunt)[i];
			if(pval >= index)
			{
				pval -= todelete;
			}
		}
	}
}

