/*
**PseudoD 1.5.0
**Creado por Alejandro Linarez Rangel
*/
#ifndef __BIBLIOTECADINAMICO_H__
#define __BIBLIOTECADINAMICO_H__

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
			PseudoLlamar(string var = "____");
			~PseudoLlamar();
			void LeerParametros(istream& in);
			void InscribirInstancia(PDDatos* data);
		private:
			string var;
			vector<string> param;
	};
}

#endif /* __BIBLIOTECADINAMICO_H__ */

