/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*** Este es PseudoD version 2.0.0                                      *****
*** Log de este archivo:                                               *****
*** Formato: DD/MM/YYYY: txt                                           *****
*** **** 14/09/2014: Se creo el archivo.                               *****
****************************************************************************
**************************************************************************/


namespace BibliotecaDinamica
{
	PseudoLlamar::PseudoLlamar(string var, vector<string> args)
	{
		this->var = var;
		this->param = args;
		this->FijarClave("Llamar","BibliotecasDinamicas");
	}

	PseudoLlamar::~PseudoLlamar()
	{
		//~~~~
	}

	void PseudoLlamar::LeerParametros(istream& in)
	{
		if(!(in >> this->var))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " inst args... FIN': EOF inesperado"
			);
			return;
		}
		string p;
		while((in >> p)&&(p != "#(Final)."))
		{
			param.push_back(p);
		}
	}

	void PseudoLlamar::InscribirInstancia(PDDatos* data)
	{
		void* con = dlopen(data->ObtenerVariable(this->var + string("#lib")).c_str(),RTLD_LAZY);
		if(!con)
		{
			throw PDvar::ErrorDelNucleo(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " inst args... FIN': Error de cargar por '"
				+ string(dlerror())
				+ "'"
			);
		}
		typedef void(*pdfun_t)(PDDatos**,vector<string>);
		pdfun_t fun = (pdfun_t) dlsym(con,data->ObtenerVariable(this->var + string("#sim")).c_str());
		if(!fun)
		{
			dlclose(con);
			throw PDvar::ErrorDelNucleo(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " inst args... FIN': Error de cargar por '"
				+ string(dlerror())
				+ "'"
			);
		}
		try
		{
			(*fun)(&data,this->param);
			dlclose(con);
		}
		catch(...)
		{
			dlclose(con);
			throw;
		}
	}

	PseudoLlamarOO::PseudoLlamarOO(string var, vector<string> args)
	{
		this->var = var;
		this->param = args;
		this->FijarClave("LlamarOO","BibliotecasDinamicas");
	}

	PseudoLlamarOO::~PseudoLlamarOO()
	{
		//~~~~
	}

	void PseudoLlamarOO::LeerParametros(istream& in)
	{
		if(!(in >> this->var))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " inst args... FIN': EOF inesperado"
			);
			return;
		}
		string p;
		while((in >> p)&&(p != "#(Final)."))
		{
			param.push_back(p);
		}
	}

	void PseudoLlamarOO::InscribirInstancia(PDDatos* data)
	{
		void* con = dlopen(data->ObtenerVariable(this->var + string("#lib")).c_str(),RTLD_LAZY);
		if(!con)
		{
			throw PDvar::ErrorDelNucleo(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " inst args... FIN': Error de cargar por '"
				+ string(dlerror())
				+ "'"
			);
		}
		typedef PDvar::Din::ModuloDinamico* (*pdini_t)(void);
		typedef void (*pdend_t)(PDvar::Din::ModuloDinamico*);
		PDvar::Din::ModuloDinamico* mod;
		pdini_t obtener = (pdini_t) dlsym(con,"ObtenerModulo");
		pdend_t liberar = (pdend_t) dlsym(con,"LiberarModulo");
		if((obtener == NULL)||(liberar == NULL))
		{
			dlclose(con);
			throw PDvar::ErrorDelNucleo(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " inst args... FIN': Error de cargar por '"
				+ string(dlerror())
				+ "'"
			);
		}
		PDvar::Din::Argumentos args;
		args.Instancia = this->var;
		args.FuncionLlamada = data->ObtenerVariable(this->var + string("#sim"));
		args.Argumentos = &this->param;
		args.Manejador = &data;
		bool liberar_antes = false;
		try
		{
			mod = (*obtener)();
			liberar_antes = mod->ManejarFuncion(&args);
			(*liberar)(mod);
			dlclose(con);
		}
		catch(...)
		{
			if(liberar_antes)
				(*liberar)(mod);
			dlclose(con);
			throw;
		}
	}
}
