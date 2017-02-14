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


namespace PDTipos
{
	PDOrden::PDOrden(void) : PDvar::PDObjeto()
	{
		this->clave = "NO FIJADA";
	}

	PDOrden::~PDOrden(void)
	{
		// nada
	}

	PDCadena PDOrden::ObtenerClave(void)
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

	PseudoArray::PseudoArray(PDCadena nom, int can) : PDInstancia()
	{
		this->cant = can;
		this->nm = nom;
		this->FijarClave("Array", "Tipos");
	}

	PseudoArray::~PseudoArray(void)
	{
		//~~~~~~nada
	}

	void PseudoArray::InscribirInstancia(PDvar::PDDatos* data)
	{
		data->CrearVariable(this->nm + "#longitud.");
		PDCadena& r = data->ObtenerVariable(this->nm + "#longitud.");
		r = eas(this->cant);
		for(int i = 0; i < this->cant; i++)
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

	PseudoClase::PseudoClase(PDCadena nm, PDCadena base, std::vector<PDCadena> mt)
		: PDInstancia()
	{
		this->nm = nm;
		this->base = base;
		this->methods = mt;
		this->FijarClave("Estructura", "Tipos");
	}

	PseudoClase::~PseudoClase(void)
	{
		//NADA
	}

	void PseudoClase::InscribirInstancia(PDvar::PDDatos* data)
	{
		data->CrearVariable(this->nm);
		data->ObtenerVariable(this->nm) = "";
		for(decltype(this->methods)::size_type i = 0; i < this->methods.size(); i++)
		{
			data->ObtenerVariable(this->nm) += this->methods[i] + " ";
		}
		data->CrearVariable(this->nm + "#Tipo");
		data->ObtenerVariable(this->nm + "#Tipo") = this->nm;
		if(this->base != "")
		{
			PseudoHerencia hb(this->base, this->nm);
			hb.InscribirInstancia(data);
		}
	}

	void PseudoClase::LeerParametros(std::istream& in)
	{
		PDCadena b = "";
		std::function<PDCadena (std::istream&)> readStr = [&](std::istream& input) -> PDCadena
		{
			PDCadena w = "";
			if(!(input >> w))
			{
				throw PDvar::ErrorDeSintaxis(
					"Error en "
					+ this->ObtenerClave()
					+ ": '"
					+ this->ObtenerClave()
					+ " nm ... FIN' alias 'clase nm ... FIN': EOF inesperado"
				);
			}
			return w;
		};
		this->nm = readStr(in);
		if(this->nm == "abstracta")
		{
			this->nm = readStr(in);
		}
		b = readStr(in);
		while((b != "#(Final).") && (b != "finclase"))
		{
			if(b.front() == '[')
			{
				if(b.back() != ']')
					std::getline(in, b, ']');
				b = readStr(in);
				continue;
			}
			if((b == "hereda") || (b == "heredar") || (b == "extiende") || (b == "implementa"))
			{
				b = readStr(in);
				if(b != "implementa") // PseudoD aún no soporta implementaciones sin herencia
					this->base = b;
				b = readStr(in);
				continue;
			}
			if(b == "atributo")
			{
				b = readStr(in);
			}
			else if(b == "puntero")
			{
				b = ";" + readStr(in);
			}
			else if(b == "metodo")
			{
				b = ":" + readStr(in);
			}
			this->methods.push_back(b);
			b = readStr(in);
		}
	}

	PseudoReferenciaClase::PseudoReferenciaClase(PDCadena n, PDCadena i)
		: PDInstancia()
	{
		this->nm = n;
		this->ni = i;
		this->FijarClave("Instancia", "Tipos");
	}

	PseudoReferenciaClase::~PseudoReferenciaClase(void)
	{
		//NADA
	}

	void PseudoReferenciaClase::InscribirInstancia(PDvar::PDDatos* data)
	{
		this->ni = data->ResolverNombre(this->ni);
		this->nm = data->ResolverNombre(this->nm);

		data->CrearVariable(this->ni);
		data->ObtenerVariable(this->ni) = this->ni;

		PDCadena buff = "";
		PDCadena& nms = data->ObtenerVariable(this->nm);
		for(PDCadena::size_type i = 0; i < nms.size(); i++)
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

		for(decltype(this->methods)::size_type i = 0; i < this->methods.size(); i++)
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
				data->CrearVariable(
					this->ni + "#" + this->methods[i] + "#cod",
					false,
					0
				);
				// No tiene sentido tener una copia de cada metodo de la instancia,
				// En cambio, puedes tener un puntero al metodo, que ocupa menos
				// espacio.
				data->ObtenerIndicePuntero(this->ni + "#" + this->methods[i])
					= data->BuscarIndice(true, this->nm + "#"
							+ this->methods[i]);
				data->ObtenerIndicePuntero(this->ni + "#" + this->methods[i] + "#cod")
					= data->BuscarIndice(true, this->nm + "#"
							+ this->methods[i]);
			}
			else
			{
				data->CrearVariable(this->ni + "#" + this->methods[i]);
			}
		}
		data->ObtenerVariable(this->ni + "#NOMBRE") = this->ni;
		data->ObtenerVariable(this->ni + "#Tipo") = this->nm;

		if((data->ExisteVariable(true, this->ni + "#crear"))
			|| (data->ExisteVariable(false, this->ni + "#crear")))
		{
			data->Ejecutar("llamar " + this->ni + "#crear finargs");
		}
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
		std::cout
			<< "@ Al salir con la orden salir se seguira ejecutando el programa"
			<< std::endl;
		std::cout << "@ Puedes ver la ayuda con \'ayuda\'" << std::endl;
		PDCadena i = "";
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
					PDCadena val = data->ObtenerVariable(var);
					std::cout << "La variable (o puntero) " << var
						<< " posee el valor \"" << val << "\"" << std::endl;
				}
				else if(i == "puntero")
				{
					PDCadena ptr = "", vptr = "", nvar = "";
					int iptr;
					std::cin >> ptr;
					vptr = data->ObtenerVariable(ptr);
					iptr = data->ObtenerIndicePuntero(ptr);
					if(iptr != -1)
						nvar = (*data->nombrev)[iptr];
					std::cout << "El puntero " << ptr << " posee el valor (apuntado) \""
						<< vptr << "\"" << std::endl
						<< " y apunta a la direccion [" << iptr
						<< "] del campo de variables." << std::endl
						<< "La variable apuntada es {" << nvar << "}" << std::endl;
				}
				else if(i == "pila")
				{
					int pil;
					std::cin >> pil;
					if((pil < 0) || (pil > (*data->pilas).size()))
					{
						std::cout << "NO EXISTE" << std::endl;
					}
					auto buffer = (*data->pilas)[pil];
					std::cout << "+----------PILA " << pil << "--------+" << std::endl;
					while(!buffer.empty())
					{
						std::cout
							<< "|"
							<< std::setw(24)
							<< std::setfill(' ')
							<< std::right
							<< buffer.top()
							<< "|"
							<< std::endl;
						buffer.pop();
					}
				}
				else if(i == "numero-de")
				{
					PDCadena que = "";
					std::cin >> que;
					if(que == "variables")
					{
						std::cout << "Hay " << (*data->nombrev).size() << " variables"
							<< std::endl;
					}
					if(que == "punteros")
					{
						std::cout << "Hay " << (*data->nombrep).size() << " punteros"
							<< std::endl;
					}
					if(que == "pilas")
					{
						std::cout << "Hay " << (*data->pilas).size() << " pilas"
							<< std::endl;
					}
				}
				else if(i == "ejecutar")
				{
					PDCadena ord = "";
					std::getline(std::cin, ord, '\n');
					data->Ejecutar(ord);
				}
				else if(i == "instancia")
				{
					std::cout
						<< "Advertencia: la instancia debe poseer los atributos"
						<< "fundamentales..." << std::endl;
					PDCadena var = "", est = "";
					std::cin >> var;
					est = data->ObtenerVariable(var + "#Tipo");
					std::cout << "La instancia del tipo " << est << " nombrada "
						<< var << " tiene los campos:" << std::endl;
					std::cout << "    [VALOR BRUTO]  =  "
						<< data->ObtenerVariable(var) << std::endl;
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
						PDCadena cnt = data->ObtenerVariable(var + "#" + est);
						if(cnt.size() > 80)
						{
							igcnt = true; // Contenido muy largo, ver con <variable a#b>
						}
						std::cout << est;
						if(!igcnt)
						{
							std::cout << " = " << cnt;
						}
						std::cout << std::endl;
					}
				}
				else if(i == "clase")
				{
					PDCadena est = "";
					std::cin >> est;
					std::cout << "La clase " << est << " tiene los siguientes campos:"
						<< std::endl;
					std::istringstream iss(data->ObtenerVariable(est));
					while(iss >> est)
					{
						std::cout << " " << est << std::endl;
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
					std::cout << " numero-de [q]: ver la cantidad de [q], donde [q] puede"
						<< std::endl;
					std::cout << "                ser (variables|punteros|pilas)"
						<< std::endl;
					std::cout
						<< " instancia [i]: ver la instancia [i](tipo auto-detectado)"
						<< std::endl;
					std::cout << " clase [c]: ver la clase [c]" << std::endl;
					std::cout << " salir: sale del depurador" << std::endl;
					std::cout << " ayuda: muestra esta ayuda" << std::endl;
					std::cout
						<< " volcar [q]: volcar todas los [q] de memoria,"
						<< "donde [q] puede ser" << std::endl;
					std::cout << "             (variables|punteros|pilas)" << std::endl;
					std::cout << " ejecutar [orden]: ejecuta el resto de la"
						<< "línea como código en PseudoD" << std::endl;
					std::cout << "Ejecución automatica:" << std::endl;
					std::cout
						<< " Si el depurador no comprende alguna de las ordenes, por"
						<< std::endl;
					std::cout << " defecto, ejecutará la linea con PseudoD." << std::endl;
					std::cout << std::endl;
					std::cout << " Si, por ejemplo: deseas llamar a una función,"
						<< std::endl;
					std::cout << " solo escribes: \'llamar [fnc]... finargs\'"
						<< std::endl;
				}
				else if(i != "salir")
				{
					data->Ejecutar(i, std::cin);
				}
			}
			catch(const PDvar::Error& e)
			{
				std::cerr << "PseudoD lanzo un error fatal: " << e.Mensaje() << std::endl;
				std::cerr << "EN " << data->ObtenerVariable("__ARCH__") << std::endl;
				std::cerr << "Captado por el debugger" << std::endl;
			}
			catch(const std::exception& e)
			{
				std::cerr << "PseudoD lanzo un error fatal: " << e.what() << std::endl;
				std::cerr << "EN " << data->ObtenerVariable("__ARCH__") << std::endl;
				std::cerr << "Captado por el debugger" << std::endl;
			}
		}
	}

	PseudoArrayEstructura::PseudoArrayEstructura(PDCadena na, PDCadena ne, int ta)
		: PDInstancia()
	{
		this->nme = ne;
		this->nma = na;
		this->tma = ta;
		this->FijarClave("EstrucArray", "Tipos");
	}

	PseudoArrayEstructura::~PseudoArrayEstructura(void)
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
		int tme = cae(data->ObtenerVariable(this->nme));
		int tmt = tme + this->tma + 1;// El 1 es de estruc#array#longitud
		data->ObtenerVariable(this->nme) = eas(tmt + 1);
		int ind = 0;
		for(int i = tme; i < tmt; i++)
		{
			data->CrearVariable(this->nme + "#(" + eas(i) + ").");
			data->ObtenerVariable(this->nme + "#(" + eas(i) + ").")
				= this->nma + "#(" + eas(ind) + ")";
			ind++;
		}
		data->CrearVariable(this->nme + "#(" + eas(tmt) + ").");
		data->ObtenerVariable(this->nme + "#(" + eas(tmt) + ").")
			= this->nma + "#longitud";
	}

	PseudoBorrarVariable::PseudoBorrarVariable(PDCadena n) : PDInstancia()
	{
		this->nm = n;
		this->FijarClave("BorrarSimple", "Tipos");
	}

	PseudoBorrarVariable::~PseudoBorrarVariable(void)
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

	PseudoHerencia::PseudoHerencia(PDCadena b, PDCadena h)
	{
		this->nmb = b;
		this->nmh = h;
		this->FijarClave("Heredar", "Tipos");
	}

	PseudoHerencia::~PseudoHerencia(void)
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
		PDCadena& base = data->ObtenerVariable(this->nmb);
		PDCadena& hija = data->ObtenerVariable(this->nmh);
		PDCadena curr_mtd = "";
		for(PDCadena::size_type i = 0; i < base.size(); i++)
		{
			if(base[i] == ' ')
			{
				if((curr_mtd.front() == ':')
					&& (hija.find(curr_mtd.c_str()) == PDCadena::npos))
				{
					// Crear una funcion llamada Hija#Metodo que sea igual a Base#Metodo
					curr_mtd.replace(0, 1, "");
					PDCadena hija_mtd = this->nmh + "#" + curr_mtd;
					PDCadena base_mtd = this->nmb + "#" + curr_mtd;
					data->CrearVariable(hija_mtd, true, 0);
					data->CrearVariable(hija_mtd + "#NOMBRE", true, 0);
					data->CrearVariable(hija_mtd + "#Tipo", true, 0);
					data->CrearVariable(hija_mtd + "#cod", false, 0);
					data->ObtenerVariable(hija_mtd)
						= data->ObtenerVariable(base_mtd);
					data->ObtenerVariable(hija_mtd + "#NOMBRE") = hija_mtd;
					data->ObtenerVariable(hija_mtd + "#Tipo") = "PseudoFuncion";
					data->ObtenerIndicePuntero(hija_mtd + "#cod")
						= data->BuscarIndice(true, hija_mtd);
				}
				curr_mtd = "";
				continue;
			}
			curr_mtd += base[i];
		}
		hija += base;
	}

	PseudoDireccionarPuntero::PseudoDireccionarPuntero(PDCadena p, PDCadena v)
		: PDInstancia()
	{
		this->nmp = p;
		this->nmv = v;
		this->FijarClave("Redireccionar", "Tipos");
	}

	PseudoDireccionarPuntero::~PseudoDireccionarPuntero(void)
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
		int oi = 0;
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

	PseudoMientras::PseudoMientras(PDCadena v, PDCadena o, PDCadena f)
		: PDInstancia()
	{
		this->nmv = v;
		this->func = f;
		this->orden = o;
		this->FijarClave("Mientras", "PseudoD");
	}
	PseudoMientras::~PseudoMientras(void)
	{
		// NADA
	}

	void PseudoMientras::LeerParametros(std::istream& in)
	{
		in >> this->nmv;
		std::getline(in,this->orden,'\n');
		std::vector<PDCadena> lineas;
		PDCadena lin = "";
		int mientras = 1;
		while(mientras > 0)
		{
			std::getline(in, lin, '\n');
			lineas.push_back(lin);
			lin.erase(std::remove_if(lin.begin(),
				lin.end(),
				[](char x){return std::isspace(x);}),
				lin.end());
			if((PDCadena(lin.substr(0, PDCadena("mientras").size())) == "mientras")
				|| (PDCadena(lin.substr(0, PDCadena("Importar.PseudoD.mientras").size()))
					== "Importar.PseudoD.mientras"))
			{
				mientras++;
			}
			if(lin == "finbucle")
			{
				mientras--;
			}
		}
		this->func = "";
		for(decltype(lineas)::size_type i = 0; i < lineas.size(); i++)
		{
			this->func += lineas[i] + "\n";
		}
	}

	void PseudoMientras::InscribirInstancia(PDvar::PDDatos* data)
	{
		std::istringstream sin(this->orden);
		PDCadena res = PDvar::ValorDelToken(this->nmv, sin, data);
		while(res == "verdadero")
		{
			PDCadena pal = "";
			std::istringstream sin2(this->func);
			while(sin2 >> pal)
			{
				data->Ejecutar(pal, sin2);
			}
			std::istringstream sin3(this->orden);
			res = PDvar::ValorDelToken(this->nmv, sin3, data);
		}
	}

	PseudoClaseContenida::PseudoClaseContenida(
		PDCadena es,
		PDCadena tp,
		PDCadena cm,
		bool pn)
		: PDInstancia()
	{
		this->nme = es; // estructura
		this->tpe = tp; // estructura del campo
		this->nmv = cm; // campo
		this->ptr = pn; // puntero?
		this->FijarClave("EstrucEstruc", "Tipos");
	}

	PseudoClaseContenida::~PseudoClaseContenida(void)
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
		int tme = 0, ttpe = 0, ate = 0;
		tme = cae(data->ObtenerVariable(this->nme));
		ttpe = cae(data->ObtenerVariable(this->tpe));
		ate = tme;
		tme += ttpe;
		data->ObtenerVariable(this->nme) = eas(tme);
		for (int i = ate; i < tme; i += 1)
		{
			data->CrearVariable(this->nme + "#(" + eas(i) + ").");
			if(this->ptr)
				data->ObtenerVariable(this->nme + "#(" + eas(i) + ").")
					= ";" + this->nmv + "#" + data->ObtenerVariable(
						this->tpe + "#(" + eas(i - ate) + ")."
					);
			else
				data->ObtenerVariable(this->nme + "#(" + eas(i) + ").")
					= this->nmv + "#" + data->ObtenerVariable(
						this->tpe + "#(" + eas(i - ate) + ")."
					);
		}
	}

	PseudoBorrarInteligente::PseudoBorrarInteligente(PDCadena var)
	{
		this->var = var;
		this->FijarClave("Borrar", "Tipos");
	}

	PseudoBorrarInteligente::~PseudoBorrarInteligente(void)
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
		// Llamamos al destructor (si existe)

		if((data->ExisteVariable(true, this->var + "#destruir"))
			|| (data->ExisteVariable(false, this->var + "#destruir")))
		{
			data->Ejecutar("llamar " + this->var + "#destruir finargs");
		}

		// Primero se determina el tipo de variable y se busca, siempre se borra la
		// variable #NOMBRE. y #Tipo. sin importar su tipo, si no se encuentra
		// ignora, se usa PseudoBorrarVariable
		PDCadena tipo = "";
		tipo = data->ObtenerVariable(this->var + "#Tipo");
		int index = data->BuscarIndice(
			true,
			this->var
		);
		int todelete = 1;
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
		int size = data->nvapunt->size();
		for(std::remove_pointer<decltype(data->nvapunt)>::type::size_type i = 0;
			i < size;
			i++)
		{
			int& pval = (*data->nvapunt)[i];
			if(pval >= index)
			{
				pval -= todelete;
			}
		}
	}
}
