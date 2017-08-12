/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*****                                                                  *****
***** Página oficial de PseudoD en:                                    *****
*****     http://pseudod.sourceforge.net/                              *****
*****                                                                  *****
****************************************************************************
Copyright 2016 Alejandro Linarez Rangel

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**************************************************************************/

#ifndef HDR_PSEUDOD_NEA_TIPOS_HH
#define HDR_PSEUDOD_NEA_TIPOS_HH 1

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <string>
#include <cstring>
#include <cstdio>
#include <type_traits>
#include <functional>

#include "interno/data.hh"
#include "interno/nmemoic.hh"
#include "interno/token.hh"
#include "interno/tokenizer.hh"
#include "PDData.hh"

/******************************************************************************/

/**
* @brief Contiene Todos los datos estandares en PseudoD.
*
* Ofrece todos los tipos de datos tanto para PseudoD como para Programadores que deseen extender NEA.
*/
namespace PDTipos
{
	/**
	* @brief Representa la clase base para las extenciones de PseudoD.
	*/
	class PDOrden : virtual public PDvar::PDObjeto
	{
		public:
			/**
			* @brief Crea una nueva instancia de PDOrden
			*/
			PDOrden(void);
			/**
			* @brief Destructor
			*/
			virtual ~PDOrden(void);
			/**
			* @brief Obtiene la clave de la orden.
			* @return La clave
			*/
			PDCadena ObtenerClave(void);
			/**
			* @brief Lee los parametros del flujo in como instancias.
			* @param in Flujo del cual se leeran los datos
			*/
			virtual void LeerParametros(pseudod::Tokenizador& in);
		protected:
			/**
			* @brief Fija la clave de la orden y el paquete
			* @param cl Clave nueva.
			* @param pack Nombre del paquete.
			*/
			void FijarClave(PDCadena cl, PDCadena pack);
		private:
			PDCadena clave;
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
			PDInstancia(void) : PDOrden() {}
			/**
			* @brief Destruye la instancia
			*/
			virtual ~PDInstancia(void) {}
			/**
			* @brief Guarda la instancia en la memoria del interprete de PseudoD, VIRTUAL PURA.
			*/
			virtual void InscribirInstancia(PDvar::PDDatos* data) = 0;
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
			PseudoArray(PDCadena a = "____", int b = 0);
			/**
			* @brief destruye este objeto.
			*/
			virtual ~PseudoArray(void);
			/**
			* @brief Guarda el nuevo Array en la memoria del interprete.
			* @param data Puntero a la memoria del interprete
			*/
			void InscribirInstancia(PDvar::PDDatos* data);
			/**
			* @biref Lee los parametros necesarios para crear el nuevo Array.
			* @param in Flujo del que se leen los parametros
			*/
			void LeerParametros(pseudod::Tokenizador& in);
		private:
			int cant;
			PDCadena nm;
	};

	/**
	* @brief Clase del lenguaje.
	*
	* Nota: No confundir, no es un array para C++ es solo para uso de PseudoD.
	*/
	class PseudoClase : virtual public PDInstancia
	{
		public:
			/**
			* @brief Crea una clase.
			*
			* @param nm Nombre de la clase.
			* @param base Nombre de la clase base.
			* @param mt Todos sus atributos o metodos.
			*/
			PseudoClase(
				PDCadena nm = "____",
				PDCadena base = "",
				std::vector<PDCadena> mt = std::vector<PDCadena>()
			);
			/**
			* @brief destruye este objeto.
			*/
			virtual ~PseudoClase(void);
			/**
			* @brief Guarda la nueva clase en la memoria del interprete.
			* @param data Puntero a la memoria del interprete
			*/
			void InscribirInstancia(PDvar::PDDatos* data);
			/**
			* @biref Lee los parametros necesarios para crear la nueva clase.
			* @param in Flujo del que se leen los parametros
			*/
			void LeerParametros(pseudod::Tokenizador& in);
		private:
			PDCadena nm;
			PDCadena base;
			std::vector<PDCadena> methods;
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
			* Crea una instancia que sirve para crear una instancia de una clase ya
			* existente.
			*
			* @param a nombre de la clase a instanciar
			* @param b nombre de la instancia
			*/
			PseudoReferenciaClase(PDCadena a = "____", PDCadena b = "____");
			/**
			* @brief Destructor
			*/
			virtual ~PseudoReferenciaClase(void);

			/**
			* @brief Guarda la instancia actual en la memoria tipo interprete
			* @param data Memoria tipo interprete
			*/
			void InscribirInstancia(PDvar::PDDatos* data);
			/**
			* @brief Lee los parametros necesarios para crear la instancia
			*/
			void LeerParametros(pseudod::Tokenizador& in);
		private:
			PDCadena nm;
			PDCadena ni;
			std::vector<PDCadena> methods;
	};

	/**
	* @brief Representa una manera de visualizar en texto la memoria del interprete.
	*
	* Nota: No confundir, es para PseudoD no C++.
	*/
	class PseudoDebug : virtual public PDInstancia
	{
		public:
			/**
			* @brief Inicializador
			*
			* Crea una instancia que sirve para visualizar los datos de memoria
			*/
			PseudoDebug(void) : PDInstancia()
			{
				this->FijarClave("debug", "PseudoD");
			}
			/**
			* @brief Destructor
			*/
			virtual ~PseudoDebug(void) {}

			/**
			* @brief Inicia el depurador del interprete.
			*
			* Desde el depurador, se puede manipular la memoria del interprete desde
			* la línea de comandos. Esto es muy útil mientras se busca una falla en
			* algún programa, sin embargo, es realmente importante quitar cualquier
			* aparición de esta instrucción cuando el programa pase a producción.
			*
			* @param data memoria del interprete
			*/
			void InscribirInstancia(PDvar::PDDatos* data);
	};

	/**
	* @brief Instancia que sirve para borrar una variable
	*/
	class PseudoArrayEstructura : virtual public PDInstancia
	{
		public:
			/**
			* @brief Inicializador
			* @param a nombre del array
			* @param b nombre de la estructura
			* @param c cantidad de elementos
			*/
			PseudoArrayEstructura(
				PDCadena a = "____",
				PDCadena b = "____",
				int c = 0
			);
			/**
			* @brief Destructor
			*/
			virtual ~PseudoArrayEstructura(void);

			/**
			* @brief Lee los parametros para ejecutar la orden
			* @param in Flujo donde estan los parametros
			*/
			void LeerParametros(pseudod::Tokenizador& in);
			/**
			* @brief Actua sobre la memoria creando el array dentro de la estructura
			* @param data memoria del interprete
			*/
			void InscribirInstancia(PDvar::PDDatos* data);
		private:
			PDCadena nma;
			PDCadena nme;
			int tma;
	};

	/**
	* @brief Instancia que sirve para borrar una variable
	*/
	class PseudoBorrarVariable : virtual public PDInstancia
	{
		public:
			/**
			* @brief Inicializador
			* @param a variable o puntero a borrar
			*/
			PseudoBorrarVariable(PDCadena a = "____");
			/**
			* @brief Destructor
			*/
			virtual ~PseudoBorrarVariable(void);

			/**
			* @brief Lee los parametros necesarios para ejecutar la orden
			* @param in Flujo donde estan los parametros
			*/
			void LeerParametros(pseudod::Tokenizador& in);
			/**
			* @brief actua sobre la memoria incapacitando el uso de la variable
			* @param data Memoria del interprete
			*/
			void InscribirInstancia(PDvar::PDDatos* data);
		private:
			PDCadena nm;
	};

	/**
	* @brief Instancia que sirve para heredar dos estructuras ya existentes
	*/
	class PseudoHerencia : virtual public PDInstancia
	{
		public:
			/**
			* @brief Inicializador
			* @param a clase base
			* @param b clase hija
			*/
			PseudoHerencia(PDCadena a = "____", PDCadena b = "____");
			/**
			* @brief Destructor
			*/
			virtual ~PseudoHerencia(void);

			/**
			* @brief Lee los parametros necesarios
			* @param in Flujo con los parametros
			*/
			void LeerParametros(pseudod::Tokenizador& in);
			/**
			* @brief Hereda las clases actuando sobre memoria del interprete
			* @param data Memoria del interprete
			*/
			void InscribirInstancia(PDvar::PDDatos* data);
		private:
			PDCadena nmb;
			PDCadena nmh;
	};

	/**
	* @brief Instancia que sirve para cambiar la direccion en memoria de un punteros
	* Es solo para PseudoD no C++
	*/
	class PseudoDireccionarPuntero : virtual public PDInstancia
	{
		public:
			/**
			* @brief Inicializador
			* @param a nombre del puntero
			* @param b nombre de la nueva variable
			*/
			PseudoDireccionarPuntero(PDCadena a = "____", PDCadena b = "____");
			/**
			* @brief destructor
			*/
			virtual ~PseudoDireccionarPuntero(void);

			/**
			* @brief Lee los parametros necesarios
			* @param in Flujo con los parametros
			*/
			void LeerParametros(pseudod::Tokenizador& in);
			/**
			* @brief Hereda las clases actuando sobre memoria del interprete
			* @param data Memoria del interprete
			*/
			void InscribirInstancia(PDvar::PDDatos* data);
		private:
			PDCadena nmp;
			PDCadena nmv;
	};

	/**
	* @brief Anida una clase en otra
	* Es solo para PseudoD no C++
	*/
	class PseudoClaseContenida : virtual public PDInstancia
	{
		public:
			/**
			* @brief Inicializador
			* @param a nombre de la estructura
			* @param b estructura del campo nuevo
			* @param c nombre del campo
			* @param d 1 para un puntero y 0 para un campo estatico
			*/
			PseudoClaseContenida(
				PDCadena a = "____",
				PDCadena b = "____",
				PDCadena c = "____",
				bool d = false
			);
			/**
			* @brief destructor
			*/
			virtual ~PseudoClaseContenida(void);

			/**
			* @brief Lee los parametros necesarios
			* @param in Flujo con los parametros
			*/
			void LeerParametros(pseudod::Tokenizador& in);
			/**
			* @brief Hereda las clases actuando sobre memoria del interprete
			* @param data Memoria del interprete
			*/
			void InscribirInstancia(PDvar::PDDatos* data);
		private:
			PDCadena nme;
			PDCadena tpe;
			PDCadena nmv;
			bool ptr;
	};

	/**
	* @brief Borra de forma inteligente una instancia
	* Es solo para PseudoD no C++
	*/
	class PseudoBorrarInteligente : virtual public PDInstancia
	{
		public:
			/**
			* @brief Inicializador
			* @param a nombre de la variable con tipo completo
			*/
			PseudoBorrarInteligente(PDCadena a = "____");
			/**
			* @brief destructor
			*/
			virtual ~PseudoBorrarInteligente(void);

			/**
			* @brief Lee los parametros necesarios
			* @param in Flujo con los parametros
			*/
			void LeerParametros(pseudod::Tokenizador& in);
			/**
			* @brief Hereda las clases actuando sobre memoria del interprete
			* @param data Memoria del interprete
			*/
			void InscribirInstancia(PDvar::PDDatos* data);
		private:
			PDCadena var;
	};
}

#endif
