/*
**PseudoD 1.5.0
**Creado por Alejandro Linarez Rangel
*/
#ifndef PDDATA_HEADER_H
#define PDDATA_HEADER_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <string>
#include <cstring>
#include <cstdio>

template<class T>
int buscar(std::vector<T> a,T b);
std::string eas(int i);
std::string dac(float i);
int cae(std::string i);
float caf(std::string i);

/**
* @brief Representa el tipo de datos nativo del interprete.
* 
* No es extendible.
*/
namespace PDvar
{
	using namespace std;
	
	/**
	* @brief Es la clase base para todas las clases de NEA.
	*/
	class PDObjeto
	{
		public:
			/**
			* @brief Constructor por defecto
			*/
			PDObjeto(){}
			/**
			* @brief Destructor
			*/
			virtual ~PDObjeto(){}
			/**
			* @brief Determina si el objeto actual y su parametro son la misma instancia
			* @param o La instancia que se va a comparar.
			* @return true si son la misma instancia y false de lo contrario.
			*/
			virtual bool MismoObjeto(PDObjeto& o){ return this == &o; }
			
	};
	
	/**
	* @brief Representa la clase base para todas las clases de parametros de PDCallBack.
	*/
	template<class T>
	class PDVoid : virtual public PDObjeto
	{
		public:
			/**
			* @brief Inicializador a base de vector
			*/
			explicit PDVoid(vector<T> args) : PDObjeto() {this->args = args;}
			/**
			* @brief Inicializador a base de objeto
			*/
			explicit PDVoid(T arg) : PDObjeto() {this.args.push_back(arg);}
			/**
			* @brief Destructor, no hace nada.
			*/
			~PDVoid(){}
			/**
			* @brief Devuelve el argumento
			* @param i numero de argumento
			* @return objeto
			*/
			T Obtener(int i = 0){return args[i];}
		private:
			vector<T> args;
	};
	
	/**
	* @brief Representa un objeto de retro-llamada.
	*/
	template<typename RV,typename AT>
	class PDCallBack : virtual public PDObjeto
	{
		public:
			/**
			* @brief Crea un nuevo call-back
			* @param F Puntero a funcion que se retro-llamara.
			*/
			PDCallBack (RV (*F)(AT));
			/**
			* @brief Destructor de PDCall-Back.
			*/
			~PDCallBack ();
			/**
			* @brief Llama a la funcion de retro-llamada.
			* @param p parametro que se le pasara a la funcion.
			* @return Resultado de la llamada
			*/
			RV Call(AT p);
			/**
			* @brief Llama a la funcion de retro-llamada con los parametros pre-establecidos
			* @return Resultado de la llamada
			*/
			RV Call();
			/**
			* @brief Fija los parametros preestablesidos.
			* @param v Nuevo parametro pre-establesido.
			*/
			void Set(AT v);
			/**
			* @brief Obtiene los parametros por defecto.
			* @return parametros por defecto
			*/
			AT Get();
		private:
			RV (** Fcn)(AT);
			AT param;
	};
	
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
			PDDatos(vector<string>& nvar,vector<string>& vvar,vector<string>& npun,vector<int>& vpun,vector< stack<string> >& pil);
			/**
			* @brief Destruye la clase
			*/
			~PDDatos();
		  /**
		  * @brief Es la constante que devuelven las funciones en caso de error.
		  */
			string ERROR;
			/**
			* @brief El valor de De Boole 1 o true en PseudoD
			*/
			string VERDADERO;
			/**
			* @brief El valor de De Boole 0 o false en PSeudoD
			*/
			string FALSO;
			/**
			* @brief Devuelve el valor de la variable o puntero.
			* @param n Nombre de la variable  puntero.
			*/
			string& ObtenerVariable(string n);
			/**
			* @brief Devuelve el valor del puntero.
			* @param n Nombre del puntero.
			*/
			string& ObtenerPuntero(string n);
			/**
			* @brief Devuelve el tope de la pila.
			* @param p Numero de la pila
			* @return Tope de la pila
			*/
			string Tope(int p);
			/**
			* @brief Borra el tope de una pila
			* @param p Numero de la pila
			*/
			void BorrarTope(int p);
			/**
			* @brief Empuja un valor en una pila
			* @param n Valor que se empujara
			* @param p Numero de la pila
			*/
			void Empujar(string n,int p);
			/**
			* @brief Crea una nueva pila
			*/
			void CrearPila();
			/**
			* @brief Crea una nueva variable o puntero
			* @param n Nombre de la nueva variable o puntero
			* @param t Tipo, "Variable" para crear una variable o "Puntero" para crear un puntero.
			* @param va Si es un puntero, direccion a la que apunta.
			*/
			void CrearVariable(string n,string t = "Variable", int va = 0);
			/**
			* @brief Obtiene el indice de la variable o puntero
			* @param t tipo, "Variable" para una variable y "Puntero" para un puntero.
			* @param n Nombre a buscar
			* @return indice
			*/
			int BuscarIndice(string t,string n);
		public:
			/**
			* @brief puntero a los nombres de variable
			*/
			vector<string>* nombrev;
			/**
			* @brief Puntero a los valores de las variables
			*/
			vector<string>* valorv;
			/**
			* @brief Puntero a los nombres de los punteros
			*/
			vector<string>* nombrep;
			/**
			* @brief Puntero a los valores-indice de los punteros.
			*/
			vector<int>*    nvapunt;
			/**
			* @brief Puntero a vector que contiene las pilas.
			*/
			vector< stack< string > >* pilas;
			/**
			* @brief Funcion madre, permite ser recursivo en el lenguaje
			* este puntero a funcion es utilizado para la recursion, se llama de la forma:
			* (*PROCESAR)(orden,flujo,PROCESO);
			* esto ejecuta orden como una orden global de PseudoD
			*/
			void (*PROCESAR)(string o,istream& e, void(*)(string,istream&));
			/**
			* @brief Funcion hija, necesaria para la funcion madre
			* este puntero a funcion es una sub-funcion, la base del NEA. Se llama de la forma:
			* (*PROCESO)(orden,flujo);
			* Esto ejecuta orden como una orden que esta en el NEA
			*/
			void (*PROCESO)(string o,istream& i);
	};
}

#endif
