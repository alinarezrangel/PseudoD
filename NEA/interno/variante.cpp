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

#include "variante.hh"

namespace PDvar
{
	Variante::Variante(void) : tipo(Variante::SinTipo), valor(nullptr)
	{
	}
	Variante::Variante(const tipo_cadena& cad) : tipo(Variante::TipoCadena)
	{
		this->valor = new Variante::tipo_cadena(cad);
	}
	Variante::Variante(const tipo_entero& ent) : tipo(Variante::TipoEntero)
	{
		this->valor = new Variante::tipo_entero(ent);
	}
	Variante::Variante(const tipo_real& rea) : tipo(Variante::TipoReal)
	{
		this->valor = new Variante::tipo_real(rea);
	}
	Variante::Variante(const Variante& var) : tipo(var.tipo)
	{
		this->Fijar(var);
	}
	Variante::Variante(const Variante::Discriminante& dc) : tipo(dc)
	{
		this->FijarTipo(dc);
	}
	Variante::Variante(Variante&& var) : tipo(var.tipo), valor(var.valor)
	{
		var.valor = nullptr;
		var.tipo = Variante::SinTipo;
	}
	Variante::~Variante(void)
	{
		if(this->valor != nullptr)
		{
			this->DestruirMemoria();
		}
	}

	Variante::operator Variante::tipo_cadena(void) const
	{
		return this->ObtenerCadena();
	}
	Variante::operator Variante::tipo_entero(void) const
	{
		return this->ObtenerEntero();
	}
	Variante::operator Variante::tipo_real(void) const
	{
		return this->ObtenerReal();
	}
	Variante::operator Variante::Discriminante(void) const
	{
		return this->ObtenerTipo();
	}
	Variante::operator bool(void) const
	{
		return this->valor != nullptr;
	}

	Variante& Variante::operator=(const Variante& var)
	{
		if(&var == this)
			return *this;
		this->Fijar(var);
		return *this;
	}
	Variante& Variante::operator=(const Variante::tipo_cadena& cad)
	{
		this->FijarCadena(cad);
		return *this;
	}
	Variante& Variante::operator=(const Variante::tipo_entero& ent)
	{
		this->FijarEntero(ent);
		return *this;
	}
	Variante& Variante::operator=(const Variante::tipo_real& rea)
	{
		this->FijarReal(rea);
		return *this;
	}
	Variante& Variante::operator=(const Variante::Discriminante& dc)
	{
		this->FijarTipo(dc);
		return *this;
	}

	// NOTA: Estas funciones comparan valor, NO IDENTIDAD
	bool Variante::operator==(const Variante& var) const
	{
		return this->EsIgualA(var);
	}
	bool Variante::operator==(const Variante::tipo_cadena& cad) const
	{
		return this->EsIgualA(cad);
	}
	bool Variante::operator==(const Variante::tipo_entero& ent) const
	{
		return this->EsIgualA(ent);
	}
	bool Variante::operator==(const Variante::tipo_real& rea) const
	{
		return this->EsIgualA(rea);
	}
	bool Variante::operator==(const Variante::Discriminante& dc) const
	{
		return this->EsIgualA(dc);
	}

	// NOTA: Estas funciones comparan valor, NO IDENTIDAD
	bool Variante::operator!=(const Variante& var) const
	{
		return !(*this == var);
	}
	bool Variante::operator!=(const Variante::tipo_cadena& cad) const
	{
		return !(*this == cad);
	}
	bool Variante::operator!=(const Variante::tipo_entero& ent) const
	{
		return !(*this == ent);
	}
	bool Variante::operator!=(const Variante::tipo_real& rea) const
	{
		return !(*this == rea);
	}
	bool Variante::operator!=(const Variante::Discriminante& dc) const
	{
		return !(*this == dc);
	}

	// NOTA: Estas funciones comparan valor, NO IDENTIDAD
	bool Variante::EsIgualA(const Variante& var) const
	{
		if(var.tipo != this->tipo)
			return false;
		switch(this->tipo)
		{
			case Variante::TipoCadena:
				return this->ObtenerCadena() == var.ObtenerCadena();
			case Variante::TipoEntero:
				return this->ObtenerEntero() == var.ObtenerEntero();
			case Variante::TipoReal:
				return this->ObtenerReal() == var.ObtenerReal();
			case Variante::SinTipo:
				return true;
			default:
				throw ErrorDeSemantica("Error en Variante::EsIgualA: tipo no reconocido");
		}
	}
	bool Variante::EsIgualA(const Variante::tipo_cadena& cad) const
	{
		if(this->tipo != Variante::TipoCadena)
			return false;
		return this->ObtenerCadena() == cad;
	}
	bool Variante::EsIgualA(const Variante::tipo_entero& ent) const
	{
		if(this->tipo != Variante::TipoEntero)
			return false;
		return this->ObtenerEntero() == ent;
	}
	bool Variante::EsIgualA(const Variante::tipo_real& rea) const
	{
		if(this->tipo != Variante::TipoReal)
			return false;
		return this->ObtenerReal() == rea;
	}
	bool Variante::EsIgualA(const Variante::Discriminante& dc) const
	{
		return this->tipo == dc;
	}

	const Variante::Discriminante& Variante::ObtenerTipo(void) const
	{
		return this->tipo;
	}
	const Variante::tipo_cadena& Variante::ObtenerCadena(void) const
	{
		if(this->tipo != Variante::TipoCadena)
			throw ErrorDeSemantica("Error en Variante::Obtener*: tipo invalido");
		return *static_cast<const Variante::tipo_cadena*>(this->valor);
	}
	const Variante::tipo_entero& Variante::ObtenerEntero(void) const
	{
		if(this->tipo != Variante::TipoEntero)
			throw ErrorDeSemantica("Error en Variante::Obtener*: tipo invalido");
		return *static_cast<const Variante::tipo_entero*>(this->valor);
	}
	const Variante::tipo_real& Variante::ObtenerReal(void) const
	{
		if(this->tipo != Variante::TipoReal)
			throw ErrorDeSemantica("Error en Variante::Obtener*: tipo invalido");
		return *static_cast<const Variante::tipo_real*>(this->valor);
	}

	bool Variante::PoseeTipo(void) const
	{
		return this->tipo != Variante::SinTipo;
	}

	void Variante::FijarTipo(const Variante::Discriminante& dc)
	{
		if(this->tipo == dc)
			return;
		if(this->valor != nullptr)
			this->DestruirMemoria();
		this->tipo = dc;
		switch(dc)
		{
			case Variante::TipoCadena:
				this->valor = new Variante::tipo_cadena;
				break;
			case Variante::TipoEntero:
				this->valor = new Variante::tipo_entero;
				break;
			case Variante::TipoReal:
				this->valor = new Variante::tipo_real;
				break;
			case Variante::SinTipo:
				this->valor = nullptr;
				break;
			default:
				throw ErrorDeSemantica("Error en Variante::FijarTipo: tipo no reconocido");
		}
	}
	void Variante::FijarCadena(const Variante::tipo_cadena& cad)
	{
		if(this->valor != nullptr)
			this->DestruirMemoria();
		this->tipo = Variante::TipoCadena;
		this->valor = new Variante::tipo_cadena(cad);
	}
	void Variante::FijarEntero(const Variante::tipo_entero& ent)
	{
		if(this->valor != nullptr)
			this->DestruirMemoria();
		this->tipo = Variante::TipoEntero;
		this->valor = new Variante::tipo_entero(ent);
	}
	void Variante::FijarReal(const Variante::tipo_real& rea)
	{
		if(this->valor != nullptr)
			this->DestruirMemoria();
		this->tipo = Variante::TipoReal;
		this->valor = new Variante::tipo_real(rea);
	}
	void Variante::Fijar(const Variante& var)
	{
		if(this->valor != nullptr)
			this->DestruirMemoria();
		this->tipo = var.tipo;
		switch(var.tipo)
		{
			case Variante::TipoCadena:
				this->valor = new Variante::tipo_cadena(var.ObtenerCadena());
				break;
			case Variante::TipoEntero:
				this->valor = new Variante::tipo_entero(var.ObtenerEntero());
				break;
			case Variante::TipoReal:
				this->valor = new Variante::tipo_real(var.ObtenerReal());
				break;
			case Variante::SinTipo:
				this->valor = nullptr;
				break;
			default:
				throw ErrorDeSemantica("Error en Variante::Fijar: tipo no reconocido");
		}
	}

	// NOTA: Estas funciones comparar identidad, NO VALOR
	bool Variante::EsMismo(const Variante& var) const
	{
		return this == &var;
	}
	bool Variante::EsMismo(const Variante::tipo_cadena& cad) const
	{
		return static_cast<Variante::tipo_cadena*>(this->valor) == &cad;
	}
	bool Variante::EsMismo(const Variante::tipo_entero& ent) const
	{
		return static_cast<Variante::tipo_entero*>(this->valor) == &ent;
	}
	bool Variante::EsMismo(const Variante::tipo_real& rea) const
	{
		return static_cast<Variante::tipo_real*>(this->valor) == &rea;
	}

	void*& Variante::ObtenerPuntero(void)
	{
		return this->valor;
	}
	Variante::Discriminante& Variante::ObtenerDiscriminante(void)
	{
		return this->tipo;
	}
	void Variante::DestruirMemoria(void)
	{
		switch(this->tipo)
		{
			case Variante::TipoCadena:
				delete static_cast<Variante::tipo_cadena*>(this->valor);
				break;
			case Variante::TipoEntero:
				delete static_cast<Variante::tipo_entero*>(this->valor);
				break;
			case Variante::TipoReal:
				delete static_cast<Variante::tipo_real*>(this->valor);
				break;
			case Variante::SinTipo:
				// SinTipo no requiere destrucción
				break;
			default:
				throw ErrorDeSemantica("Error en Variante::DestruirMemoria: tipo no reconocido");
		}
	}
}
