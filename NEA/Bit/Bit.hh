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

#ifndef __PSEUDOD_NEA_BITS_BIT_HH__
#define __PSEUDOD_NEA_BITS_BIT_HH__ 1

namespace Bit
{
	class PseudoBitAnd : public PDTipos::PDInstancia
	{
		public:
			PseudoBitAnd(PDCadena a = "", PDCadena b = "", PDCadena r = "");
			virtual ~PseudoBitAnd(void);
			virtual void InscribirInstancia(PDvar::PDDatos* data);
			virtual void LeerParametros(std::istream& in);
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
			virtual void LeerParametros(std::istream& in);
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
			virtual void LeerParametros(std::istream& in);
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
			virtual void LeerParametros(std::istream& in);
		private:
			PDCadena res;
			PDCadena pa;
	};
}

#endif
