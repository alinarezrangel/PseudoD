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

#ifndef __PSEUDOD_NEA_INTERNO_TIPOS_HH__
#define __PSEUDOD_NEA_INTERNO_TIPOS_HH__ 1

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <string>
#include <cstring>
#include <cstdio>
#include <exception>

#ifdef NUMEROS_EN_C
# define MINGW
#endif

typedef long long int PDEntero;
typedef long double PDDecimal;
typedef std::string PDCadena;
typedef void(*PDFuncionNEA)(PDCadena, std::istream&);
typedef void (*PDFuncionNIA)(PDCadena, std::istream&, PDFuncionNEA);

template<class T>
int buscar(std::vector<T> a,T b);
PDCadena eas(PDEntero i);
PDCadena dac(PDDecimal i);
PDEntero cae(PDCadena i);
PDDecimal caf(PDCadena i);

/**
* @brief Espacio de nombres principal de PseudoD.
*
* Posee las clases PDDatos y PDObjeto.
*/
namespace PDvar
{
	/**
	* @brief Es la clase base para todas las clases de NEA.
	*/
	class PDObjeto
	{
		public:
			/**
			* @brief Constructor por defecto
			*/
			PDObjeto(){}
			/**
			* @brief Destructor
			*/
			virtual ~PDObjeto(){}
			/**
			* @brief Determina si el objeto actual y su parametro son la misma instancia
			* @param o La instancia que se va a comparar.
			* @return true si son la misma instancia y false de lo contrario.
			*/
			virtual bool MismoObjeto(PDObjeto& o){ return this == &o; }
	};

	/**
	* @brief Representa la clase base para todas las clases de parametros de PDCallBack.
	*/
	template<class T>
	class PDVoid : virtual public PDObjeto
	{
		public:
			typedef T type;
			/**
			* @brief Inicializador a base de vector
			*/
			explicit PDVoid(std::vector<T> args) : PDObjeto() {this->args = args;}
			/**
			* @brief Inicializador a base de objeto
			*/
			explicit PDVoid(T arg) : PDObjeto() {this->args.push_back(arg);}
			/**
			* @brief Inicializador sin base
			*/
			explicit PDVoid(void) : PDObjeto() {}
			/**
			* @brief Destructor, no hace nada.
			*/
			virtual ~PDVoid(){}
			/**
			* @brief Devuelve el argumento
			* @param i numero de argumento
			* @return objeto
			*/
			virtual T Obtener(int i = 0){return args[i];}
		private:
			std::vector<T> args;
	};

	/**
	* @brief Representa un objeto de retro-llamada.
	*/
	template<typename RV,typename AT>
	class PDCallBack : virtual public PDObjeto
	{
		public:
			typedef RV return_type;
			typedef AT argument_type;
			/**
			* @brief Crea un nuevo call-back
			* @param F Puntero a funcion que se retro-llamara.
			*/
			PDCallBack (RV (*F)(AT));
			/**
			* @brief Destructor de PDCall-Back.
			*/
			virtual ~PDCallBack ();
			/**
			* @brief Llama a la funcion de retro-llamada.
			* @param p parametro que se le pasara a la funcion.
			* @return Resultado de la llamada
			*/
			RV Call(AT p);
			/**
			* @brief Llama a la funcion de retro-llamada con los parametros pre-establecidos
			* @return Resultado de la llamada
			*/
			RV Call();
			/**
			* @brief Fija los parametros preestablesidos.
			* @param v Nuevo parametro pre-establesido.
			*/
			void Set(AT v);
			/**
			* @brief Obtiene los parametros por defecto.
			* @return parametros por defecto
			*/
			AT Get();
		private:
			RV (** Fcn)(AT);
			AT param;
	};

	template<typename RV,typename AT>
	PDCallBack<RV,AT>::PDCallBack (RV (*F)(AT)) : PDObjeto()
	{
		this->Fcn = &F;
	}

	template<typename RV,typename AT>
	PDCallBack<RV,AT>::~PDCallBack ()
	{
		// ~~~~~~~~~Nada
	}

	template<typename RV,typename AT>
	RV PDCallBack<RV,AT>::Call(AT p)
	{
		return (**this->Fcn)(p);
	}

	template<typename RV,typename AT>
	RV PDCallBack<RV,AT>::Call()
	{
		return (**this->Fcn)(this->param);
	}

	template<typename RV,typename AT>
	void PDCallBack<RV,AT>::Set(AT v)
	{
		this->param = v;
	}

	template<typename RV,typename AT>
	AT PDCallBack<RV,AT>::Get()
	{
		return this->param;
	}

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

#endif
