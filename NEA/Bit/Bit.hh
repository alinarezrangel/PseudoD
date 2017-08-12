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

#ifndef HDR_PSEUDOD_NEA_BITS_BIT_HH
#define HDR_PSEUDOD_NEA_BITS_BIT_HH 1

namespace Bit
{
	class PseudoBitAnd : public PDTipos::PDInstancia
	{
		public:
			PseudoBitAnd(PDCadena a = "", PDCadena b = "", PDCadena r = "");
			virtual ~PseudoBitAnd(void);
			virtual void InscribirInstancia(PDvar::PDDatos* data);
			virtual void LeerParametros(pseudod::Tokenizador& in);
		private:
			PDCadena res;
			PDCadena pa;
			PDCadena pb;
	};
	class PseudoBitOr : public PDTipos::PDInstancia
	{
		public:
			PseudoBitOr(PDCadena a = "", PDCadena b = "", PDCadena r = "");
			virtual ~PseudoBitOr(void);
			virtual void InscribirInstancia(PDvar::PDDatos* data);
			virtual void LeerParametros(pseudod::Tokenizador& in);
		private:
			PDCadena res;
			PDCadena pa;
			PDCadena pb;
	};
	class PseudoBitXor : public PDTipos::PDInstancia
	{
		public:
			PseudoBitXor(PDCadena a = "", PDCadena b = "", PDCadena r = "");
			virtual ~PseudoBitXor(void);
			virtual void InscribirInstancia(PDvar::PDDatos* data);
			virtual void LeerParametros(pseudod::Tokenizador& in);
		private:
			PDCadena res;
			PDCadena pa;
			PDCadena pb;
	};
	class PseudoBitNot : public PDTipos::PDInstancia
	{
		public:
			PseudoBitNot(PDCadena a = "", PDCadena r = "");
			virtual ~PseudoBitNot(void);
			virtual void InscribirInstancia(PDvar::PDDatos* data);
			virtual void LeerParametros(pseudod::Tokenizador& in);
		private:
			PDCadena res;
			PDCadena pa;
	};
}

#endif
