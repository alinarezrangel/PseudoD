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

#ifndef HDR_PSEUDOD_LOCALIZACION_HH
#define HDR_PSEUDOD_LOCALIZACION_HH 1

namespace Localizacion
{
	/*
	* Cambia la localización global a la especificada en el constructor.
	*/
	class PseudoFijarLocalizacion : public PDTipos::PDInstancia
	{
		public:
			PseudoFijarLocalizacion(PDCadena nloc = "C");
			virtual ~PseudoFijarLocalizacion(void);
			virtual void InscribirInstancia(PDvar::PDDatos* data);
			virtual void LeerParametros(pseudod::Tokenizador& in);
		private:
			PDCadena locale;
			bool var;
	};

	/*
	* Reestablece la localización global a la inicial "C".
	*/
	class PseudoReestablecerLocalizacion : public PDTipos::PDInstancia
	{
		public:
			PseudoReestablecerLocalizacion(void);
			virtual ~PseudoReestablecerLocalizacion(void);
			virtual void InscribirInstancia(PDvar::PDDatos* data);
			virtual void LeerParametros(pseudod::Tokenizador& in);
	};

	/*
	* Convierte un decimal de la localización "C" a la especificada
	*/
	class PseudoConvertirDecimal : public PDTipos::PDInstancia
	{
		public:
			PseudoConvertirDecimal(PDCadena nloc = "C", PDCadena var = "____");
			virtual ~PseudoConvertirDecimal(void);
			virtual void InscribirInstancia(PDvar::PDDatos* data);
			virtual void LeerParametros(pseudod::Tokenizador& in);
		private:
			PDCadena locale;
			bool var;
			PDCadena vard;
	};

	/*
	* Convierte un entero de la localización "C" a la especificada
	*/
	class PseudoConvertirEntero : public PDTipos::PDInstancia
	{
		public:
			PseudoConvertirEntero(PDCadena nloc = "C", PDCadena var = "____");
			virtual ~PseudoConvertirEntero(void);
			virtual void InscribirInstancia(PDvar::PDDatos* data);
			virtual void LeerParametros(pseudod::Tokenizador& in);
		private:
			PDCadena locale;
			bool var;
			PDCadena vard;
	};

	/*
	* Convierte un decimal de la localización especificada a la "C"
	*/
	class PseudoConvertirDecimalDesde : public PDTipos::PDInstancia
	{
		public:
			PseudoConvertirDecimalDesde(PDCadena nloc = "C", PDCadena var = "____");
			virtual ~PseudoConvertirDecimalDesde(void);
			virtual void InscribirInstancia(PDvar::PDDatos* data);
			virtual void LeerParametros(pseudod::Tokenizador& in);
		private:
			PDCadena locale;
			bool var;
			PDCadena vard;
	};

	/*
	* Convierte un entero de la localización especificada a la "C"
	*/
	class PseudoConvertirEnteroDesde : public PDTipos::PDInstancia
	{
		public:
			PseudoConvertirEnteroDesde(PDCadena nloc = "C", PDCadena var = "____");
			virtual ~PseudoConvertirEnteroDesde(void);
			virtual void InscribirInstancia(PDvar::PDDatos* data);
			virtual void LeerParametros(pseudod::Tokenizador& in);
		private:
			PDCadena locale;
			bool var;
			PDCadena vard;
	};

	/*
	* Determina el tipo del carácter en la localización especificada o la global.
	*/
	class PseudoEsCaracter : public PDTipos::PDInstancia
	{
		public:
			PseudoEsCaracter(
				PDCadena nloc = "C",
				PDCadena var = "____",
				PDCadena tp = "____"
			);
			virtual ~PseudoEsCaracter(void);
			virtual void InscribirInstancia(PDvar::PDDatos* data);
			virtual void LeerParametros(pseudod::Tokenizador& in);
		private:
			PDCadena locale;
			bool var;
			PDCadena vard;
			PDCadena varh;
	};
}

#endif
