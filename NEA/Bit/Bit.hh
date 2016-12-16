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
	using namespace std;

	class PseudoBitAnd : public PDTipos::PDInstancia
	{
		public:
			PseudoBitAnd(std::string a = "", std::string b = "", std::string r = "");
			virtual ~PseudoBitAnd(void);
			virtual void InscribirInstancia(PDvar::PDDatos* data);
			virtual void LeerParametros(std::istream& in);
		private:
			std::string res;
			std::string pa;
			std::string pb;
	};
	class PseudoBitOr : public PDTipos::PDInstancia
	{
		public:
			PseudoBitOr(std::string a = "", std::string b = "", std::string r = "");
			virtual ~PseudoBitOr(void);
			virtual void InscribirInstancia(PDvar::PDDatos* data);
			virtual void LeerParametros(std::istream& in);
		private:
			std::string res;
			std::string pa;
			std::string pb;
	};
	class PseudoBitXor : public PDTipos::PDInstancia
	{
		public:
			PseudoBitXor(std::string a = "", std::string b = "", std::string r = "");
			virtual ~PseudoBitXor(void);
			virtual void InscribirInstancia(PDvar::PDDatos* data);
			virtual void LeerParametros(std::istream& in);
		private:
			std::string res;
			std::string pa;
			std::string pb;
	};
	class PseudoBitNot : public PDTipos::PDInstancia
	{
		public:
			PseudoBitNot(std::string a = "", std::string r = "");
			virtual ~PseudoBitNot(void);
			virtual void InscribirInstancia(PDvar::PDDatos* data);
			virtual void LeerParametros(std::istream& in);
		private:
			std::string res;
			std::string pa;
	};
}

#endif
