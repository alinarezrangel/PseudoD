/*
**PseudoD 1.5.0
**Creado por Alejandro Linarez Rangel
*/
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

