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


namespace BibliotecaDinamica
{
	PseudoLlamar::PseudoLlamar(PDCadena var, std::vector<PDCadena> args)
	{
		this->var = var;
		this->param = args;
		this->FijarClave("Llamar", "BibliotecasDinamicas");
	}

	PseudoLlamar::~PseudoLlamar(void)
	{
		//~~~~
	}

	void PseudoLlamar::LeerParametros(std::istream& in)
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
		}
		PDCadena p = "";
		while((in >> p) && (p != "#(Final)."))
		{
			param.push_back(p);
		}
	}

	void PseudoLlamar::InscribirInstancia(PDvar::PDDatos* data)
	{
		void* con = dlopen(data->ObtenerVariable(this->var + "#lib").c_str(), RTLD_LAZY);
		if(!con)
		{
			throw PDvar::ErrorDelNucleo(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " inst args... FIN': Error de cargar por '"
				+ PDCadena(dlerror())
				+ "'"
			);
		}

		typedef void (*pdfun_t)(PDvar::PDDatos**, std::vector<PDCadena>);
		pdfun_t fun = NULL;
		*(void**) (&fun) = dlsym(con, data->ObtenerVariable(this->var + "#sim").c_str());

		if(!fun)
		{
			dlclose(con);
			throw PDvar::ErrorDelNucleo(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " inst args... FIN': Error de cargar por '"
				+ PDCadena(dlerror())
				+ "'"
			);
		}
		try
		{
			(*fun)(&data, this->param);
			dlclose(con);
		}
		catch(...)
		{
			dlclose(con);
			throw;
		}
	}

	PseudoLlamarOO::PseudoLlamarOO(PDCadena var, std::vector<PDCadena> args)
	{
		this->var = var;
		this->param = args;
		this->FijarClave("LlamarOO", "BibliotecasDinamicas");
	}

	PseudoLlamarOO::~PseudoLlamarOO(void)
	{
		//~~~~
	}

	void PseudoLlamarOO::LeerParametros(std::istream& in)
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
		PDCadena p = "";
		while((in >> p) && (p != "#(Final)."))
		{
			param.push_back(p);
		}
	}

	void PseudoLlamarOO::InscribirInstancia(PDvar::PDDatos* data)
	{
		void* con = dlopen(data->ObtenerVariable(this->var + "#lib").c_str(), RTLD_LAZY);
		if(!con)
		{
			throw PDvar::ErrorDelNucleo(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " inst args... FIN': Error de cargar por '"
				+ PDCadena(dlerror())
				+ "'"
			);
		}
		typedef PDvar::Din::ModuloDinamico* (*pdini_t)(void);
		typedef void (*pdend_t)(PDvar::Din::ModuloDinamico*);
		PDvar::Din::ModuloDinamico* mod;

		pdini_t obtener = NULL;
		pdend_t liberar = NULL;
		*(void**) (&obtener) = dlsym(con, "ObtenerModulo");
		*(void**) (&liberar) = dlsym(con, "LiberarModulo");

		if((obtener == NULL) || (liberar == NULL))
		{
			dlclose(con);
			throw PDvar::ErrorDelNucleo(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " inst args... FIN': Error de cargar por '"
				+ PDCadena(dlerror())
				+ "'"
			);
		}

		PDvar::Din::Argumentos args;
		args.Instancia = this->var;
		args.FuncionLlamada = data->ObtenerVariable(this->var + "#sim");
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
