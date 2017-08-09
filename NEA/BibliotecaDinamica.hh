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

#ifndef __PSEUDOD_NEA_BIBLIOTECADINAMICA_HH__
#define __PSEUDOD_NEA_BIBLIOTECADINAMICA_HH__ 1

/**
* @brief Aqui se encuentran todos lo referente a bibliotecas de carga dinamica.
*/
namespace BibliotecaDinamica
{
	/**
	* @brief Llama un modulo dinamico
	*/
	class PseudoLlamar : public PDTipos::PDInstancia
	{
		public:
			/**
			* @brief Inicializa la instancia.
			* @arg var Nombre de la instancia de BibliotecaDinamica
			* @arg args Argumentos a pasar a la biblioteca
			*/
			PseudoLlamar(PDCadena var = "____", std::vector<PDCadena> args = std::vector<PDCadena>());
			/**
			* @brief Destruye la instancia.
			*/
			virtual ~PseudoLlamar(void);
			/**
			* @brief Lee los parametros del flujo.
			* @arg in Flujo del cual leer.
			*/
			void LeerParametros(pseudod::Tokenizador& in);
			/**
			* @brief Ejecuta el llamado a la biblioteca dinamica.
			* @arg data Memoria del interprete.
			*/
			void InscribirInstancia(PDvar::PDDatos* data);
		private:
			PDCadena var;
			std::vector<PDCadena> param;
	};

	/**
	* @brief Llama a un modulo dinamico orientado a objetos.
	*/
	class PseudoLlamarOO : public PDTipos::PDInstancia
	{
		public:
			/**
			* @brief Inicializa la instancia.
			* @arg var Nombre de la instancia de BibliotecaDinamica
			* @arg args Argumentos a pasar a la biblioteca
			*/
			PseudoLlamarOO(PDCadena var = "____", std::vector<PDCadena> args = std::vector<PDCadena>());
			/**
			* @brief Destruye la instancia.
			*/
			virtual ~PseudoLlamarOO(void);
			/**
			* @brief Lee los parametros del flujo.
			* @arg in Flujo del cual leer.
			*/
			void LeerParametros(pseudod::Tokenizador& in);
			/**
			* @brief Ejecuta el llamado a la biblioteca dinamica.
			* @arg data Memoria del interprete.
			*/
			void InscribirInstancia(PDvar::PDDatos* data);
		private:
			PDCadena var;
			std::vector<PDCadena> param;
	};
}

#endif
