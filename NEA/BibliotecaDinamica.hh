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
			virtual ~PseudoLlamar();
			/**
			* @brief Lee los parametros del flujo.
			* @arg in Flujo del cual leer.
			*/
			void LeerParametros(std::istream& in);
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
			virtual ~PseudoLlamarOO();
			/**
			* @brief Lee los parametros del flujo.
			* @arg in Flujo del cual leer.
			*/
			void LeerParametros(std::istream& in);
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
