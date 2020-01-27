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

#include "nmemoic.hh"

static std::multimap<PDCadena, pseudod::NMemonico::Palabra> ConversorS2P =
{
	{"adquirir", pseudod::NMemonico::PD_ADQUIRIR},
	{"variable", pseudod::NMemonico::PD_ADQUIRIR},
	{"variables", pseudod::NMemonico::PD_ADQUIRIR},
	{"puntero", pseudod::NMemonico::PD_PUNTERO},
	{"liberar", pseudod::NMemonico::PD_LIBERAR},
	{"instancia", pseudod::NMemonico::PD_INSTANCIA},
	{"clase", pseudod::NMemonico::PD_CLASE},
	{"estructura", pseudod::NMemonico::PD_CLASE},
	{"heredar", pseudod::NMemonico::PD_HEREDAR},
	{"hereda", pseudod::NMemonico::PD_HEREDAR},
	{"extiende", pseudod::NMemonico::PD_HEREDAR},
	{"implementa", pseudod::NMemonico::PD_IMPLEMENTAR},
	{"redireccionar", pseudod::NMemonico::PD_REDIRECCIONAR},
	{"mientras", pseudod::NMemonico::PD_MIENTRAS},
	{"incrementar_p", pseudod::NMemonico::PD_INCREMENTAR_PUNTERO},
	{"decrementar_p", pseudod::NMemonico::PD_DECREMENTAR_PUNTERO},
	{"incrementar_puntero", pseudod::NMemonico::PD_INCREMENTAR_PUNTERO},
	{"decrementar_puntero", pseudod::NMemonico::PD_DECREMENTAR_PUNTERO},
	{"escribir", pseudod::NMemonico::PD_ESCRIBIR},
	{"ejecutar", pseudod::NMemonico::PD_EJECUTAR},
	{"nl", pseudod::NMemonico::PD_NUEVALINEA},
	{"oper", pseudod::NMemonico::PD_FIJAR},
	{"operador", pseudod::NMemonico::PD_FIJAR},
	{"fijar", pseudod::NMemonico::PD_FIJAR},
	{"a", pseudod::NMemonico::PD_OPERADOR_A},
	{"=?", pseudod::NMemonico::PD_OPERADOR_IGUAL},
	{"=¿?", pseudod::NMemonico::PD_OPERADOR_IGUAL},
	{"=*", pseudod::NMemonico::PD_OPERADOR_FIJAR},
	{"son", pseudod::NMemonico::PD_OPERADOR_SON},
	{"sean", pseudod::NMemonico::PD_OPERADOR_SON},
	{"iguales", pseudod::NMemonico::PD_OPERADOR_IGUALES},
	{"diferentes", pseudod::NMemonico::PD_OPERADOR_DIFERENTES},
	{"y", pseudod::NMemonico::PD_OPERADOR_Y},
	{"e", pseudod::NMemonico::PD_OPERADOR_Y},
	{"comparar", pseudod::NMemonico::PD_OPERADOR_COMPARAR},
	{"no", pseudod::NMemonico::PD_OPERADOR_NO},
	{"ejecutar", pseudod::NMemonico::PD_OPERADOR_EJECUTAR},
	{"¿son_iguales?", pseudod::NMemonico::PD_OPERADOR_SON_IGUALES},
	{"tanto", pseudod::NMemonico::PD_OPERADOR_TANTO},
	{"como", pseudod::NMemonico::PD_OPERADOR_COMO},
	{"y", pseudod::NMemonico::PD_OPERADOR_COMO},
	{"e", pseudod::NMemonico::PD_OPERADOR_COMO},
	{"algun", pseudod::NMemonico::PD_OPERADOR_ALGUN},
	{"o", pseudod::NMemonico::PD_OPERADOR_O},
	{"u", pseudod::NMemonico::PD_OPERADOR_O},
	{"leer", pseudod::NMemonico::PD_LEER},
	{"utilizar", pseudod::NMemonico::PD_UTILIZAR},
	{"llamar", pseudod::NMemonico::PD_LLAMAR},
	{"y", pseudod::NMemonico::PD_SEPARADOR_DE_ARGUMENTOS},
	{"e", pseudod::NMemonico::PD_SEPARADOR_DE_ARGUMENTOS},
	{"o", pseudod::NMemonico::PD_SEPARADOR_DE_ARGUMENTOS},
	{"u", pseudod::NMemonico::PD_SEPARADOR_DE_ARGUMENTOS},
	{",", pseudod::NMemonico::PD_SEPARADOR_DE_ARGUMENTOS},
	{"de", pseudod::NMemonico::PD_CON},
	{"con", pseudod::NMemonico::PD_CON},
	{"funcion", pseudod::NMemonico::PD_FUNCION},
	{"procedimiento", pseudod::NMemonico::PD_FUNCION},
	{"metodo", pseudod::NMemonico::PD_FUNCION},
	{"finfun", pseudod::NMemonico::PD_FIN_FUNCION},
	{"finfuncion", pseudod::NMemonico::PD_FIN_FUNCION},
	{"finmetodo", pseudod::NMemonico::PD_FIN_FUNCION},
	{"finprocedimiento", pseudod::NMemonico::PD_FIN_FUNCION},
	{"fin", pseudod::NMemonico::PD_FIN_SI},
	{"finsi", pseudod::NMemonico::PD_FIN_SI},
	{"finbucle", pseudod::NMemonico::PD_FIN_BUCLE},
	{"finmientras", pseudod::NMemonico::PD_FIN_BUCLE},
	{"empujar", pseudod::NMemonico::PD_EMPUJAR},
	{"devolver", pseudod::NMemonico::PD_EMPUJAR},
	{"enviar_parametro", pseudod::NMemonico::PD_EMPUJAR},
	{"devolver", pseudod::NMemonico::PD_DEVOLVER},
	{"sacar", pseudod::NMemonico::PD_SACAR},
	{"recibir_parametro", pseudod::NMemonico::PD_SACAR},
	{"recibir_resultado", pseudod::NMemonico::PD_SACAR},
	{"usar_pila", pseudod::NMemonico::PD_USAR_PILA},
	{"crear_pila", pseudod::NMemonico::PD_CREAR_PILA},
	{"necesitas", pseudod::NMemonico::PD_NECESITAS},
	{"si", pseudod::NMemonico::PD_SI},
	{"sino", pseudod::NMemonico::PD_SINO},
	{"si_no", pseudod::NMemonico::PD_SI_NO},
	{"comparar_i", pseudod::NMemonico::PD_SON_IGUALES},
	{"¿son_iguales?", pseudod::NMemonico::PD_SON_IGUALES},
	{"escribir_esp", pseudod::NMemonico::PD_ESCRIBIR_ESPACIO},
	{"sal", pseudod::NMemonico::PD_SALIR},
	{"salir", pseudod::NMemonico::PD_SALIR},
	{"intenta", pseudod::NMemonico::PD_INTENTA},
	{"atrapar", pseudod::NMemonico::PD_ATRAPA_ERROR},
	{"finintenta", pseudod::NMemonico::PD_FIN_INTENTA},
	{"atributo", pseudod::NMemonico::PD_CLASE_ATRIBUTO},
	{"atributos", pseudod::NMemonico::PD_CLASE_ATRIBUTO},
	{"puntero", pseudod::NMemonico::PD_CLASE_PUNTERO},
	{"estatico", pseudod::NMemonico::PD_CLASE_METODO_ESTATICO},
	{"metodo", pseudod::NMemonico::PD_CLASE_METODO},
	{"finargs", pseudod::NMemonico::PD_FIN_ARGUMENTOS},
	{"#(Final).", pseudod::NMemonico::PD_FIN_ARGUMENTOS},
	{"finclase", pseudod::NMemonico::PD_FIN_CLASE},
	{"abstracta", pseudod::NMemonico::PD_CLASE_ABSTRACTA},
	{"( NEA )", pseudod::NMemonico::PD_NEA},
	//{"menor", pseudod::NMemonico::PD_COMPARAR_MENOR},
	{"<", pseudod::NMemonico::PD_COMPARAR_MENOR},
	//{"menor_o_igual", pseudod::NMemonico::PD_COMPARAR_MENOR_O_IGUAL},
	//{"menoroigual", pseudod::NMemonico::PD_COMPARAR_MENOR_O_IGUAL},
	{"<=", pseudod::NMemonico::PD_COMPARAR_MENOR_O_IGUAL},
	//{"mayor", pseudod::NMemonico::PD_COMPARAR_MAYOR},
	{">", pseudod::NMemonico::PD_COMPARAR_MAYOR},
	//{"mayor_o_igual", pseudod::NMemonico::PD_COMPARAR_MAYOR_O_IGUAL},
	//{"mayoroigual", pseudod::NMemonico::PD_COMPARAR_MAYOR_O_IGUAL},
	{">=", pseudod::NMemonico::PD_COMPARAR_MAYOR_O_IGUAL},
	//{"igual", pseudod::NMemonico::PD_COMPARAR_IGUAL},
	{"=", pseudod::NMemonico::PD_COMPARAR_IGUAL},
	{"==", pseudod::NMemonico::PD_COMPARAR_IGUAL},
	//{"distinto", pseudod::NMemonico::PD_COMPARAR_DISTINTO},
	{"<>", pseudod::NMemonico::PD_COMPARAR_DISTINTO},
	{"!=", pseudod::NMemonico::PD_COMPARAR_DISTINTO},
	{"<?", pseudod::NMemonico::PD_COMPARAR_MINIMO},
	{"?>", pseudod::NMemonico::PD_COMPARAR_MAXIMO},
	{"es", pseudod::NMemonico::PD_OPERADOR_ES},
	{"a", pseudod::NMemonico::PD_OPERADOR_REDIRECCIONAR_A},
	{"#", pseudod::NMemonico::PD_ENVIAR_MENSAJE},
	{":", pseudod::NMemonico::PD_OPERADOR_LLAMAR},
	{"&", pseudod::NMemonico::PD_REFERENCIA_VARIABLE},
	{"%", pseudod::NMemonico::PD_AUTOEJECUTA_VARIABLE},
	{"ref", pseudod::NMemonico::PD_REFERENCIAR},
	{"desref", pseudod::NMemonico::PD_DESREFERENCIAR},
	{"(", pseudod::NMemonico::PD_PARENTESIS_IZQUIERDO},
	{")", pseudod::NMemonico::PD_PARENTESIS_DERECHO}
};

static std::multimap<pseudod::NMemonico::Palabra, PDCadena> ConversorP2S;
static bool CreadoConversor = false;

static inline void IntentaCrearConversor(void)
{
	if(CreadoConversor)
		return;
	for(auto i = ConversorS2P.begin(); i != ConversorS2P.end(); i++)
	{
		ConversorP2S.insert(std::make_pair(i->second, i->first));
	}
	CreadoConversor = true;
}

namespace pseudod
{
	NMemonico::NMemonico(void) : valor(NMemonico::PD_OTRO) {}
	NMemonico::NMemonico(NMemonico::Palabra otro) : valor(otro) {}
	NMemonico::NMemonico(const NMemonico& otro) : valor(otro.ObtenerValor()) {}
	NMemonico::NMemonico(NMemonico&& otro) : valor(otro.ObtenerValor())
	{
		otro.valor = NMemonico::PD_OTRO;
	}
	NMemonico::~NMemonico(void) {}
	NMemonico::operator NMemonico::Palabra(void)
	{
		return this->valor;
	}
	NMemonico::operator std::string(void)
	{
		IntentaCrearConversor();
		return ConversorP2S.find(this->valor)->second;
	}
	bool NMemonico::operator==(const NMemonico& otro)
	{
		return this->valor == otro.valor;
	}
	bool NMemonico::operator==(NMemonico::Palabra otro)
	{
		return this->valor != otro;
	}
	bool NMemonico::operator!=(const NMemonico& otro)
	{
		return !(*this == otro);
	}
	bool NMemonico::operator!=(NMemonico::Palabra otro)
	{
		return !(*this == otro);
	}
	// Los siguientes metodos son los mismos que los superiores,
	// solo que constantes.
	bool NMemonico::operator==(const NMemonico& otro) const
	{
		return this->valor == otro.valor;
	}
	bool NMemonico::operator==(NMemonico::Palabra otro) const
	{
		return this->valor != otro;
	}
	bool NMemonico::operator!=(const NMemonico& otro) const
	{
		return !(*this == otro);
	}
	bool NMemonico::operator!=(NMemonico::Palabra otro) const
	{
		return !(*this == otro);
	}
	NMemonico& NMemonico::operator=(const NMemonico& otro)
	{
		this->valor = otro.valor;
		return *this;
	}
	NMemonico& NMemonico::operator=(NMemonico::Palabra otro)
	{
		this->valor = otro;
		return *this;
	}
	NMemonico& NMemonico::operator=(NMemonico&& otro)
	{
		this->valor = otro.valor;
		otro.valor = NMemonico::PD_OTRO;
		return *this;
	}
	// Obtener valor:
	NMemonico::Palabra& NMemonico::ObtenerValor(void)
	{
		return this->valor;
	}
	NMemonico::Palabra NMemonico::ObtenerValor(void) const
	{
		return this->valor;
	}

	// Proxy:
	NMemonicoProxy::operator PDCadena(void)
	{
		return this->original;
	}
	bool NMemonicoProxy::operator==(NMemonicoProxy otro) const
	{
		if(!this->matched)
		{
			return false;
		}

		NMemonicoProxy::iterator iter;

		for(iter = this->begin; iter != this->end; iter++)
		{
			if(otro == iter->second)
			{
				return true;
			}
		}

		return false;
	}
	bool NMemonicoProxy::operator!=(NMemonicoProxy otro) const
	{
		return !(*this == otro);
	}
	bool NMemonicoProxy::operator==(NMemonico otro) const
	{
		if(!this->matched)
		{
			return false;
		}

		NMemonicoProxy::iterator iter;
		for(iter = this->begin; iter != this->end; iter++)
		{
			if(iter->second == otro.ObtenerValor())
			{
				return true;
			}
		}
		return false;
	}
	bool NMemonicoProxy::operator!=(NMemonico otro) const
	{
		return !(*this == otro);
	}
	bool NMemonicoProxy::operator==(NMemonico::Palabra otro) const
	{
		if(!this->matched)
		{
			return false;
		}

		NMemonicoProxy::iterator iter;
		for(iter = this->begin; iter != this->end; iter++)
		{
			if(iter->second == otro)
			{
				return true;
			}
		}
		return false;
	}
	bool NMemonicoProxy::operator!=(NMemonico::Palabra otro) const
	{
		return !(*this == otro);
	}

	NMemonicoProxy ConvertirCadenaANMemonico(PDCadena in)
	{
		auto values = ConversorS2P.equal_range(in);
		// decltype(values) =
		//    std::pair<decltype(ConversorS2P)::iterator (begin),
		//              decltype(ConversorS2P)::iterator (end)>

		// values es un iterador sobre todos los nmemonicos que concuerdan
		// con "val"
		// Sin embargo, debido al funcionamiento de los nmemonicos no
		// podemos determinar a exactitud cual es:
		// > Podemos determinar que cadena le pertenece a un nmemonico,
		// > más no que nmemonico le pertenece a una cadena: esto es
		// > debido a que los nmemonicos comparten cadenas (existen
		// > dos nmemonicos con simbolos distintos y cadenas iguales)
		// > y cuya diferencia es únicamente semantica y no sintáctica.
		// Por ello, debemos devolver todos los posibles nmemonicos y
		// que el parser compare si entre ellos está el que busca.

		// Note que debido a la naturaleza dinámica del NEA, todas las palabras
		// que comiencen por Importar serán tratadas como instrucciones del NEA.

		PDCadena imp = "Importar.";

		NMemonicoProxy proxy;

		proxy.original = in;
		proxy.rlend = ConversorS2P.end();
		proxy.begin = values.first;
		proxy.end = values.second;

		if((in.size() > imp.size()) && (in.find(imp.c_str()) == 0))
		{
			values = ConversorS2P.equal_range("( NEA )");

			proxy.begin = values.first;
			proxy.end = values.second;
			proxy.matched = true;

			return proxy;
		}

		proxy.matched = (ConversorS2P.count(in) > 0);

		return proxy;
	}

	NMemonicoProxy ConvertirNMemonicoAProxy(NMemonico mem)
	{
		return ConvertirPalabraAProxy(mem.ObtenerValor());
	}

	NMemonicoProxy ConvertirPalabraAProxy(NMemonico::Palabra pal)
	{
		// Esta **garantizado** que no importa cual valor devuelva
		// ConversorP2S.find(pal), el resultado de
		// ConvertirCadenaANMemonico(ConversorP2S.find(pal)) **siempre**
		// será igual a pal.

		IntentaCrearConversor();

		return ConvertirCadenaANMemonico(ConversorP2S.find(pal)->second);
	}

	std::istream& operator>>(std::istream& in, NMemonicoProxy& res)
	{
		std::string val = "";
		in >> val;
		res = ConvertirCadenaANMemonico(val);
		return in;
	}
	std::ostream& operator<<(std::ostream& out, NMemonico res)
	{
		IntentaCrearConversor();
		out << ConversorP2S.find(res)->second;
		return out;
	}
}
