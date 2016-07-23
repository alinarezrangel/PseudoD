/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*** Este es PseudoD version 2.0.0                                      *****
*** Log de este archivo:                                               *****
*** Formato: DD/MM/YYYY: txt                                           *****
*** **** 16/07/2016: Se creo el archivo.                               *****
****************************************************************************
**************************************************************************/

#ifndef __MANEJADOR_DE_BITS__
#define __MANEJADOR_DE_BITS__

namespace Bit
{
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

