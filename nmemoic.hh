/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*** Este es PseudoD version 2.1.0                                      *****
*** Log de este archivo:                                               *****
*** Formato: DD/MM/YYYY: txt                                           *****
*** **** 17/09/2016: Se creo el archivo.                               *****
****************************************************************************
**************************************************************************/

#ifndef __NMEMOICOS_PSEUDOD_H__
#define __NMEMOICOS_PSEUDOD_H__

#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <string>
#include <map>
#include <unordered_map>
#include <exception>
#include <dlfcn.h>

#include "NEA/PDData.hh"

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
				PD_OPERADOR_A, // fijar X a Y
				PD_OPERADOR_IGUAL, // fijar X =?/=¿? Y
				PD_OPERADOR_FIJAR, // fijar X =* Y
				PD_OPERADOR_SON, // son/sean
				PD_OPERADOR_IGUALES, // iguales
				PD_OPERADOR_DIFERENTES, // diferentes
				PD_OPERADOR_Y, // y
				PD_OPERADOR_COMPARAR, // comparar
				PD_OPERADOR_NO, // no
				PD_OPERADOR_EJECUTAR, // ejecutar
				PD_OPERADOR_SON_IGUALES, // ¿son_iguales?
				PD_LEER, // leer
				PD_UTILIZAR, // utilizar
				PD_LLAMAR, // llamar
				PD_FUNCION, // funcion
				PD_FIN_FUNCION, // finfun
				PD_FIN_SI, // fin
				PD_FIN_BUCLE, // finbucle
				PD_EMPUJAR, // empujar/devolver/enviar_parametro
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
				PD_NEA, // Importar.X.Y
				PD_ALIAS, // Aliases
				PD_OTRO // ???
			};

			NMemonico(void);
			NMemonico(Palabra otro);
			NMemonico(const NMemonico& otro);
			NMemonico(NMemonico&& otro);
			virtual ~NMemonico(void);

			// Operadores

			operator Palabra(void); // Convertir a ENUM
			bool operator==(const NMemonico& otro); // Comparar desde otro
			bool operator==(Palabra otro); // Comparar a ENUM
			bool operator==(const NMemonico& otro) const; // Comparar desde otro
			bool operator==(Palabra otro) const; // Comparar a ENUM
			NMemonico& operator=(const NMemonico& otro); // Fijar desde otro
			NMemonico& operator=(Palabra otro); // Fijar a otro

			Palabra& ObtenerValor(void);
			Palabra ObtenerValor(void) const;
		private:
			Palabra valor;
	};

	std::istream& operator>>(std::istream& in, NMemonico& res);
	std::ostream& operator<<(std::ostream& out, NMemonico res);
}

#endif /* __NMEMOICOS_PSEUDOD_H__ */

