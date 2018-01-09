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

#ifndef HDR_PSEUDOD_NEA_PDDATA_HH
#define HDR_PSEUDOD_NEA_PDDATA_HH 1

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
#include <stdexcept>
#include <algorithm>

#define PDVOID_INIC_VACIO char
#define PDVOID_INIC_VACIO_NULL ('a')

#include "interno/data.hh"
#include "interno/nmemoic.hh"
#include "interno/token.hh"
#include "interno/tokenizer.hh"

typedef void(*PDFuncionNEA)(pseudod::Token, pseudod::Tokenizador&);
typedef void (*PDFuncionNIA)(pseudod::Token, pseudod::Tokenizador&, PDFuncionNEA);

/**
* @brief Espacio de nombres principal de PseudoD.
*
* Posee las clases PDDatos y PDObjeto.
*/
namespace PDvar
{
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
			explicit PDDatos(
				std::vector<PDCadena>& nvar,
				std::vector<PDCadena>& vvar,
				std::vector<PDCadena>& npun,
				std::vector<int>& vpun,
				std::vector<std::stack<PDCadena>>& pil
			);
			/**
			* @brief inicia la instancia como Manejador principal de la memoria.
			* este tiene acceso a la memoria y la maneja de forma independiente.
			*/
			explicit PDDatos(void);
			/**
			* @brief Contruye la clase como con los vectores, pero desde otra instancia
			*/
			explicit PDDatos(PDDatos& otro);
			/**
			* @brief Destruye la clase
			*/
			virtual ~PDDatos(void);
		  /**
		  * @brief Es la constante que devuelven las funciones en caso de error.
		  */
			PDCadena ERROR;
			/**
			* @brief El valor de De Boole 1 o true en PseudoD
			*/
			PDCadena VERDADERO;
			/**
			* @brief El valor de De Boole 0 o false en PSeudoD
			*/
			PDCadena FALSO;
			/**
			* @brief Devuelve el valor de la variable o puntero.
			* @param n Nombre de la variable  puntero.
			*/
			PDCadena& ObtenerVariable(PDCadena n);
			/**
			* @brief Devuelve el valor del puntero.
			* @param n Nombre del puntero.
			*/
			PDCadena& ObtenerPuntero(PDCadena n);
			/**
			* @brief devuelve el indice al que apunte el puntero.
			* @param n nombre del puntero
			* @return Valor indice al que apunta
			*/
			int& ObtenerIndicePuntero(PDCadena n);
			/**
			* @brief Devuelve el tope de la pila.
			* @param p Numero de la pila
			* @return Tope de la pila
			*/
			PDCadena Tope(int p);
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
			PDCadena Sacar(int n);
			/**
			* @brief Empuja un valor en una pila
			* @param n Valor que se empujara
			* @param p Numero de la pila
			*/
			void Empujar(PDCadena n, int p);
			/**
			* @brief Crea una nueva pila
			*/
			void CrearPila(void);
			/**
			* @brief Determina si la pila esta vacía.
			* Sacar un valor de una pila vacía es un comportamiento indefinído en C++,
			* siempre verifica que esta función retorne "false" antes de sacar de
			* la pila.
			*/
			bool PilaVacia(int n);
			/**
			* @brief Crea una nueva variable o puntero
			* @param n Nombre de la nueva variable o puntero
			* @param t Tipo, true para crear una variable y false para un puntero
			* @param va Si es un puntero, direccion a la que apunta.
			* @param vl Se es una variable, se fija su valor a el valor de vl.
			*/
			void CrearVariable(PDCadena n, bool t = true, int va = 0, PDCadena vl = "nulo");
			/**
			* @brief Obtiene el indice de la variable o puntero
			* @param t tipo, true para una variable y false para un puntero
			* @param n Nombre a buscar
			* @return indice o -1 si no existe.
			*/
			int BuscarIndice(bool t, PDCadena n);
			/**
			* @brief Determina si existe la variable o puntero.
			* @param t tipo, true para una variable y false para un puntero
			* @param n nombre de la variable/puntero.
			* @return true si existe y false de lo contrario.
			*/
			bool ExisteVariable(bool t, PDCadena n);
			/**
			* @brief ejecuta una orden como interprete.
			* @param ord Orden a ejecutar.
			*/
			void Ejecutar(PDCadena ord);
			/**
			* @brief ejecuta una orden como interprete.
			* @param ord Orden a ejecutar.
			* @param in Flujo de tokens
			*/
			void Ejecutar(PDCadena ord, std::istream& in);
			/**
			* @brief ejecuta una orden como interprete.
			* @param ord Orden a ejecutar.
			* @param in Flujo de tokens
			*/
			void Ejecutar(PDCadena ord, pseudod::Tokenizador& in);
			/**
			* @brief ejecuta una orden como interprete.
			* @param ord Orden a ejecutar.
			* @param in Flujo de tokens
			*/
			void Ejecutar(pseudod::Token ord, pseudod::Tokenizador& in);
			/**
			* @brief Aplica la reducción de nombres sobre el nombre <v>.
			* Esta reducción resuelve punteros y referencias dentro del nombre.
			*
			* @param nombre Nombre a resolver
			* @return Nombre resuelto
			*/
			PDCadena ResolverNombre(PDCadena nombre);
		public:
			/**
			* @brief puntero a los nombres de variable
			*/
			std::vector<PDCadena>* nombrev;
			/**
			* @brief Puntero a los valores de las variables
			*/
			std::vector<PDCadena>* valorv;
			/**
			* @brief Puntero a los nombres de los punteros
			*/
			std::vector<PDCadena>* nombrep;
			/**
			* @brief Puntero a los valores-indice de los punteros.
			*/
			std::vector<int>*    nvapunt;
			/**
			* @brief Puntero a vector que contiene las pilas.
			*/
			std::vector<std::stack<PDCadena>>* pilas;
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
			PDFuncionNIA PROCESAR;
			/**
			* @brief Funcion hija, necesaria para la funcion madre
			* este puntero a funcion es una sub-funcion, la base del NEA. Se llama de la forma:
			* (*PROCESO)(orden,flujo);
			* Esto ejecuta orden como una orden que esta en el NEA
			*/
			PDFuncionNEA PROCESO;
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
			explicit PDEntradaBasica(PDCadena tok, std::istream& in, PDDatos& dat);
			/**
			* @brief Destruye la entrada.
			*/
			virtual ~PDEntradaBasica(void);
			/**
			* @brief Obtiene el flujo de tokens.
			* @return Flujo de tokens
			*/
			std::istream& ObtenerFlujo(void);
			/**
			* @brief Obtiene el puntero a el manejador de memoria del interprete.
			* @return Puntero al manejador.
			*/
			PDDatos* ObtenerMemoria(void);
			/**
			* @brief Obtiene el token actual.
			* @return Token actual.
			*/
			PDCadena ObtenerToken(void);
		private:
			std::istream* in; // flujo de tokens
			PDCadena token; // token actual
			PDDatos* data; // datos del interprete

			explicit PDEntradaBasica(std::vector<PDVOID_INIC_VACIO> args) : PDVoid(args) {}
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
	PDCadena ValorDelToken(pseudod::Token tok, pseudod::Tokenizador& in, PDDatos* data);

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
			PDCadena Instancia;
			PDCadena FuncionLlamada;
			std::vector<PDCadena>* Argumentos;
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
