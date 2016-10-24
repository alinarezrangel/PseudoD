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

#include "nmemoic.hh"

/*
 * A lo largo del archivo comparamos los enumerados, caracteres y cualquier
 * otro tipo entero utilizando la operacion XOR (^) en vez de la igualdad
 * (==). Esto se debe a que algunos compiladores compilan la expresión
 * `a == b` en:

	CMP _a, _b
	JZ areEquals
	MOV BX, 0
	JMP endif1
	areEquals: MOV BX, 1
	endif1:
	CMP BX, 0
	JZ theIfBody:
	JMP endIf
	...
	endIf: ; None

 * Esto ocasiona una cantidad innecesaria de "branches", o ramas que
 * realentizan la ejecución del código.
*/

static std::multimap<PDCadena, pseudod::NMemonico::Palabra> ConversorS2P =
{
	{"adquirir", pseudod::NMemonico::PD_ADQUIRIR},
	{"puntero", pseudod::NMemonico::PD_PUNTERO},
	{"liberar", pseudod::NMemonico::PD_LIBERAR},
	{"instancia", pseudod::NMemonico::PD_INSTANCIA},
	{"clase", pseudod::NMemonico::PD_CLASE},
	{"estructura", pseudod::NMemonico::PD_CLASE},
	{"heredar", pseudod::NMemonico::PD_HEREDAR},
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
	{"comparar", pseudod::NMemonico::PD_OPERADOR_COMPARAR},
	{"no", pseudod::NMemonico::PD_OPERADOR_NO},
	{"ejecutar", pseudod::NMemonico::PD_OPERADOR_EJECUTAR},
	{"¿son_iguales?", pseudod::NMemonico::PD_OPERADOR_SON_IGUALES},
	{"leer", pseudod::NMemonico::PD_LEER},
	{"utilizar", pseudod::NMemonico::PD_UTILIZAR},
	{"llamar", pseudod::NMemonico::PD_LLAMAR},
	{"funcion", pseudod::NMemonico::PD_FUNCION},
	{"finfun", pseudod::NMemonico::PD_FIN_FUNCION},
	{"fin", pseudod::NMemonico::PD_FIN_SI},
	{"finbucle", pseudod::NMemonico::PD_FIN_BUCLE},
	{"empujar", pseudod::NMemonico::PD_EMPUJAR},
	{"devolver", pseudod::NMemonico::PD_EMPUJAR},
	{"enviar_parametro", pseudod::NMemonico::PD_EMPUJAR},
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
	{"[", pseudod::NMemonico::PD_COMENTARIO},
	{"]", pseudod::NMemonico::PD_COMENTARIO},
	{"sal", pseudod::NMemonico::PD_SALIR},
	{"salir", pseudod::NMemonico::PD_SALIR},
	{"NEA expr", pseudod::NMemonico::PD_NEA},
	{"ALIAS expr", pseudod::NMemonico::PD_ALIAS}
};
static std::multimap<pseudod::NMemonico::Palabra, std::string> ConversorP2S;
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
	NMemonico::operator PDCadena(void)
	{
		if(!(this->valor ^ NMemonico::PD_NEA) || (this->valor ^ NMemonico::PD_ALIAS))
		{
			// No podemos determinar cual era la palabra clave original.
			return "undeterminated";
		}
		IntentaCrearConversor();
		return ConversorP2S.find(this->valor)->second;
	}
	bool operator==(const NMemonico& yo, const NMemonico& otro)
	{
		// Utilizamos XOR para hacer la comparación más rápida
		return !(yo.valor ^ otro.valor);
	}
	bool operator==(const NMemonico& yo, NMemonico::Palabra otro)
	{
		return !(yo.valor ^ otro);
	}
	bool operator!=(const NMemonico& yo, const NMemonico& otro)
	{
		return !(yo == otro);
	}
	bool operator!=(const NMemonico& yo, NMemonico::Palabra otro)
	{
		return !(yo == otro);
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
	NMemonicoProxy& NMemonicoProxy::operator=(const NMemonicoProxy& yo)
	{
		this->begin = yo.begin;
		this->end = yo.end;
		this->original = yo.original;
		return *this;
	}
	NMemonicoProxy& NMemonicoProxy::operator=(const NMemonico& yo)
	{
		*this = ConvertirCadenaANMemonico(ConversorP2S.equal_range(yo.ObtenerValor()).first->second);
		return *this;
	}
	NMemonicoProxy& NMemonicoProxy::operator=(const NMemonico::Palabra& yo)
	{
		*this = ConvertirCadenaANMemonico(ConversorP2S.equal_range(yo).first->second);
		return *this;
	}
	bool operator==(const NMemonicoProxy& yo, NMemonico otro)
	{
		return (yo == otro.ObtenerValor());
	}
	bool operator!=(const NMemonicoProxy& yo, NMemonico otro)
	{
		return !(yo == otro);
	}
	bool operator==(const NMemonicoProxy& yo, NMemonico::Palabra otro)
	{
		NMemonicoProxy::iterator iter;
		for(iter = yo.begin; iter != yo.end; iter++)
		{
			if(iter->second == otro)
			{
				return true;
			}
		}
		return false;
	}
	bool operator!=(const NMemonicoProxy& yo, NMemonico::Palabra otro)
	{
		return !(yo == otro);
	}
	bool operator==(const NMemonicoProxy& yo, const NMemonicoProxy& otro)
	{
		NMemonicoProxy::iterator iter;
		for(iter = yo.begin; iter != yo.end; iter++)
		{
			if(otro == iter->second)
			{
				return true;
			}
		}
		return false;
	}
	bool operator!=(const NMemonicoProxy& yo, const NMemonicoProxy& otro)
	{
		return !(yo == otro);
	}

	NMemonicoProxy ConvertirCadenaANMemonico(PDCadena in)
	{
		PDCadena aConvertir = in;
		if(in.substr(0, PDCadena("Importar.").size()) == PDCadena("Importar."))
		{
			aConvertir = "NEA expr";
		}
		auto values = ConversorS2P.equal_range(aConvertir);
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
		NMemonicoProxy proxy;
		proxy.original = in;
		proxy.begin = values.first;
		proxy.end = values.second;
		return proxy;
	}

	std::istream& operator>>(std::istream& in, NMemonicoProxy& res)
	{
		std::string val = "";
		in >> val;
		res = ConvertirCadenaANMemonico(val);
		return in;
	}
	std::ostream& operator<<(std::ostream& out, const NMemonicoProxy& res)
	{
		out << res.original;
		return out;
	}
	std::ostream& operator<<(std::ostream& out, NMemonico res)
	{
		IntentaCrearConversor();
		out << ConversorP2S.find(res)->second;
		return out;
	}
}
