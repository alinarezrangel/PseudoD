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

namespace Bit
{
	PseudoBitAnd::PseudoBitAnd(std::string a, std::string b, std::string r) : PDTipos::PDInstancia()
	{
		this->res = r;
		this->pa = a;
		this->pb = b;
		this->FijarClave(string("Conjuncion"),string("Bit"));
	}
	PseudoBitAnd::~PseudoBitAnd(void)
	{}
	void PseudoBitAnd::InscribirInstancia(PDvar::PDDatos* data)
	{
		long long int a = 0, b = 0;
		a = cae(data->ObtenerVariable(this->pa));
		b = cae(data->ObtenerVariable(this->pb));
		data->ObtenerVariable(this->res) = eas(a & b);
	}
	void PseudoBitAnd::LeerParametros(std::istream& in)
	{
		if(!(in >> this->pa >> this->pb >> this->res))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " a b res': EOF inesperado"
			);
		}
	}
	PseudoBitOr::PseudoBitOr(std::string a, std::string b, std::string r) : PDTipos::PDInstancia()
	{
		this->res = r;
		this->pa = a;
		this->pb = b;
		this->FijarClave(string("DisyuncionInclusiva"),string("Bit"));
	}
	PseudoBitOr::~PseudoBitOr(void)
	{}
	void PseudoBitOr::InscribirInstancia(PDvar::PDDatos* data)
	{
		long long int a = 0, b = 0;
		a = cae(data->ObtenerVariable(this->pa));
		b = cae(data->ObtenerVariable(this->pb));
		data->ObtenerVariable(this->res) = eas(a | b);
	}
	void PseudoBitOr::LeerParametros(std::istream& in)
	{
		if(!(in >> this->pa >> this->pb >> this->res))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " a b res': EOF inesperado"
			);
		}
	}
	PseudoBitXor::PseudoBitXor(std::string a, std::string b, std::string r) : PDTipos::PDInstancia()
	{
		this->res = r;
		this->pa = a;
		this->pb = b;
		this->FijarClave(string("DisyuncionExclusiva"),string("Bit"));
	}
	PseudoBitXor::~PseudoBitXor(void)
	{}
	void PseudoBitXor::InscribirInstancia(PDvar::PDDatos* data)
	{
		long long int a = 0, b = 0;
		a = cae(data->ObtenerVariable(this->pa));
		b = cae(data->ObtenerVariable(this->pb));
		data->ObtenerVariable(this->res) = eas(a ^ b);
	}
	void PseudoBitXor::LeerParametros(std::istream& in)
	{
		if(!(in >> this->pa >> this->pb >> this->res))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " a b res': EOF inesperado"
			);
		}
	}
	PseudoBitNot::PseudoBitNot(std::string a, std::string r) : PDTipos::PDInstancia()
	{
		this->res = r;
		this->pa = a;
		this->FijarClave(string("Negacion"),string("Bit"));
	}
	PseudoBitNot::~PseudoBitNot(void)
	{}
	void PseudoBitNot::InscribirInstancia(PDvar::PDDatos* data)
	{
		long long int a = 0;
		a = cae(data->ObtenerVariable(this->pa));
		data->ObtenerVariable(this->res) = eas(~a);
	}
	void PseudoBitNot::LeerParametros(std::istream& in)
	{
		if(!(in >> this->pa >> this->res))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " a b res': EOF inesperado"
			);
		}
	}
}
