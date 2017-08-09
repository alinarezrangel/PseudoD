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

#include "pseudod.hh"
#include "definitionfile.cpp"

PDvar::PDDatos* PDDATA;

PDCadena PDS(PDCadena o)
{
	return o;
}

extern "C" void PDInicializar(
	std::vector<PDCadena>& nvar,
	std::vector<PDCadena>& vvar,
	std::vector<PDCadena>& npun,
	std::vector<int>& vpun,
	std::vector<std::stack<PDCadena>>& pil,
	PDFuncionNIA fnc
)
{
	PDDATA = new PDvar::PDDatos(nvar, vvar, npun, vpun, pil);
	PDDATA->PROCESAR = fnc;
	PDDATA->PROCESO = PDEjecutar;
}

extern "C" void PDEjecutar(pseudod::Token tok, pseudod::Tokenizador& i)
{
	using namespace pseudod::Utilidades;
	using pseudod::Token;

	// El substr(0, 9) elimina el "Importar."
	PDCadena o = tok.ObtenerNMemonico().original.substr(9);

	/*
	std::cout
		<< "# "
		<< tok.ObtenerNMemonico().original
		<< " "
		<< PDDATA->ObtenerVariable("__ARCH__")
		<< ":"
		<< tok.ObtenerDatosFuente().linea
		<< std::endl;
	*/

	if(o == PDS("SaludarA"))
	{
		Token v;

		i >> v;

		if(!Tokens::EsIdentificador(v))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en SaludarA: 'SaludarA var1': Se esperaba una variable"
			);
		}

		const PDCadena& r = PDDATA->ObtenerVariable(Tokens::ObtenerValor(v));

		std::cout << "Hola " << r << " mucho gusto!" << std::endl;
	}
	else if(o == PDS("ModificarA"))
	{
		Token v;

		i >> v;

		if(!Tokens::EsIdentificador(v))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en ModificarA: 'ModificarA var1': Se esperaba una variable"
			);
		}

		PDCadena& r = PDDATA->ObtenerVariable(Tokens::ObtenerValor(v));

		r = PDDATA->VERDADERO;
	}
	else if(o == PDS("LlamarA"))
	{
		std::cerr << "Advertencia: Importar.LlamarA esta obsoleto" << std::endl;
		std::cerr <<
			"Advertencia: Importar.LlamarA ahora esta implementado como una llamada "
			"a 'utilizar'"
			<< std::endl;

		Token k;

		i >> k;

		if(!Tokens::EsIdentificador(k))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en LlamarA: 'LlamarA var1': Se esperaba una variable"
			);
		}

		PDDATA->Ejecutar("utilizar " + Tokens::ObtenerValor(k));
	}
	else if(o == PDS("Cad.Concatenar"))
	{
		Token a, b, r;

		i >> a >> b >> r;

		if((!Tokens::EsIdentificador(a))
			|| (!Tokens::EsIdentificador(b))
			|| (!Tokens::EsIdentificador(r)))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Cad.Concatenar: 'Cad.Concatenar a b r': Se esperaban "
				"variables 'a b r'"
			);
		}

		PDCadena& vr = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r));
		const PDCadena& va = PDDATA->ObtenerVariable(Tokens::ObtenerValor(a));
		const PDCadena& vb = PDDATA->ObtenerVariable(Tokens::ObtenerValor(b));

		vr = va + vb;
	}
	else if(o == PDS("Cad.Sub_Cadena"))
	{
		Token ta, tb, tr1, tr2;

		i >> ta >> tb >> tr1 >> tr2;

		if((!Tokens::EsIdentificador(ta))
			|| (!Tokens::EsIdentificador(tb))
			|| (!Tokens::EsIdentificador(tr1))
			|| (!Tokens::EsIdentificador(tr2)))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Cad.Sub_Cadena: 'Cad.Sub_Cadena a b r1 r2': Se esperaban "
				"variables 'a b r1 r2'"
			);
		}

		const PDCadena& a = PDDATA->ObtenerVariable(Tokens::ObtenerValor(ta));
		const PDCadena& b = PDDATA->ObtenerVariable(Tokens::ObtenerValor(tb));
		PDCadena& r1 = PDDATA->ObtenerVariable(Tokens::ObtenerValor(tr1));
		const PDCadena& r2 = PDDATA->ObtenerVariable(Tokens::ObtenerValor(tr2));

		PDEntero v1 = 0, v2 = 0;

		v1 = cae(a);
		v2 = cae(b);

		r1 = r2.substr(v1, v2);
	}
	else if(o == PDS("Cad.Reemplazar"))
	{
		Token a, b, r;

		i >> a >> b >> r;

		if((!Tokens::EsIdentificador(a))
			|| (!Tokens::EsIdentificador(b))
			|| (!Tokens::EsIdentificador(r)))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Cad.Reemplazar: 'Cad.Reemplazar a b r': Se esperaban "
				"variables 'a b r'"
			);
		}

		PDCadena& vr = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r));
		const PDCadena& va = PDDATA->ObtenerVariable(Tokens::ObtenerValor(a));
		const PDCadena& vb = PDDATA->ObtenerVariable(Tokens::ObtenerValor(b));

		// Debemos prevenir bucles infinitos:
		// > Si vb contiene va en algun punto, el siguiente bucle se
		// > ejecutaria por siempre!
		if(vb.find(va.c_str()) != std::string::npos)
		{
			throw PDvar::ErrorDeSemantica(
				"Error en Cad.Reemplazar: 'Cad.Reemplazar a b r': 'a' (cadena a buscar)"
				" y 'b' (cadena a reemplazar): 'a' contiene a 'b' y se previene un"
				"bucle infinito"
			);
		}

		while(vr.find(va.c_str()) != std::string::npos)
		{
			int i2 = vr.find(va.c_str());

			vr.replace(i2, va.size(), vb);
		}
	}
	else if(o == PDS("Cad.Caracter_Especial"))
	{
		Token a, r;

		i >> a >> r;

		if((!Tokens::EsIdentificador(a))
			|| (!Tokens::EsIdentificador(r)))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Cad.Caracter_Especial: 'Cad.Caracter_Especial a r': Se "
				"esperaban variables 'a r'"
			);
		}

		const PDCadena& va = PDDATA->ObtenerVariable(Tokens::ObtenerValor(a));
		PDCadena& vr = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r));

		if(va == "NL")
		{
			vr += "\n";
		}
		else if(va == "RC")
		{
			vr += "\r";
		}
		else if(va == "CN")
		{
			vr += "\0";
		}
		else if(va == "EB")
		{
			vr += " ";
		}
		else if(va == "CV")
		{
			vr = "";
		}
		else
		{
			vr = PDDATA->ERROR;

			throw PDvar::ErrorDeSintaxis(
				"Error en Cad.Caracter_Especial: 'Caracter_Especial a r': no se "
				"reconoce 'a'"
			);
		}
	}
	else if(o == PDS("Cad.Buscar"))
	{
		Token a, b, c, r;

		i >> a >> b >> c >> r;

		if((!Tokens::EsIdentificador(a))
			|| (!Tokens::EsIdentificador(b))
			|| (!Tokens::EsIdentificador(c))
			|| (!Tokens::EsIdentificador(r)))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Cad.Buscar: 'Cad.Buscar a b c r': Se "
				"esperaban variables 'a b c r'"
			);
		}

		PDCadena& vr = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r));
		const PDCadena& va = PDDATA->ObtenerVariable(Tokens::ObtenerValor(a));
		const PDCadena& vb = PDDATA->ObtenerVariable(Tokens::ObtenerValor(b));
		const PDCadena& vc = PDDATA->ObtenerVariable(Tokens::ObtenerValor(c));

		PDEntero v1 = cae(vb);

		if(vc.find(va, v1) != std::string::npos)
			vr = eas(vc.find(va.c_str(), v1));
		else
			vr = PDDATA->ERROR;
	}
	else if(o == PDS(u8"Cad.Tamaño"))
	{
		Token a, r;

		i >> a >> r;

		if((!Tokens::EsIdentificador(a))
			|| (!Tokens::EsIdentificador(r)))
		{
			throw PDvar::ErrorDeSintaxis(
				u8"Error en Cad.Tamaño: 'Cad.Tamaño a r': Se "
				u8"esperaban variables 'a r'"
			);
		}

		PDCadena& vr = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r));
		const PDCadena& va = PDDATA->ObtenerVariable(Tokens::ObtenerValor(a));

		vr = eas(va.size());
	}
	else if(o == PDS("Cad.Caracter"))
	{
		Token a, r1, r2;

		i >> a >> r1 >> r2;

		if((!Tokens::EsIdentificador(a))
			|| (!Tokens::EsIdentificador(r1))
			|| (!Tokens::EsIdentificador(r2)))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Cad.Caracter: 'Cad.Caracter a r1 r2': Se "
				"esperaban variables 'a r1 r2'"
			);
		}

		PDEntero va = cae(PDDATA->ObtenerVariable(Tokens::ObtenerValor(a)));
		const PDCadena& vr1 = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r1));
		PDCadena& vr2 = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r2));

		if(va < vr1.size())
		{
			vr2 += vr1[va];
		}
		else
		{
			vr2 = PDDATA->ERROR;

			throw PDvar::ErrorDelNucleo(
				"Error en Cad.Caracter: 'Cad.Caracter a r r2': Indice invalido"
			);
		}
	}
	else if(o == PDS("Ent.Sumar"))
	{
		Token a, b, r;

		i >> a >> b >> r;

		if((!Tokens::EsIdentificador(a))
			|| (!Tokens::EsIdentificador(b))
			|| (!Tokens::EsIdentificador(r)))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Ent.Sumar: 'Ent.Sumar a b r': Se "
				"esperaban variables 'a b r'"
			);
		}

		PDCadena& vr = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r));
		const PDCadena& va = PDDATA->ObtenerVariable(Tokens::ObtenerValor(a));
		const PDCadena& vb = PDDATA->ObtenerVariable(Tokens::ObtenerValor(b));

		PDEntero v1 = cae(va);
		PDEntero v2 = cae(vb);

		PDEntero v3 = v1 + v2;

		vr = eas(v3);
	}
	else if(o == PDS("Ent.Restar"))
	{
		Token a, b, r;

		i >> a >> b >> r;

		if((!Tokens::EsIdentificador(a))
			|| (!Tokens::EsIdentificador(b))
			|| (!Tokens::EsIdentificador(r)))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Ent.Restar: 'Ent.Restar a b r': Se "
				"esperaban variables 'a b r'"
			);
		}

		PDCadena& vr = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r));
		const PDCadena& va = PDDATA->ObtenerVariable(Tokens::ObtenerValor(a));
		const PDCadena& vb = PDDATA->ObtenerVariable(Tokens::ObtenerValor(b));

		PDEntero v1 = cae(va);
		PDEntero v2 = cae(vb);

		PDEntero v3 = v1 - v2;

		vr = eas(v3);
	}
	else if(o == PDS("Ent.Multiplicar"))
	{
		Token a, b, r;

		i >> a >> b >> r;

		if((!Tokens::EsIdentificador(a))
			|| (!Tokens::EsIdentificador(b))
			|| (!Tokens::EsIdentificador(r)))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Ent.Multiplicar: 'Ent.Multiplicar a b r': Se "
				"esperaban variables 'a b r'"
			);
		}

		PDCadena& vr = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r));
		const PDCadena& va = PDDATA->ObtenerVariable(Tokens::ObtenerValor(a));
		const PDCadena& vb = PDDATA->ObtenerVariable(Tokens::ObtenerValor(b));

		PDEntero v1 = cae(va);
		PDEntero v2 = cae(vb);

		PDEntero v3 = v1 * v2;

		vr = eas(v3);
	}
	else if(o == PDS("Ent.Dividir"))
	{
		Token a, b, r;

		i >> a >> b >> r;

		if((!Tokens::EsIdentificador(a))
			|| (!Tokens::EsIdentificador(b))
			|| (!Tokens::EsIdentificador(r)))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Ent.Dividir: 'Ent.Dividir a b r': Se "
				"esperaban variables 'a b r'"
			);
		}

		PDCadena& vr = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r));
		const PDCadena& va = PDDATA->ObtenerVariable(Tokens::ObtenerValor(a));
		const PDCadena& vb = PDDATA->ObtenerVariable(Tokens::ObtenerValor(b));

		PDEntero v1 = cae(va);
		PDEntero v2 = cae(vb);

		if(v2 != 0)
		{
			PDEntero v3 = v1 / v2;

			vr = eas(v3);
		}
		else
		{
			vr = PDDATA->ERROR;

			throw PDvar::ErrorDeSemantica(
				"Error en Ent.Dividir: 'Ent.Dividir a b r': Division entre cero"
			);
		}
	}
	else if(o == PDS("Ent.Comparar"))
	{
		Token a, b, c, r;

		i >> a >> c >> b >> r;

		if((!Tokens::EsIdentificador(a))
			|| (!Tokens::EsIdentificador(b))
			|| (!Tokens::EsIdentificador(r)))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Ent.Comparar: 'Ent.Comparar a c b r': Se "
				"esperaban variables 'a * b r'"
			);
		}

		if(!Tokens::EsNMemonico(c))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Ent.Comparar: 'Ent.Comparar a c b r': Se "
				"esperaba nmemonico 'c'"
			);
		}

		PDCadena& vr = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r));
		const PDCadena& va = PDDATA->ObtenerVariable(Tokens::ObtenerValor(a));
		const PDCadena& vb = PDDATA->ObtenerVariable(Tokens::ObtenerValor(b));

		PDEntero v1 = cae(va);
		PDEntero v2 = cae(vb);

		if(c == pseudod::NMemonico::PD_COMPARAR_IGUAL)
		{
			vr = ((v1 == v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == pseudod::NMemonico::PD_COMPARAR_MENOR)
		{
			vr = ((v1 < v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == pseudod::NMemonico::PD_COMPARAR_MENOR_O_IGUAL)
		{
			vr = ((v1 <= v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == pseudod::NMemonico::PD_COMPARAR_MAYOR)
		{
			vr = ((v1 > v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == pseudod::NMemonico::PD_COMPARAR_MAYOR_O_IGUAL)
		{
			vr = ((v1 >= v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == pseudod::NMemonico::PD_COMPARAR_DISTINTO)
		{
			vr = ((v1 != v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == pseudod::NMemonico::PD_COMPARAR_MINIMO)
		{
			vr = eas(((v1 <= v2)? v1 : v2));
		}
		else if(c == pseudod::NMemonico::PD_COMPARAR_MAXIMO)
		{
			vr = eas(((v1 >= v2)? v1 : v2));
		}
		else
		{
			vr = PDDATA->ERROR;

			throw PDvar::ErrorDeSintaxis(
				"Error en Ent.Comparar, 'Ent.Comparar a c b r': No se reconoce 'c': "
				"Se esperaba un operador"
			);
		}
	}
	else if(o == PDS("Dec.Sumar"))
	{
		Token a, b, r;

		i >> a >> b >> r;

		if((!Tokens::EsIdentificador(a))
			|| (!Tokens::EsIdentificador(b))
			|| (!Tokens::EsIdentificador(r)))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Dec.Sumar: 'Dec.Sumar a b r': Se "
				"esperaban variables 'a b r'"
			);
		}

		PDCadena& vr = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r));
		const PDCadena& va = PDDATA->ObtenerVariable(Tokens::ObtenerValor(a));
		const PDCadena& vb = PDDATA->ObtenerVariable(Tokens::ObtenerValor(b));

		PDDecimal v1 = caf(va);
		PDDecimal v2 = caf(vb);

		PDDecimal v3 = v1 + v2;

		vr = dac(v3);
	}
	else if(o == PDS("Dec.Restar"))
	{
		Token a, b, r;

		i >> a >> b >> r;

		if((!Tokens::EsIdentificador(a))
			|| (!Tokens::EsIdentificador(b))
			|| (!Tokens::EsIdentificador(r)))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Dec.Restar: 'Dec.Restar a b r': Se "
				"esperaban variables 'a b r'"
			);
		}

		PDCadena& vr = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r));
		const PDCadena& va = PDDATA->ObtenerVariable(Tokens::ObtenerValor(a));
		const PDCadena& vb = PDDATA->ObtenerVariable(Tokens::ObtenerValor(b));

		PDDecimal v1 = caf(va);
		PDDecimal v2 = caf(vb);

		PDDecimal v3 = v1 - v2;

		vr = dac(v3);
	}
	else if(o == PDS("Dec.Multiplicar"))
	{
		Token a, b, r;

		i >> a >> b >> r;

		if((!Tokens::EsIdentificador(a))
			|| (!Tokens::EsIdentificador(b))
			|| (!Tokens::EsIdentificador(r)))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Dec.Multiplicar: 'Dec.Multiplicar a b r': Se "
				"esperaban variables 'a b r'"
			);
		}

		PDCadena& vr = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r));
		const PDCadena& va = PDDATA->ObtenerVariable(Tokens::ObtenerValor(a));
		const PDCadena& vb = PDDATA->ObtenerVariable(Tokens::ObtenerValor(b));

		PDDecimal v1 = caf(va);
		PDDecimal v2 = caf(vb);

		PDDecimal v3 = v1 * v2;

		vr = dac(v3);
	}
	else if(o == PDS("Dec.Dividir"))
	{
		Token a, b, r;

		i >> a >> b >> r;

		if((!Tokens::EsIdentificador(a))
			|| (!Tokens::EsIdentificador(b))
			|| (!Tokens::EsIdentificador(r)))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Dec.Dividir: 'Dec.Dividir a b r': Se "
				"esperaban variables 'a b r'"
			);
		}

		PDCadena& vr = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r));
		const PDCadena& va = PDDATA->ObtenerVariable(Tokens::ObtenerValor(a));
		const PDCadena& vb = PDDATA->ObtenerVariable(Tokens::ObtenerValor(b));

		PDDecimal v1 = caf(va);
		PDDecimal v2 = caf(vb);

		if(v2 != 0)
		{
			PDDecimal v3 = v1 / v2;

			vr = dac(v3);
		}
		else
		{
			vr = PDDATA->ERROR;

			throw PDvar::ErrorDeSemantica(
				"Error en Dec.Dividir: 'Dec.Dividir a b r': Division entre cero"
			);
		}
	}
	else if(o == PDS("Dec.Comparar"))
	{
		Token a, b, c, r;

		i >> a >> c >> b >> r;

		if((!Tokens::EsIdentificador(a))
			|| (!Tokens::EsIdentificador(b))
			|| (!Tokens::EsIdentificador(r)))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Dec.Comparar: 'Dec.Comparar a c b r': Se "
				"esperaban variables 'a * b r'"
			);
		}

		if(!Tokens::EsNMemonico(c))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en Dec.Comparar: 'Dec.Comparar a c b r': Se "
				"esperaba nmemonico 'c'"
			);
		}

		PDCadena& vr = PDDATA->ObtenerVariable(Tokens::ObtenerValor(r));
		const PDCadena& va = PDDATA->ObtenerVariable(Tokens::ObtenerValor(a));
		const PDCadena& vb = PDDATA->ObtenerVariable(Tokens::ObtenerValor(b));

		PDDecimal v1 = caf(va);
		PDDecimal v2 = caf(vb);

		if(c == pseudod::NMemonico::PD_COMPARAR_IGUAL)
		{
			vr = ((v1 == v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == pseudod::NMemonico::PD_COMPARAR_MENOR)
		{
			vr = ((v1 < v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == pseudod::NMemonico::PD_COMPARAR_MENOR_O_IGUAL)
		{
			vr = ((v1 <= v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == pseudod::NMemonico::PD_COMPARAR_MAYOR)
		{
			vr = ((v1 > v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == pseudod::NMemonico::PD_COMPARAR_MAYOR_O_IGUAL)
		{
			vr = ((v1 >= v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == pseudod::NMemonico::PD_COMPARAR_DISTINTO)
		{
			vr = ((v1 != v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == pseudod::NMemonico::PD_COMPARAR_MINIMO)
		{
			vr = dac(((v1 <= v2)? v1 : v2));
		}
		else if(c == pseudod::NMemonico::PD_COMPARAR_MAXIMO)
		{
			vr = dac(((v1 >= v2)? v1 : v2));
		}
		else
		{
			vr = PDDATA->ERROR;

			throw PDvar::ErrorDeSintaxis(
				"Error en Dec.Comparar, 'Dec.Comparar a c b r': No se reconoce 'c': "
				"Se esperaba un operador"
			);
		}
	}

	#include "codefile.cpp"
}

extern "C" void PDFinal(void)
{
	delete PDDATA;
}
