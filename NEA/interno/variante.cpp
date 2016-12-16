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
	Variante::Variante(void) : tipo(Variante::SinTipo), destruido(true)
	{
		this->valor.cadena = nullptr;
		this->valor.entero = nullptr;
		this->valor.real = nullptr;
		// std::cout << "Creado VARIANTE por defecto en " << (long long int)this << std::endl;
	}
	Variante::Variante(const tipo_cadena& cad) : tipo(Variante::TipoCadena), destruido(false)
	{
		this->valor.entero = nullptr;
		this->valor.real = nullptr;
		// std::cout << "Creado variante desde cadena " << cad << " en " << (long long int)this << std::endl;
		this->valor.cadena = new Variante::tipo_cadena(cad);
	}
	Variante::Variante(const tipo_entero& ent) : tipo(Variante::TipoEntero), destruido(false)
	{
		this->valor.cadena = nullptr;
		this->valor.real = nullptr;
		// std::cout << "Creado variante desde entero " << ent << " en " << (long long int)this << std::endl;
		this->valor.entero = new Variante::tipo_entero(ent);
	}
	Variante::Variante(const tipo_real& rea) : tipo(Variante::TipoReal), destruido(false)
	{
		this->valor.cadena = nullptr;
		this->valor.entero = nullptr;
		// std::cout << "Creado variante desde real " << rea << " en " << (long long int)this << std::endl;
		this->valor.real = new Variante::tipo_real(rea);
	}
	Variante::Variante(const Variante& var) : tipo(Variante::SinTipo), destruido(true)
	{
		this->valor.cadena = nullptr;
		this->valor.entero = nullptr;
		this->valor.real = nullptr;
		// std::cout << "Creado variante desde variante en " << (long long int)this << std::endl;
		// std::cout << "# " << var.tipo << std::endl;
		this->Fijar(var);
	}
	Variante::Variante(const Variante::Discriminante& dc) : tipo(Variante::SinTipo), destruido(true)
	{
		this->valor.cadena = nullptr;
		this->valor.entero = nullptr;
		this->valor.real = nullptr;
		// std::cout << "Creado variante desde descriminante " << dc << " en " << (long long int)this << std::endl;
		this->FijarTipo(dc);
	}
	Variante::Variante(Variante&& var) : tipo(Variante::SinTipo), destruido(true)
	{
		this->valor.cadena = nullptr;
		this->valor.entero = nullptr;
		this->valor.real = nullptr;
		this->Fijar(var);
		var.DestruirMemoria();
		// std::cout << "Creado variante desde rvalueReferenceVariante en " << (long long int)this << std::endl;
		// std::cout << "!1Con tipo y valor: " << this->tipo << " " << this->ObtenerCadena() << std::endl;
		// std::cout << "!2Con tipo y valor: " << var.tipo << " " << var.ObtenerCadena() << std::endl;
	}
	Variante::Variante(const char* cad) : tipo(Variante::TipoCadena), destruido(false)
	{
		this->valor.entero = nullptr;
		this->valor.real = nullptr;
		// std::cout << "Creado variante desde cstrcadena " << cad << " en " << (long long int)this << std::endl;
		this->valor.cadena = new Variante::tipo_cadena(cad);
	}
	Variante::~Variante(void)
	{
		// std::cout << "Destruido variante en " << (long long int)this << std::endl;
		this->DestruirMemoria();
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
		return this->PoseeTipo();
	}

	Variante::operator Variante::tipo_cadena&(void)
	{
		return this->ObtenerCadena();
	}
	Variante::operator Variante::tipo_entero&(void)
	{
		return this->ObtenerEntero();
	}
	Variante::operator Variante::tipo_real&(void)
	{
		return this->ObtenerReal();
	}
	Variante::operator Variante::Discriminante&(void)
	{
		return this->ObtenerTipo();
	}

	Variante& Variante::operator=(const Variante& var)
	{
		if(&var == this)
			return *this;
		this->Fijar(var);
		return *this;
	}
	Variante& Variante::operator=(Variante&& var)
	{
		this->Fijar(var);
		var.DestruirMemoria();
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
			throw PDvar::ErrorDeSemantica("Error en Variante::ObtenerC*: Tipo no convertible");
		return *this->valor.cadena;
	}
	const Variante::tipo_entero& Variante::ObtenerEntero(void) const
	{
		if(this->tipo != Variante::TipoEntero)
			throw PDvar::ErrorDeSemantica("Error en Variante::ObtenerE*: Tipo no convertible");
		return *this->valor.entero;
	}
	const Variante::tipo_real& Variante::ObtenerReal(void) const
	{
		if(this->tipo != Variante::TipoReal)
			throw PDvar::ErrorDeSemantica("Error en Variante::ObtenerR*: Tipo no convertible");
		return *this->valor.real;
	}

	Variante::Discriminante& Variante::ObtenerTipo(void)
	{
		return this->tipo;
	}
	Variante::tipo_cadena& Variante::ObtenerCadena(void)
	{
		if(this->tipo != Variante::TipoCadena)
			this->Fijar(this->ConvertirA(Variante::TipoCadena));
		return *this->valor.cadena;
	}
	Variante::tipo_entero& Variante::ObtenerEntero(void)
	{
		if(this->tipo != Variante::TipoEntero)
			this->Fijar(this->ConvertirA(Variante::TipoEntero));
		return *this->valor.entero;
	}
	Variante::tipo_real& Variante::ObtenerReal(void)
	{
		if(this->tipo != Variante::TipoReal)
			this->Fijar(this->ConvertirA(Variante::TipoReal));
		return *this->valor.real;
	}

	Variante Variante::ConvertirA(const Variante::Discriminante& dc) const
	{
		Variante rt;
		rt.FijarTipo(dc);

		if(dc == Variante::SinTipo)
			return rt;

		if(this->tipo == Variante::SinTipo)
		{
			rt.FijarTipo(Variante::SinTipo);
			return rt;
		}

		if(dc == this->tipo)
		{
			rt = *this;
			return rt;
		}

		switch(this->tipo)
		{
			case Variante::TipoCadena:
				switch(dc)
				{
					case Variante::TipoEntero:
						rt = cae(this->ObtenerCadena());
						break;
					case Variante::TipoReal:
						rt = caf(this->ObtenerCadena());
						break;
					default:
						throw ErrorDeSemantica("Error en Variante::ConvertirA: Conversión inválida");
				}
				break;
			case Variante::TipoEntero:
				switch(dc)
				{
					case Variante::TipoCadena:
						rt = eas(this->ObtenerEntero());
						break;
					case Variante::TipoReal:
						// Como tipo_real generalmente es un <long double>, podemos
						// realizar una conversion explicita y listo!
						rt = static_cast<Variante::tipo_real>(this->ObtenerEntero());
						break;
					default:
						throw ErrorDeSemantica("Error en Variante::ConvertirA: Conversión inválida");
				}
				break;
			case Variante::TipoReal:
				switch(dc)
				{
					case Variante::TipoCadena:
						rt = dac(this->ObtenerReal());
						break;
					case Variante::TipoEntero:
						// Como tipo_entero generalmente es un <long long int>, podemos
						// realizar una conversion explicita y listo!
						rt = static_cast<Variante::tipo_entero>(this->ObtenerReal());
						break;
					default:
						throw ErrorDeSemantica("Error en Variante::ConvertirA: Conversión inválida");
				}
				break;
			default:
				throw ErrorDeSemantica("Error en Variante::ConvertirA: Conversión inválida");
		}

		return rt;
	}

	bool Variante::PoseeTipo(void) const
	{
		return this->tipo != Variante::SinTipo;
	}

	void Variante::FijarTipo(const Variante::Discriminante& dc)
	{
		if(this->tipo == dc)
			return;
		this->DestruirMemoria();
		this->tipo = dc;
		switch(dc)
		{
			case Variante::TipoCadena:
				this->valor.cadena = new Variante::tipo_cadena;
				this->destruido = false;
				break;
			case Variante::TipoEntero:
				this->valor.entero = new Variante::tipo_entero;
				this->destruido = false;
				break;
			case Variante::TipoReal:
				this->valor.real = new Variante::tipo_real;
				this->destruido = false;
				break;
			case Variante::SinTipo:
				break;
			default:
				throw ErrorDeSemantica("Error en Variante::FijarTipo: tipo " + eas(this->tipo) + " no reconocido");
		}
		// std::cout << "Recreado variante con tipo: " << ((long long int)this) << ":" << dc << std::endl;
	}
	void Variante::FijarCadena(const Variante::tipo_cadena& cad)
	{
		this->DestruirMemoria();
		this->tipo = Variante::TipoCadena;
		this->valor.cadena = new Variante::tipo_cadena(cad);
		this->destruido = false;
		// std::cout << "Recreado variante con cadena: " << ((long long int)this) << ":" << cad << std::endl;
	}
	void Variante::FijarEntero(const Variante::tipo_entero& ent)
	{
		this->DestruirMemoria();
		this->tipo = Variante::TipoEntero;
		this->valor.entero = new Variante::tipo_entero(ent);
		this->destruido = false;
		// std::cout << "Recreado variante con entero: " << ((long long int)this) << ":" << ent << std::endl;
	}
	void Variante::FijarReal(const Variante::tipo_real& rea)
	{
		this->DestruirMemoria();
		this->tipo = Variante::TipoReal;
		this->valor.real = new Variante::tipo_real(rea);
		this->destruido = false;
		// std::cout << "Recreado variante con real: " << ((long long int)this) << ":" << rea << std::endl;
	}
	void Variante::Fijar(const Variante& var)
	{
		if(this == &var)
			return;
		// std::cout << "@@FijarA: " << var.tipo << ":" << ((long long int) &var) << ";" << ((long long int)this) << std::endl;
		switch((Variante::Discriminante) var)
		{
			case Variante::TipoCadena:
				this->FijarCadena(var.ObtenerCadena());
				break;
			case Variante::TipoEntero:
				this->FijarEntero(var.ObtenerEntero());
				break;
			case Variante::TipoReal:
				this->FijarReal(var.ObtenerReal());
				break;
			case Variante::SinTipo:
				break;
			default:
				this->tipo = Variante::SinTipo;
			// FIXME:
			/* default:
				throw ErrorDeSemantica(
					"Error en Variante::Fijar: tipo " + eas(this->tipo) + " no reconocido"
				); */
		}
	}

	// NOTA: Estas funciones comparar identidad, NO VALOR
	bool Variante::EsMismo(const Variante& var) const
	{
		return this == &var;
	}

	Variante::tipo_interno& Variante::ObtenerPuntero(void)
	{
		return this->valor;
	}
	Variante::Discriminante& Variante::ObtenerDiscriminante(void)
	{
		return this->tipo;
	}
	void Variante::DestruirMemoria(void)
	{
		if(this->destruido)
		{
			return;
		}
		//std::cout << "RDestruido variante en " << (long long int)this << std::endl
		//	<< "De tipo " << this->tipo << std::endl;
		if(this->tipo == Variante::TipoCadena)
		{
			//std::cout << "delcad" << std::endl;
			delete this->valor.cadena;
		}
		else if(this->tipo == Variante::TipoEntero)
		{
			//std::cout << "delent" << std::endl;
			delete this->valor.entero;
		}
		else if(this->tipo == Variante::TipoReal)
		{
			//std::cout << "delreal" << std::endl;
			delete this->valor.real;
		}
		//std::cout << "Fin 1" << std::endl;
		this->tipo = Variante::SinTipo;
		this->valor.cadena = nullptr;
		this->valor.entero = nullptr;
		this->valor.real = nullptr;
		this->destruido = true;
		//std::cout << "Fin 2" << std::endl;
	}

	Variante operator+(Variante a, Variante b)
	{
		if(a.ObtenerTipo() != b.ObtenerTipo())
		{
			throw ErrorDeSemantica("Error en Variante::operator+: Operacion inválida en tipos");
		}

		switch(a.ObtenerTipo())
		{
			case Variante::SinTipo:
				return Variante(Variante::SinTipo);
			case Variante::TipoCadena:
				return a.ObtenerCadena() + b.ObtenerCadena();
			case Variante::TipoEntero:
				return a.ObtenerEntero() + b.ObtenerEntero();
			case Variante::TipoReal:
				return a.ObtenerReal() + b.ObtenerReal();
			default:
				throw ErrorDeSemantica("Error en Variante::operator+: tipo no reconocido");
		}
	}
	Variante operator-(Variante a, Variante b)
	{
		if(a.ObtenerTipo() != b.ObtenerTipo())
		{
			throw ErrorDeSemantica("Error en Variante::operator-: Operacion inválida en tipos");
		}

		switch(a.ObtenerTipo())
		{
			case Variante::SinTipo:
				return Variante(Variante::SinTipo);
			case Variante::TipoCadena:
				throw ErrorDeSemantica("Error en Variante::operator-: Operacion inválida en cadenas");
			case Variante::TipoEntero:
				return a.ObtenerEntero() - b.ObtenerEntero();
			case Variante::TipoReal:
				return a.ObtenerReal() - b.ObtenerReal();
			default:
				throw ErrorDeSemantica("Error en Variante::operator-: tipo no reconocido");
		}
	}
	Variante operator*(Variante a, Variante b)
	{
		if(a.ObtenerTipo() != b.ObtenerTipo())
		{
			throw ErrorDeSemantica("Error en Variante::operator*: Operacion inválida en tipos");
		}

		switch(a.ObtenerTipo())
		{
			case Variante::SinTipo:
				return Variante(Variante::SinTipo);
			case Variante::TipoCadena:
				throw ErrorDeSemantica("Error en Variante::operator*: Operacion inválida en cadenas");
			case Variante::TipoEntero:
				return a.ObtenerEntero() * b.ObtenerEntero();
			case Variante::TipoReal:
				return a.ObtenerReal() * b.ObtenerReal();
			default:
				throw ErrorDeSemantica("Error en Variante::operator*: tipo no reconocido");
		}
	}
	Variante operator/(Variante a, Variante b)
	{
		if(a.ObtenerTipo() != b.ObtenerTipo())
		{
			throw ErrorDeSemantica("Error en Variante::operator/: Operacion inválida en tipos");
		}

		switch(a.ObtenerTipo())
		{
			case Variante::SinTipo:
				return Variante(Variante::SinTipo);
			case Variante::TipoCadena:
				throw ErrorDeSemantica("Error en Variante::operator/: Operacion inválida en cadenas");
			case Variante::TipoEntero:
				return a.ObtenerEntero() / b.ObtenerEntero();
			case Variante::TipoReal:
				return a.ObtenerReal() / b.ObtenerReal();
			default:
				throw ErrorDeSemantica("Error en Variante::operator/: tipo no reconocido");
		}
	}
}
