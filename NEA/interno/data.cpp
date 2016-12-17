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

#include "data.hh"

PDCadena eas(PDEntero i)
{
#ifndef MINGW
	return std::to_string(i);
#else
	char buffer[50];
	sprintf(buffer,"%d",i);
	return std::string(buffer);
#endif
}

PDCadena dac(PDDecimal i)
{
#ifndef MINGW
	return std::to_string(i);
#else
	char buffer[50];
	sprintf(buffer,"%a",i);
	return std::string(buffer);
#endif
}

PDEntero cae(PDCadena i)
{
#ifndef MINGW
	return std::stoll(i);
#else
	return atoi(i.c_str());
#endif
}

PDDecimal caf(PDCadena i)
{
#ifndef MINGW
	return std::stold(i);
#else
	return atof(i.c_str());
#endif
}

namespace PDvar
{
	Error::Error(void) noexcept : PseudoDClass("Error"), ErrorMessage("")
	{
	}
	Error::Error(const Error& other) noexcept
	{
		this->PseudoDClass = other.ObtenerClaseEnPseudoD();
		this->ErrorMessage = other.ObtenerErrorElemental();
	}
	Error::Error(PDCadena classname, PDCadena message) noexcept
		: PseudoDClass(classname), ErrorMessage(message)
	{
	}
	Error::~Error(void) noexcept
	{
	}
	Error& Error::operator=(const Error& other) noexcept
	{
		this->PseudoDClass = other.ObtenerClaseEnPseudoD();
		this->ErrorMessage = other.ObtenerErrorElemental();
		return *this;
	}
	const char* Error::what(void) noexcept
	{
		return this->ObtenerErrorElemental().c_str();
	}
	PDCadena Error::ObtenerClaseEnPseudoD(void) const noexcept
	{
		return this->PseudoDClass;
	}
	PDCadena Error::ObtenerErrorElemental(void) const noexcept
	{
		return this->ErrorMessage;
	}
	void Error::FijarClase(PDCadena classname) noexcept
	{
		this->PseudoDClass = classname;
	}
	void Error::FijarMensaje(PDCadena message) noexcept
	{
		this->ErrorMessage = message;
	}
	PDCadena Error::Mensaje(void) const noexcept
	{
		return "(" + this->ObtenerClaseEnPseudoD() + ") " + this->ObtenerErrorElemental();
	}
	ErrorDeSintaxis::ErrorDeSintaxis(void) noexcept : Error("ErrorDeSintaxis","")
	{}
	ErrorDeSintaxis::ErrorDeSintaxis(PDCadena message) noexcept : Error("ErrorDeSintaxis",message)
	{}
	ErrorDeSintaxis::ErrorDeSintaxis(const Error& other) noexcept : Error(other)
	{}
	ErrorDeSintaxis::~ErrorDeSintaxis(void) noexcept
	{}
	ErrorDeSemantica::ErrorDeSemantica(void) noexcept : Error("ErrorDeSemantica","")
	{}
	ErrorDeSemantica::ErrorDeSemantica(PDCadena message) noexcept : Error("ErrorDeSemantica",message)
	{}
	ErrorDeSemantica::ErrorDeSemantica(const Error& other) noexcept : Error(other)
	{}
	ErrorDeSemantica::~ErrorDeSemantica(void) noexcept
	{}
	ErrorDelNucleo::ErrorDelNucleo(void) noexcept : Error("ErrorDelNucleo","")
	{}
	ErrorDelNucleo::ErrorDelNucleo(PDCadena message) noexcept : Error("ErrorDelNucleo",message)
	{}
	ErrorDelNucleo::ErrorDelNucleo(const Error& other) noexcept : Error(other)
	{}
	ErrorDelNucleo::~ErrorDelNucleo(void) noexcept
	{}
}
