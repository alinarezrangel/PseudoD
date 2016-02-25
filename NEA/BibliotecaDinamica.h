/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*** Este es PseudoD version 2.0.0                                      *****
*** Log de este archivo:                                               *****
*** Formato: DD/MM/YYYY: txt                                           *****
*** **** 14/09/2014: Se creo el archivo.                               *****
****************************************************************************
**************************************************************************/


#ifndef __BIBLIOTECADINAMICO_H__
#define __BIBLIOTECADINAMICO_H__
/**
* @brief Aqui se encuentran todos lo referente a bibliotecas de carga dinamica.
*/
namespace BibliotecaDinamica
{
	using namespace PDvar;
	using namespace PDTipos;
	
	/**
	* @brief Llama un modulo dinamico
	*/
	class PseudoLlamar : public PDInstancia
	{
		public:
			/**
			* @brief Inicializa la instancia.
			* @arg var Nombre de la instancia de BibliotecaDinamica
			* @arg args Argumentos a pasar a la biblioteca
			*/
			PseudoLlamar(string var = "____", vector<string> args = vector<string>());
			/**
			* @brief Destruye la instancia.
			*/
			~PseudoLlamar();
			/**
			* @brief Lee los parametros del flujo.
			* @arg in Flujo del cual leer.
			*/
			void LeerParametros(istream& in);
			/**
			* @brief Ejecuta el llamado a la biblioteca dinamica.
			* @arg data Memoria del interprete.
			*/
			void InscribirInstancia(PDDatos* data);
		private:
			string var;
			vector<string> param;
	};
}

#endif /* __BIBLIOTECADINAMICO_H__ */

