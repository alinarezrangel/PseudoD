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

#ifndef HDR_PSEUDOD_NIA_NMEMOIC_HH
#define HDR_PSEUDOD_NIA_NMEMOIC_HH 1

#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <string>
#include <map>
#include <unordered_map>
#include <exception>
#include <algorithm>
#include <dlfcn.h>

#include "NEA/interno/data.hh"

// #include "NEA/PDData.hh"

/**
* @file nmemoic.hh
* Contiene la clase pseudod::NMemonico, que actua
* como instrucción para el núcleo.
*/

namespace pseudod
{
	/**
	* @brief Representa una instrucción.
	* Anteriormente, en PseudoD las instrucciones eran representadas por
	* cadenas de caracteres, pero debido a su gran coste de comparación,
	* se decidio aislar las ordenes en esta clase que utiliza métodos de
	* comparación avanzados y más eficientes.
	*/
	class NMemonico : public PDvar::PDObjeto
	{
		public:
			/**
			* @brief Conjunto de palabras del nmemonico.
			* Estas son todas las palabras que admite el nmemonico.
			*/
			enum Palabra
			{
				PD_ADQUIRIR, // adquirir
				PD_PUNTERO, // puntero
				PD_LIBERAR, // liberar (alias)
				PD_INSTANCIA, // instancia (alias)
				PD_CLASE, // clase (alias)
				PD_HEREDAR, // heredar (alias)
				PD_REDIRECCIONAR, // redireccionar (alias)
				PD_MIENTRAS, // mientras (alias)
				PD_INCREMENTAR_PUNTERO, // incrementar_p(untero)?
				PD_DECREMENTAR_PUNTERO, // decrementar_p(untero)?
				PD_ESCRIBIR, // escribir
				PD_EJECUTAR, // ejecutar
				PD_NUEVALINEA, // nl
				PD_FIJAR, // fijar/oper(ador)?
				PD_CLONAR, // clonar
				PD_CLONAR_CON, // clonar <> con
				PD_FIN_CLONAR, // finclonar
				PD_OPERADOR_A, // fijar X a Y
				PD_OPERADOR_IGUAL, // fijar X =?/=¿? Y
				PD_OPERADOR_FIJAR, // fijar X =* Y
				PD_OPERADOR_SON, // son/sean
				PD_OPERADOR_IGUALES, // iguales
				PD_OPERADOR_DIFERENTES, // diferentes
				PD_OPERADOR_Y, // y/e
				PD_OPERADOR_COMPARAR, // comparar
				PD_OPERADOR_NO, // no
				PD_OPERADOR_EJECUTAR, // ejecutar
				PD_OPERADOR_SON_IGUALES, // ¿son_iguales?
				PD_OPERADOR_TANTO, // tanto X [como] Y ...
				PD_OPERADOR_COMO, // [tanto] X como/y/e Y ...
				PD_OPERADOR_ALGUN, // algun X [o] Y ...
				PD_OPERADOR_O, // [algun] X o/u Y
				PD_LEER, // leer
				PD_UTILIZAR, // utilizar
				PD_LLAMAR, // llamar
				PD_SEPARADOR_DE_ARGUMENTOS, // y/e/o/u
				PD_CON, // con|de
				PD_FUNCION, // funcion
				PD_FIN_FUNCION, // finfun
				PD_FIN_SI, // fin
				PD_FIN_BUCLE, // finbucle
				PD_FIN_CLASE, // finclase
				PD_EMPUJAR, // empujar/devolver/enviar_parametro
				PD_DEVOLVER, // devolver
				PD_SACAR, // sacar/recibir_parametro/recibir_resultado
				PD_USAR_PILA, // usar_pila
				PD_CREAR_PILA, // crear_pila
				PD_NECESITAS, // necesitas
				PD_SI, // si
				PD_SINO, // sino
				PD_SI_NO, // si_no
				PD_SON_IGUALES, // comparar_i/¿son_iguales?
				PD_ESCRIBIR_ESPACIO, // escribir_esp
				PD_SALIR, // sal(ir)?
				PD_COMENTARIO, // [ ... ]
				PD_INTENTA, // intenta
				PD_ATRAPA_ERROR, // error
				PD_FIN_INTENTA, // finintenta
				PD_IMPLEMENTAR, // clase X implementa Y
				PD_CLASE_ATRIBUTO, // atributo X
				PD_CLASE_METODO, // metodo X
				PD_CLASE_PUNTERO, // puntero X
				PD_CLASE_METODO_ESTATICO, // metodo estatico X
				PD_FIN_ARGUMENTOS, // #(Final).|finargs
				PD_COMPARAR_IGUAL, // =/==
				PD_COMPARAR_MENOR, // <
				PD_COMPARAR_MENOR_O_IGUAL, // <=
				PD_COMPARAR_MAYOR, // >
				PD_COMPARAR_MAYOR_O_IGUAL, // >=
				PD_COMPARAR_DISTINTO, // <>, !=
				PD_COMPARAR_MINIMO, // <?
				PD_COMPARAR_MAXIMO, //?>
				PD_OPERADOR_ES, // es
				PD_CLASE_ABSTRACTA, // abstracta
				PD_OPERADOR_REDIRECCIONAR_A, // a
				PD_ENVIAR_MENSAJE, // #
				PD_OPERADOR_LLAMAR, // :
				PD_PARENTESIS_IZQUIERDO, // (
				PD_PARENTESIS_DERECHO, // )
				PD_REFERENCIA_VARIABLE, // &
				PD_AUTOEJECUTA_VARIABLE, // %
				PD_REFERENCIAR, // ref
				PD_DESREFERENCIAR, // desref
				PD_PUNTO, // .
				PD_OPERADOR_GENERAL,
				PD_NEA, // Importar.X.Y
				PD_ALIAS, // Aliases
				PD_OTRO // ???
			};

			/**
			* @brief Crea un nuevo nmemonico.
			* La palabra predeterminada es PD_OTRO.
			*/
			NMemonico(void);
			/**
			* @brief Crea un nuevo nmemonico.
			* Utiliza la palabra provista.
			* @param otro Palabra a utilizar.
			*/
			NMemonico(Palabra otro);
			/**
			* @brief Crea un nuevo nmemonico.
			* Copia todos los datos desde el otro.
			* @param otro NMemonico a copiar.
			*/
			NMemonico(const NMemonico& otro);
			/**
			* @brief Mueve un nmemonico.
			* Mueve todos los datos desde el otro nmemonico.
			* @param otro Fuente desde la cual se moverán los datos.
			*/
			NMemonico(NMemonico&& otro);
			/**
			* @brief Destruye el nmemonico.
			*/
			virtual ~NMemonico(void);

			// Operadores

			/**
			* @brief Convierte este nmemonico a una palabra.
			* Realmente devuelve la palabra interna.
			*/
			operator Palabra(void); // Convertir a ENUM
			/**
			* @brief Convierte este nmemonico a una cadena.
			*/
			operator std::string(void); // Convertir a ENUM
			/**
			* @brief Compara si los dos nmemonicos son iguales.
			* @param otro Otro nmemonico a comparar.
			*/
			bool operator==(const NMemonico& otro); // Comparar desde otro
			/**
			* @brief Comparar si las dos palabras son iguales.
			* @param otro Otra palabra a comparar.
			*/
			bool operator==(Palabra otro); // Comparar a ENUM
			/**
			* @brief Compara si los dos nmemonicos son diferentes.
			* @param otro Otro nmemonico a comparar.
			*/
			bool operator!=(const NMemonico& otro); // Comparar desde otro
			/**
			* @brief Comparar si las dos palabras son diferentes.
			* @param otro Otra palabra a comparar.
			*/
			bool operator!=(Palabra otro); // Comparar a ENUM
			/**
			* @brief Versión constante de operator==
			* @param otro Otro valor a comparar.
			*/
			bool operator==(const NMemonico& otro) const; // Comparar desde otro
			/**
			* @brief Versión constante de operator==
			* @param otro Otro valor a comparar.
			*/
			bool operator==(Palabra otro) const; // Comparar a ENUM
			/**
			* @brief Versión constante de operator!=
			* @param otro Otro valor a comparar.
			*/
			bool operator!=(const NMemonico& otro) const; // Comparar desde otro
			/**
			* @brief Versión constante de operator!=
			* @param otro Otro valor a comparar.
			*/
			bool operator!=(Palabra otro) const; // Comparar a ENUM
			/**
			* @brief Asigna desde otro nmemonico.
			* Copia todos los datos desde el otro nmemonico hasta este.
			* @param otro NMemonico a copiar.
			*/
			NMemonico& operator=(const NMemonico& otro); // Fijar desde otro
			/**
			* @brief Mueve desde otro nmemonico.
			* Mueve todos los datos desde el otro nmemonico hasta este.
			* @param otro NMemonico a mover.
			*/
			NMemonico& operator=(NMemonico&& otro); // Mover desde otro
			/**
			* @brief Asigna otra palabra.
			* Cambia la palabra actual a la especificada.
			* @param otro Nueva palabra del nmemonico.
			*/
			NMemonico& operator=(Palabra otro); // Fijar a otro

			/**
			* @brief Obtinene una referencia a la palabra.
			* @return Referencia a la palabra de este nmemonico.
			*/
			Palabra& ObtenerValor(void);
			/**
			* @brief Obtinene la palabra.
			* @return La palabra de este nmemonico.
			*/
			Palabra ObtenerValor(void) const;
		private:
			Palabra valor;
	};

	/**
	* @brief Proxy para obtener nmemonicos.
	* Existen nmemonicos con el mismo simbolo sintactico
	* pero con distintos simbolos semánticos. En caso
	* que se lea o detecte uno de estos, se deben devolver
	* todos los simbolos semánticos y que el parser seleccione
	* el util. Este proxy permite realizar este tipo de operaciones
	* y es eficiente solamente en conjuntos pequeños.
	*
	* Todos los operadores de igualdad determinan si el valor
	* esta en el conjunto.
	*/
	struct NMemonicoProxy
	{
		typedef
			std::multimap<PDCadena, pseudod::NMemonico::Palabra>::iterator
			iterator;
		iterator begin;
		iterator end;
		iterator rlend;
		PDCadena original;
		bool matched;

		operator PDCadena(void);

		bool operator==(NMemonicoProxy otro) const;
		bool operator!=(NMemonicoProxy otro) const;
		bool operator==(NMemonico otro) const;
		bool operator!=(NMemonico otro) const;
		bool operator==(NMemonico::Palabra otro) const;
		bool operator!=(NMemonico::Palabra otro) const;
	};

	/**
	* @brief Convierte una cadena a un conjunto de nmemonicos.
	* El conjunto contiene todos los simbolos semánticos que
	* concuerdan con el simbolo sintáctico especificado.
	*
	* Si el simbolo semantico no existe, todas las comparaciones
	* sobre el proxy devolverán falso.
	*
	* @param in Simbolo sintáctico a convertir.
	* @return El conjunto de nmemonicos que concuerdan con la palabra.
	*/
	NMemonicoProxy ConvertirCadenaANMemonico(PDCadena in);

	/**
	* @brief Convierte un memonico a un conjunto de nmemonicos.
	* El conjunto contendrá únicamente este nmemonico.
	*
	* Equivalente a `ConvertirPalabraAProxy(mem.ObtenerValor())`.
	*
	* @param mem El nmemonico a convertir.
	* @return El conjunto de nmemonicos que contiene al nmemonico dado.
	*/
	NMemonicoProxy ConvertirNMemonicoAProxy(NMemonico mem);

	/**
	* @brief Convierte una palabra a un conjunto de nmemonicos.
	* El conjunto contendrá únicamente esta palabra.
	*
	* @param mem La palabra a convertir.
	* @return El conjunto de nmemonicos que contiene a la palabra dada.
	*/
	NMemonicoProxy ConvertirPalabraAProxy(NMemonico::Palabra pal);

	std::istream& operator>>(std::istream& in, NMemonicoProxy& res);
	std::ostream& operator<<(std::ostream& out, NMemonico res);
}

#endif
