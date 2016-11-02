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

#ifndef __PDVARIANTE_PSEUDOD_PDDATA_H__
#define __PDVARIANTE_PSEUDOD_PDDATA_H__

#include <iostream>
#include <cstddef>
#include <memory>

#include "data.hh"

/**
* @file variante.hh
* Contiene la clase PDvar::Variante la cual representa el valor
* de una variable.
*/

/**
* @brief Espacio de nombres principal de PseudoD.
*
* Posee las clases PDDatos y PDObjeto.
*/
namespace PDvar
{
	class Variante
	{
		public:
			/*
			** Siempre tipo_cadena, tipo_entero y tipo_real se pasan por referencia
			** para evitar la copiar de estas variables, debido a que son valores
			** grandes, la copia puede realentizar la ejecución del código y crear
			** una carga innecesaria en memoria.
			**
			** Discriminante también se pasa por referencia, para ser consistentes.
			*/
			typedef PDCadena tipo_cadena;
			typedef PDEntero tipo_entero;
			typedef PDDecimal tipo_real;

			enum Discriminante
			{
				TipoCadena,
				TipoEntero,
				TipoReal,
				SinTipo
			};

			Variante(void);
			Variante(const tipo_cadena&);
			Variante(const tipo_entero&);
			Variante(const tipo_real&);
			Variante(const Variante&);
			Variante(const Discriminante&);
			Variante(Variante&&);
			virtual ~Variante(void);

			operator tipo_cadena(void) const;
			operator tipo_entero(void) const;
			operator tipo_real(void) const;
			operator Discriminante(void) const;
			operator bool(void) const;

			Variante& operator=(const Variante&);
			Variante& operator=(const tipo_cadena&);
			Variante& operator=(const tipo_entero&);
			Variante& operator=(const tipo_real&);
			Variante& operator=(const Discriminante&);

			// NOTA: Estas funciones comparan valor, NO IDENTIDAD
			bool operator==(const Variante&) const;
			bool operator==(const tipo_cadena&) const;
			bool operator==(const tipo_entero&) const;
			bool operator==(const tipo_real&) const;
			bool operator==(const Discriminante&) const;

			// NOTA: Estas funciones comparan valor, NO IDENTIDAD
			bool operator!=(const Variante&) const;
			bool operator!=(const tipo_cadena&) const;
			bool operator!=(const tipo_entero&) const;
			bool operator!=(const tipo_real&) const;
			bool operator!=(const Discriminante&) const;

			// NOTA: Estas funciones comparan valor, NO IDENTIDAD
			bool EsIgualA(const Variante&) const;
			bool EsIgualA(const tipo_cadena&) const;
			bool EsIgualA(const tipo_entero&) const;
			bool EsIgualA(const tipo_real&) const;
			bool EsIgualA(const Discriminante&) const;

			const Discriminante& ObtenerTipo(void) const;
			const tipo_cadena& ObtenerCadena(void) const;
			const tipo_entero& ObtenerEntero(void) const;
			const tipo_real& ObtenerReal(void) const;

			bool PoseeTipo(void) const;

			void FijarTipo(const Discriminante&);
			void FijarCadena(const tipo_cadena&);
			void FijarEntero(const tipo_entero&);
			void FijarReal(const tipo_real&);
			void Fijar(const Variante&);

			// NOTA: Estas funciones comparar identidad, NO VALOR
			bool EsMismo(const Variante&) const;
			bool EsMismo(const tipo_cadena&) const;
			bool EsMismo(const tipo_entero&) const;
			bool EsMismo(const tipo_real&) const;
		protected:
			void*& ObtenerPuntero(void);
			Discriminante& ObtenerDiscriminante(void);
			void DestruirMemoria(void);
		private:
			Discriminante tipo;
			void* valor;
	};
}

#endif /* ~__PDVARIANTE_PSEUDOD_PDDATA_H__ */
