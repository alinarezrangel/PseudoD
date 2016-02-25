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
			cerr << "Error en " << this->ObtenerClave() << ", no se pudo leer bien el fichero de entrada." << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " EOF inesperado");
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
			cerr << "Error en:" << endl << this->ObtenerClave() << " " << this->var;
			for (int i = 0; i < this->param.size(); i += 1)
			{
				cerr << " " << this->param[i];
			}
			cerr << endl;
			cerr << "No se pudo cargar la biblioteca de carga dinamica por el error:" << dlerror() << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " Error en la carga dinamica");
			return;
		}
		typedef void(*pdfun_t)(PDDatos**,vector<string>);
		pdfun_t fun = (pdfun_t) dlsym(con,data->ObtenerVariable(this->var + string("#sim")).c_str());
		if(!fun)
		{
			cerr << "Error en:" << endl << this->ObtenerClave() << " " << this->var;
			for (int i = 0; i < this->param.size(); i += 1)
			{
				cerr << " " << this->param[i];
			}
			cerr << endl;
			cerr << "No se pudo cargar el simbolo de la biblioteca de carga dinamica por el error:" << dlerror() << endl;
			throw string("Error en el la parte " + this->ObtenerClave() + " Error en la carga dinamica");
			dlclose(con);
			return;
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
}
