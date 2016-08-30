/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*** Este es PseudoD version 2.0.0                                      *****
*** Log de este archivo:                                               *****
*** Formato: DD/MM/YYYY: txt                                           *****
*** **** 14/09/2014: Se creo el archivo.                               *****
*** **** 08/03/2015: Se agrego soporte para CygWin y Windows           *****
*** **** 26/04/2015: Se agrego soporte para Meta-Programacion          *****
****************************************************************************
**************************************************************************/


#ifndef PDDATA_HEADER_H
#define PDDATA_HEADER_H

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

#define PDVOID_INIC_VACIO char
#define PDVOID_INIC_VACIO_NULL ('a')

typedef long long int PDentero;
typedef long double PDdecimal;
typedef std::string PDcadena;

template<class T>
int buscar(std::vector<T> a,T b);
PDcadena eas(long long int i);
PDcadena dac(long double i);
long long int cae(PDcadena i);
long double caf(PDcadena i);

/**
* @brief Representa el tipo de datos nativo del interprete.
*
* No es extendible.
*/
namespace PDvar
{
	using namespace std;

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
			/**
			* @brief Inicializador a base de vector
			*/
			explicit PDVoid(vector<T> args) : PDObjeto() {this->args = args;}
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
			vector<T> args;
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
			explicit Error(string classname,string message) noexcept;
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
			string ObtenerClaseEnPseudoD(void) const noexcept;
			/**
			* @brief Obtiene el error elemental
			* este es un string con el mensaje de error.
			*/
			string ObtenerErrorElemental(void) const noexcept;
			/**
			* @brief Devuelve el mensaje de error
			* este mensaje es estructurado para que un usuario pueda leerlo.
			*/
			string Mensaje(void) const noexcept;
		protected:
			/**
			* @brief Fija la clase manejadora.
			*/
			void FijarClase(string classname) noexcept;
			/**
			* @brief Fija el mensaje de error.
			*/
			void FijarMensaje(string message) noexcept;
		private:
			string PseudoDClass;
			string ErrorMessage;
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
			explicit ErrorDeSintaxis(string message) noexcept;
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
			explicit ErrorDeSemantica(string message) noexcept;
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
			explicit ErrorDelNucleo(string message) noexcept;
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

	/**
	* @brief El tipo de dato nativo del interprete, es usado internamente.
	*/
	class PDDatos : public PDObjeto
	{
		public:
			/**
			* @brief Crea una nueva instancia de la clase donde:
			* @param nvar Vector con los nombres de las variables.
			* @param vvar Vector con los valores de las variables.
			* @param npun Vector con los nombres de los punteros.
			* @param vpun Vector con los indices de los ounteros.
			* @param pil Vector de Pilas,(stacks) del lenguaje.
			*/
			PDDatos(vector<string>& nvar,vector<string>& vvar,vector<string>& npun,vector<int>& vpun,vector< stack<string> >& pil);
			/**
			* @brief inicia la instancia como Manejador principal de la memoria.
			* este tiene acceso a la memoria y la maneja de forma independiente.
			*/
			PDDatos(void);
			/**
			* @brief Destruye la clase
			*/
			~PDDatos();
		  /**
		  * @brief Es la constante que devuelven las funciones en caso de error.
		  */
			string ERROR;
			/**
			* @brief El valor de De Boole 1 o true en PseudoD
			*/
			string VERDADERO;
			/**
			* @brief El valor de De Boole 0 o false en PSeudoD
			*/
			string FALSO;
			/**
			* @brief Devuelve el valor de la variable o puntero.
			* @param n Nombre de la variable  puntero.
			*/
			string& ObtenerVariable(string n);
			/**
			* @brief Devuelve el valor del puntero.
			* @param n Nombre del puntero.
			*/
			string& ObtenerPuntero(string n);
			/**
			* @brief devuelve el indice al que apunte el puntero.
			* @param n nombre del puntero
			* @return Valor indice al que apunta
			*/
			int& ObtenerIndicePuntero(string n);
			/**
			* @brief Devuelve el tope de la pila.
			* @param p Numero de la pila
			* @return Tope de la pila
			*/
			string Tope(int p);
			/**
			* @brief Borra el tope de una pila
			* @param p Numero de la pila
			*/
			void BorrarTope(int p);
			/**
			* @brief llama a Tope y BorrarTope.
			* @param n numero de pila
			* @return El tope.
			*/
			string Sacar(int n);
			/**
			* @brief Empuja un valor en una pila
			* @param n Valor que se empujara
			* @param p Numero de la pila
			*/
			void Empujar(string n,int p);
			/**
			* @brief Crea una nueva pila
			*/
			void CrearPila();
			/**
			* @brief Crea una nueva variable o puntero
			* @param n Nombre de la nueva variable o puntero
			* @param t Tipo, "Variable" para crear una variable o "Puntero" para crear un puntero.
			* @param va Si es un puntero, direccion a la que apunta.
			* @param vl Se es una variable, se fija su valor a el valor de vl.
			*/
			void CrearVariable(string n,string t = "Variable", int va = 0,string vl = "nulo");
			/**
			* @brief Obtiene el indice de la variable o puntero
			* @param t tipo, "Variable" para una variable y "Puntero" para un puntero.
			* @param n Nombre a buscar
			* @return indice
			*/
			int BuscarIndice(string t,string n);
			/**
			* @brief Determina si existe la variable o puntero.
			* @param n nombre de la variable/puntero.
			* @param t "Variable" para variables o "Puntero" para punteros.
			* @return true si existe y false de lo contrario.
			*/
			bool ExisteVariable(string n,string t = "Variable");
			/**
			* @brief ejecuta una orden como interprete.
			* @param ord Orden a ejecutar.
			*/
			void Ejecutar(string ord);
			/**
			* @brief ejecuta una orden como interprete.
			* @param ord Orden a ejecutar.
			* @param in Flujo de tokens
			*/
			void Ejecutar(string ord,istream& in);
		public:
			/**
			* @brief puntero a los nombres de variable
			*/
			vector<string>* nombrev;
			/**
			* @brief Puntero a los valores de las variables
			*/
			vector<string>* valorv;
			/**
			* @brief Puntero a los nombres de los punteros
			*/
			vector<string>* nombrep;
			/**
			* @brief Puntero a los valores-indice de los punteros.
			*/
			vector<int>*    nvapunt;
			/**
			* @brief Puntero a vector que contiene las pilas.
			*/
			vector< stack< string > >* pilas;
			/**
			* @brief nivel de las advertencias.
			*/
			int adver;
		private:
			/**
			* @brief Si esta como manejador principal.
			*/
			bool manager;
		public:
			/**
			* @brief Funcion madre, permite ser recursivo en el lenguaje
			* este puntero a funcion es utilizado para la recursion, se llama de la forma:
			* (*PROCESAR)(orden,flujo,PROCESO);
			* esto ejecuta orden como una orden global de PseudoD
			*/
			void (*PROCESAR)(string o,istream& e, void(*)(string,istream&));
			/**
			* @brief Funcion hija, necesaria para la funcion madre
			* este puntero a funcion es una sub-funcion, la base del NEA. Se llama de la forma:
			* (*PROCESO)(orden,flujo);
			* Esto ejecuta orden como una orden que esta en el NEA
			*/
			void (*PROCESO)(string o,istream& i);
	};

	/**
	* @brief Clase base para funciones PDCallBack en forma de orden(plugins y el NIA).
	*/
	class PDEntradaBasica : virtual public PDVoid<PDVOID_INIC_VACIO>
	{
		public:
			/**
			* @brief Crea la entrada con el token tok, el flujo in, y los datos dat.
			*/
			explicit PDEntradaBasica(string tok, istream& in, PDDatos& dat);
			/**
			* @brief Destruye la entrada.
			*/
			virtual ~PDEntradaBasica(void);
			/**
			* @brief Obtiene el flujo de tokens.
			* @return Flujo de tokens
			*/
			istream& ObtenerFlujo(void);
			/**
			* @brief Obtiene el puntero a el manejador de memoria del interprete.
			* @return Puntero al manejador.
			*/
			PDDatos* ObtenerMemoria(void);
			/**
			* @brief Obtiene el token actual.
			* @return Token actual.
			*/
			string ObtenerToken(void);
		private:
			istream* in; // flujo de tokens
			string token; // token actual
			PDDatos* data; // datos del interprete

			explicit PDEntradaBasica(vector<PDVOID_INIC_VACIO> args) : PDVoid(args) {}
			explicit PDEntradaBasica(PDVOID_INIC_VACIO args) : PDVoid(args) {}
			explicit PDEntradaBasica(void) : PDVoid() {}
			virtual PDVOID_INIC_VACIO Obtener(int i = 0) {return PDVOID_INIC_VACIO_NULL;}
	};

	/**
	* @brief Devuelve el valor del token.
	* Diseñado para bucles y condicionales.
	* @param tok Token
	* @param in Flujo de tokens
	* @param data Puntero a la memoria del interprete.
	* @return el valor de los tokens
	*/
	string ValorDelToken(string tok,istream& in,PDDatos* data);

	/**
	* @brief Posee lo necesario para crear modulos dinamicos
	*/
	namespace Din
	{
		/**
		* @brief Representa un argumento para un modulo dinamico.
		*/
		struct Argumentos
		{
			std::string Instancia;
			std::string FuncionLlamada;
			std::vector<std::string>* Argumentos;
			PDDatos** Manejador;
		};

		/**
		* @brief Es la clase base para los modulos dinamicos.
		* Es abstracta.
		*/
		class ModuloDinamico
		{
			public:
				ModuloDinamico(void);
				virtual ~ModuloDinamico(void);
				virtual bool ManejarFuncion(PDvar::Din::Argumentos* args);
		};
	}
}

#endif
