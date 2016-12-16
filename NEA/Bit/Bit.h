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

#ifndef __MANEJADOR_DE_BITS__
#define __MANEJADOR_DE_BITS__

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

