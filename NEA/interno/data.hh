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

#ifndef __DATA_PSEUDOD_INTERNO_H__
#define __DATA_PSEUDOD_INTERNO_H__

#include <iostream>
#include <string>
#include <vector>

#ifdef NUMEROS_EN_C
# define MINGW
#endif

#define PDVOID_INIC_VACIO char
#define PDVOID_INIC_VACIO_NULL ('a')

/**
* @file data.hh
* Declara funciones y clases globales para PseudoD
*/

typedef long long int PDEntero;
typedef long double PDDecimal;
typedef std::string PDCadena;
typedef void(*PDFuncionNEA)(PDCadena, std::istream&);
typedef void (*PDFuncionNIA)(PDCadena, std::istream&, PDFuncionNEA);

template<class T>
int buscar(std::vector<T> a, const T& b)
{
	for(int i = (a.size() - 1);i >= 0;i--)
	{
		if(a[i] == b)
		{
			return i;
		}
	}
	return -1;
}

PDCadena eas(PDEntero i);
PDCadena dac(PDDecimal i);
PDEntero cae(PDCadena i);
PDDecimal caf(PDCadena i);

/**
* @brief El espacio de nombres principal de PseudoD NEA.
*/
namespace PDvar
{
	/**
	* @brief Representa un error.
	* Esta clase simplemente es la clase base, no posee información adicional.
	*/
	class Error : public std::exception
	{
		public:
			/**
			* @brief Crea un error
			* Por defecto, el mensaje es nulo y el nombre del manejador es "Error"
			*/
			explicit Error(void) noexcept;
			/**
			* @brief Crea un error
			* @param classname Nombre del mnejador en PseudoD.
			* @param message Mensaje de error.
			*/
			explicit Error(PDCadena classname, PDCadena message) noexcept;
			/**
			* @brief Crea un error
			* Copia la información desde otro error.
			* @param other Error a copiar.
			*/
			explicit Error(const Error& other) noexcept;
			/**
			* @brief Destruye este error.
			*/
			virtual ~Error(void) noexcept;
			/**
			* @brief Copia la información desde otro error.
			*/
			virtual Error& operator=(const Error& other) noexcept;
			/**
			* @brief Devuelve el mensaje de error
			* El mensaje es devuelto en forma de un C-string.
			*/
			virtual const char* what(void) noexcept;
			/**
			* @brief Obtiene el manejador.
			* Devuelve un string con el nombre de la clase en PseudoD que
			* representa este error.
			*/
			PDCadena ObtenerClaseEnPseudoD(void) const noexcept;
			/**
			* @brief Obtiene el error elemental
			* este es un string con el mensaje de error.
			*/
			PDCadena ObtenerErrorElemental(void) const noexcept;
			/**
			* @brief Devuelve el mensaje de error
			* este mensaje es estructurado para que un usuario pueda leerlo.
			*/
			PDCadena Mensaje(void) const noexcept;
		protected:
			/**
			* @brief Fija la clase manejadora.
			*/
			void FijarClase(PDCadena classname) noexcept;
			/**
			* @brief Fija el mensaje de error.
			*/
			void FijarMensaje(PDCadena message) noexcept;
		private:
			PDCadena PseudoDClass;
			PDCadena ErrorMessage;
	};

	/**
	* @brief Representa un error de sintaxis.
	*/
	class ErrorDeSintaxis : public Error
	{
		public:
			/**
			* @brief Crea un error
			* Por defecto, el mensaje es nulo y el manejador "ErrorDeSintaxis".
			*/
			explicit ErrorDeSintaxis(void) noexcept;
			/**
			* @brief Crea un error
			* @param message Mensaje de error.
			*/
			explicit ErrorDeSintaxis(PDCadena message) noexcept;
			/**
			* @brief Crea un error
			* Copia la información desde otro error.
			* @param other Error a copiar.
			*/
			explicit ErrorDeSintaxis(const Error& other) noexcept;
			/**
			* @brief Destruye este error.
			*/
			virtual ~ErrorDeSintaxis(void) noexcept;
	};

	/**
	* @brief Representa un error de semantica.
	*/
	class ErrorDeSemantica : public Error
	{
		public:
			/**
			* @brief Crea un error
			* Por defecto, el mensaje es nulo y el manejador "ErrorDeSemantica".
			*/
			explicit ErrorDeSemantica(void) noexcept;
			/**
			* @brief Crea un error
			* @param message Mensaje de error.
			*/
			explicit ErrorDeSemantica(PDCadena message) noexcept;
			/**
			* @brief Crea un error
			* Copia la información desde otro error.
			* @param other Error a copiar.
			*/
			explicit ErrorDeSemantica(const Error& other) noexcept;
			/**
			* @brief Destruye este error.
			*/
			virtual ~ErrorDeSemantica(void) noexcept;
	};

	/**
	* @brief Representa un error del nucleo.
	*/
	class ErrorDelNucleo : public Error
	{
		public:
			/**
			* @brief Crea un error
			* Por defecto, el mensaje es nulo y el manejador "ErrorDelNucleo".
			*/
			explicit ErrorDelNucleo(void) noexcept;
			/**
			* @brief Crea un error
			* @param message Mensaje de error.
			*/
			explicit ErrorDelNucleo(PDCadena message) noexcept;
			/**
			* @brief Crea un error
			* Copia la información desde otro error.
			* @param other Error a copiar.
			*/
			explicit ErrorDelNucleo(const Error& other) noexcept;
			/**
			* @brief Destruye este error.
			*/
			virtual ~ErrorDelNucleo(void) noexcept;
	};
}

#endif /* ~__DATA_PSEUDOD_INTERNO_H__ */
