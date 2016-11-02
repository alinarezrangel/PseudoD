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

#ifndef __TOKENIZADOR_FLUJO_PSEUDOD_H__
#define __TOKENIZADOR_FLUJO_PSEUDOD_H__

#include <iostream>

#include "nmemoic.hh"

/**
* @file token.hh
* Contiene la clase pseudod::tokenizador::Token la cual representa un token
* dentro del tokenizador.
*/

namespace pseudod
{
	/**
	* @brief Es espacio de nombres tokenizador, posee la interfaz para generar,
	* manipular, extender y utilizar el tokenizador de PseudoD.
	*/
	namespace tokenizador
	{
		/**
		* @brief La clase token representa un "token" o bloque sintáctico de
		* PseudoD.
		*
		* Un token se define como el mínimo elemento sintáctico en PseudoD, este
		* es, generalmente, una palabra, pero tambien puede ser una literal, un
		* comentario explícito o implícito o cualquier otro elemento.
		*
		* Cabe resaltar que la clase token no manipula la semántica de estos,
		* y es trabajo del parser determinar si un determinado token es correcto
		* semánticamente o no.
		*/
		class Token
		{
			public:
				/**
				* @brief Representa el valor interno de un token.
				* Debido a que un token puede ser un NMemonicoProxy, un NMemonico o
				* un PDvar::Variante (próximo) es necesario crear una estructura que
				* agrupe todos bajo la menor cantidad de memoria posible.
				*/
				struct ValorInterno
				{
					pseudod::NMemonicoProxy* nmemonicoProxy;
					PDCadena 
				};

				/**
				* @brief Crea un nuevo token.
				* Utiliza el Constructor predeterminado, este crea el token utilizando
				* únicamente valores por defecto, pero cualquier operación sobre el
				* token sin inicializar antes sus valores esta indefinida.
				*/
				explicit Token(void);

				/**
				* @brief Crea un nuevo token.
				* Utiliza las variables proporcionadas para construirlo.
				*
				* Recuerde que utilizar la clase NMemonico directamente esta
				* altamente desaprovado, utilize NMemonicoProxy en cambio.
				* @param nmem NMemonico utilizado para crear el token.
				*/
				explicit Token(pseudod::NMemonico nmem);

				/**
				* @brief Crea un nuevo token.
				* Utiliza las variables proporcionadas para construirlo.
				*
				* Recuerde que los NMemonicoProxy son siempre pasados por referencia
				* constante, debido a la cantidad de memoria que pueden llegar a
				* consumir.
				*
				* @param nmem NMemonicoProxy a utilizar para crear el token.
				*/
				explicit Token(const pseudod::NMemonicoProxy& nmem);

				/**
				* @brief Crea un nuevo token.
				* Este token es una copia del token pasado.
				* @param tok Token a copiar
				*/
				explicit Token(const Token& token);

				/**
				* @brief Crea un nuevo token.
				* Este token es una copia del token pasado, y el token pasado
				* se vuelve inválido (move constructor)
				* @param token Token a mover.
				*/
				explicit Token(Token&& token);

				operator pseudod::NMemonicoProxy(void);
			private:
				ValorInterno valor;
		};
	}
}

#endif /* ~__TOKENIZADOR_FLUJO_PSEUDOD_H__ */
