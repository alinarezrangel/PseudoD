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


#ifndef __PSEUDOD_NEA_TIPOS_HH__
#define __PSEUDOD_NEA_TIPOS_HH__ 1

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <string>
#include <cstring>
#include <cstdio>


#include "PDData.hh"

/********************************************************************************************/

/**
* @brief Contiene Todos los datos estandares en PseudoD.
*
* Ofrece todos los tipos de datos tanto para PseudoD como para Programadores que deseen extender NEA.
*/
namespace PDTipos
{
	using namespace PDvar;
	using namespace std;

	/**
	* @brief Representa la clase base para las extenciones de PseudoD.
	*/
	class PDOrden : virtual public PDObjeto
	{
		public:
			/**
			* @brief Crea una nueva instancia de PDOrden
			*/
			PDOrden();
			/**
			* @brief Destructor
			*/
			virtual ~PDOrden();
			/**
			* @brief Obtiene la clave de la orden.
			* @return La clave
			*/
			string ObtenerClave();
			/**
			* @brief Lee los parametros del flujo in como instancias.
			* @param in Flujo del cual se leeran los datos
			*/
			virtual void LeerParametros(istream& in);
		protected:
			/**
			* @brief Fija la clave de la orden y el paquete
			* @param cl Clave nueva.
			* @param pack Nombre del paquete.
			*/
			void FijarClave(string cl,string pack);
		private:
			string clave;
	};

	/**
	* @brief Clase base para todos los tipos de datos no declarados nativos de PseudoD.
	*/
	class PDInstancia : virtual public PDOrden
	{
		public:
			/**
			* @brief Crea una nueva instancia
			*/
			PDInstancia() : PDObjeto() {}
			/**
			* @brief Destruye la instancia
			*/
			virtual ~PDInstancia(){}
			/**
			* @brief Guarda la instancia en la memoria del interprete de PseudoD, VIRTUAL PURA.
			*/
			virtual void InscribirInstancia(PDDatos* data) = 0;
	};

	/**
	* @brief Tipo del lenguaje, representa un array.
	*
	* Nota: No confundir, no es un array para C++ es solo para uso de PseudoD.
	*/
	class PseudoArray : virtual public PDInstancia
	{
		public:
			/**
			* @brief crea un array de datos.
			* todos los datos son inicializados a NULO.
			* @param a nombre del nuevo array
			* @param b cantidad de elementos
			*/
			PseudoArray(string a = "____",int b = 0);
			/**
			* @brief destruye este objeto.
			*/
			virtual ~PseudoArray();
			/**
			* @brief Guarda el nuevo Array en la memoria del interprete.
			* @param data Puntero a la memoria del interprete
			*/
			void InscribirInstancia(PDDatos* data);
			/**
			* @biref Lee los parametros necesarios para crear el nuevo Array.
			* @param in Flujo del que se leen los parametros
			*/
			void LeerParametros(istream& in);
		private:
			int cant;
			string nm;
	};

	/**
	* @brief Tipo del lenguaje, representa una estructura.
	*
	* Nota: No confundir, es solo para PseudoD no es una estructura de C++.
	*/
	class PseudoClase : virtual public PDInstancia
	{
		public:
			/**
			* @brief Inicializador.
			*
			* Crea una instancia que sirve para crear una estructura en PseudoD
			* @param a nombre de la clase
			* @param b campos de la dicha clase
			*/
			PseudoClase(string a = "____",vector<string> b = vector<string>());
			/**
			* @brief Destruye la dicha instancia
			*/
			virtual ~PseudoClase();
			/**
			* @brief Guarda la instancia en memoria del interprete.
			* @param data Puntero a la memoria del interprete.
			*/
			void InscribirInstancia(PDDatos* data);
			/**
			* @brief Lee los parametros necesarios.
			* @param in Flujo donde estan los parametros.
			*/
			void LeerParametros(istream& in);
		private:
			string nm;
			vector<string> methods;
	};

	/**
	* @brief Representa una referencia a una instancia de PseudoClase
	*
	* Nota: No confundir es solo para PseudoD no C++
	*/
	class PseudoReferenciaClase : virtual public PDInstancia
	{
		public:
			/**
			* @brief Inicializador
			*
			* Crea una instancia que sirve para crear una instancia de una clase ya existente
			* @param a nombre de la clase a instanciar
			* @param b nombre de la instancia
			*/
			PseudoReferenciaClase(string a = "____",string b = "____");
			/**
			* @brief Destructor
			*/
			virtual ~PseudoReferenciaClase();
			/**
			* @brief Guarda la instancia actual en la memoria tipo interprete
			* @param data Memoria tipo interprete
			*/
			void InscribirInstancia(PDDatos* data);
			/**
			* @brief Lee los parametros necesarios para crear la instancia
			*/
			void LeerParametros(istream& in);
		private:
			string nm;
			string ni;
			vector<string> methods;
	};

	/**
	* @brief Representa una manera de visualizar en texto la memoria del interprete.
	*
	* Nota: No confundir, es para PseudoD no C++.
	*/
	class PseudoDebug : public PDInstancia
	{
		public:
			/**
			* @brief Inicializador
			*
			* Crea una instancia que sirve para visualizar los datos de memoria
			*/
			PseudoDebug() : PDInstancia(){this->FijarClave(string("debug"),string("PseudoD"));}
			/**
			* @brief Destructor
			*/
			virtual ~PseudoDebug(){}
			/**
			* @brief Lee y visualiza la memoria del interprete.
			*
			* Al visualizarla, la muestra como en arreglos de la forma ["valor","valor"] numerovalores. Visualiza casi todos los datos como:
			*
			* Nombres de las variables.
			* Valores de las variables.
			* Nombres de los punteros.
			* Valores de los puntero y variables a las que apuntan.
			* Los contenidos de las pilas .
			*
			* @param data memoria del interprete
			*/
			void InscribirInstancia(PDDatos* data);
	};

	/**
	* @brief Instancia que sirve para borrar una variable
	*/
	class PseudoArrayEstructura : public PDInstancia
	{
		public:
			/**
			* @brief Inicializador
			* @param a nombre del array
			* @param b nombre de la estructura
			* @param c cantidad de elementos
			*/
			PseudoArrayEstructura(string a = "____",string b = "____",int c = 0);
			/**
			* @brief Destructor
			*/
			virtual ~PseudoArrayEstructura();
			/**
			* @brief Lee los parametros para ejecutar la orden
			* @param in Flujo donde estan los parametros
			*/
			void LeerParametros(istream& in);
			/**
			* @brief Actua sobre la memoria creando el array dentro de la estructura
			* @param data memoria del interprete
			*/
			void InscribirInstancia(PDDatos* data);
		private:
			string nma;
			string nme;
			int tma;
	};

	/**
	* @brief Instancia que sirve para borrar una variable
	*/
	class PseudoBorrarVariable : public PDInstancia
	{
		public:
			/**
			* @brief Inicializador
			* @param a variable o puntero a borrar
			*/
			PseudoBorrarVariable(string a = "____");
			/**
			* @brief Destructor
			*/
			virtual ~PseudoBorrarVariable();
			/**
			* @brief Lee los parametros necesarios para ejecutar la orden
			* @param in Flujo donde estan los parametros
			*/
			void LeerParametros(istream& in);
			/**
			* @brief actua sobre la memoria incapacitando el uso de la variable
			* @param data Memoria del interprete
			*/
			void InscribirInstancia(PDDatos* data);
		private:
			string nm;
	};

	/**
	* @brief Instancia que sirve para heredar dos estructuras ya existentes
	*/
	class PseudoHerencia : public PDInstancia
	{
		public:
			/**
			* @brief Inicializador
			* @param a clase base
			* @param b clase hija
			*/
			PseudoHerencia(string a = "____",string b = "____");
			/**
			* @brief Destructor
			*/
			virtual ~PseudoHerencia();
			/**
			* @brief Lee los parametros necesarios
			* @param in Flujo con los parametros
			*/
			void LeerParametros(istream& in);
			/**
			* @brief Hereda las clases actuando sobre memoria del interprete
			* @param data Memoria del interprete
			*/
			void InscribirInstancia(PDDatos* data);
		private:
			string nmb,nmh;
	};

	/**
	* @brief Instancia que sirve para cambiar la direccion en memoria de un punteros
	* Es solo para PseudoD no C++
	*/
	class PseudoDireccionarPuntero : public PDInstancia
	{
		public:
			/**
			* @brief Inicializador
			* @param a nombre del puntero
			* @param b nombre de la nueva variable
			*/
			PseudoDireccionarPuntero(string a = "____",string b = "____");
			/**
			* @brief destructor
			*/
			virtual ~PseudoDireccionarPuntero();
			/**
			* @brief Lee los parametros necesarios
			* @param in Flujo con los parametros
			*/
			void LeerParametros(istream& in);
			/**
			* @brief Hereda las clases actuando sobre memoria del interprete
			* @param data Memoria del interprete
			*/
			void InscribirInstancia(PDDatos* data);
		private:
			string nmp;
			string nmv;
	};

	/**
	* @brief Representa el bucle mientras, pero pide una meta-funcion en vez de un cuerpo
	* Es solo para PseudoD no C++
	*/
	class PseudoMientras : public PDInstancia
	{
		public:
			/**
			* @brief Inicializador
			* @param a token inicial de la expresion.
			* @param o resto de la expresion.
			* @param b cuerpo a ejecutar.
			*/
			PseudoMientras(string b = "____",string o = "____",string f = "____");
			/**
			* @brief destructor
			*/
			virtual ~PseudoMientras();
			/**
			* @brief Lee los parametros necesarios
			* @param in Flujo con los parametros
			*/
			void LeerParametros(istream& in);
			/**
			* @brief Hereda las clases actuando sobre memoria del interprete
			* @param data Memoria del interprete
			*/
			void InscribirInstancia(PDDatos* data);
		private:
			string nmv;
			string func;
			string orden;
	};

	/**
	* @brief Anida una clase en otra
	* Es solo para PseudoD no C++
	*/
	class PseudoClaseContenida : public PDInstancia
	{
		public:
			/**
			* @brief Inicializador
			* @param a nombre de la estructura
			* @param b estructura del campo nuevo
			* @param c nombre del campo
			* @param d 1 para un puntero y 0 para un campo estatico
			*/
			PseudoClaseContenida(string a = "____",string b = "____",string c = "____",bool d = false);
			/**
			* @brief destructor
			*/
			virtual ~PseudoClaseContenida();
			/**
			* @brief Lee los parametros necesarios
			* @param in Flujo con los parametros
			*/
			void LeerParametros(istream& in);
			/**
			* @brief Hereda las clases actuando sobre memoria del interprete
			* @param data Memoria del interprete
			*/
			void InscribirInstancia(PDDatos* data);
		private:
			string nme;
			string tpe;
			string nmv;
			bool ptr;
	};

	/**
	* @brief Borra de forma inteligente una instancia
	* Es solo para PseudoD no C++
	*/
	class PseudoBorrarInteligente : public PDInstancia
	{
		public:
			/**
			* @brief Inicializador
			* @param a nombre de la variable con tipo completo
			*/
			PseudoBorrarInteligente(string a = "____");
			/**
			* @brief destructor
			*/
			virtual ~PseudoBorrarInteligente();
			/**
			* @brief Lee los parametros necesarios
			* @param in Flujo con los parametros
			*/
			void LeerParametros(istream& in);
			/**
			* @brief Hereda las clases actuando sobre memoria del interprete
			* @param data Memoria del interprete
			*/
			void InscribirInstancia(PDDatos* data);
		private:
			string var;
	};
}

#endif
