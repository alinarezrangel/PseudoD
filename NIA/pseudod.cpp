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

#include "pseudod.hh"
#include "definitionfile.cpp"

PDvar::PDDatos* PDDATA;

PDCadena PDS(PDCadena o)
{
	return "Importar." + o;
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

extern "C" void PDEjecutar(PDCadena o, std::istream& i)
{
	if(o == PDS("SaludarA"))
	{
		PDCadena v;
		i >> v;
		PDCadena r = PDDATA->ObtenerVariable(v);
		std::cout << "Hola " << r << " mucho gusto!" << std::endl;
	}
	else if(o == PDS("ModificarA"))
	{
		PDCadena v;
		i >> v;
		PDCadena& r = PDDATA->ObtenerVariable(v);
		r = PDDATA->VERDADERO;
	}
	else if(o == PDS("LlamarA"))
	{
		PDCadena v;
		i >> v;
		std::ifstream in(PDDATA->ObtenerVariable(v).c_str());
		PDCadena a = PDDATA->ObtenerVariable("__ARCH__");
		PDDATA->ObtenerVariable("__ARCH__") = PDDATA->ObtenerVariable(v);
		while(in >> v)
		{
			PDDATA->Ejecutar(v, in);
		}
		PDDATA->ObtenerVariable("__ARCH__") = a;
	}
	else if(o == PDS("Cad.Concatenar"))
	{
		PDCadena a = "", b = "", r = "", va = "", vb = "";
		i >> a >> b >> r;
		PDCadena& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		vr = va + vb;
	}
	else if(o == PDS("Cad.Sub_Cadena"))
	{
		PDCadena a = "", b = "", r = "", r2 = "", va = "", vb = "";
		PDEntero v1 = 0, v2 = 0;
		i >> a >> b >> r2 >> r;
		PDCadena& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		PDCadena& vr2 = PDDATA->ObtenerVariable(r2);
		v1 = cae(va);
		v2 = cae(vb);
		if((v1 + v2) <= vr2.size())
			vr = vr2.substr(v1,v2);
		else
		{
			vr = PDDATA->ERROR;
			throw PDvar::ErrorDeSemantica(
				PDCadena("Error en Cad.Sub_Cadena: 'Cad.Sub_Cadena v1 v2 r1 r2': la suma del") +
				"indice y el tamaño es mayor a la cadena"
			);
		}
	}
	else if(o == PDS("Cad.Reemplazar"))
	{
		PDCadena a = "", b = "", r = "", va = "", vb = "";
		i >> a >> b >> r;
		PDCadena& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		while(vr.find(va.c_str(), 0) != std::string::npos)
		{
			int i2 = vr.find(va.c_str(), 0);
			vr.replace(i2, va.size(), vb);
		}
	}
	else if(o == PDS("Cad.Caracter_Especial"))
	{
		PDCadena a = "", r = "", va = "";
		i >> a >> r;
		va = PDDATA->ObtenerVariable(a);
		PDCadena& vr = PDDATA->ObtenerVariable(r);
		if(va == "NL")
			vr += "\n";
		else if(va == "RC")
			vr += "\r";
		else if(va == "CN")
			vr += "\0";
		else if(va == "EB")
			vr += " ";
		else if(va == "CV")
			vr = "";
		else
		{
			vr = PDDATA->ERROR;
			throw PDvar::ErrorDeSintaxis(
				"Error en Cad.Caracter_Especial: 'Caracter_Especial a r': no se reconoce 'a'"
			);
		}
	}
	else if(o == PDS("Cad.Buscar"))
	{
		PDCadena a = "", b = "", c = "", r = "", va = "", vb = "", vc = "";
		i >> a >> b >> c >> r;
		PDCadena& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		vc = PDDATA->ObtenerVariable(c);
		PDEntero v1 = cae(vb);
		if(vc.find(va, v1) != std::string::npos)
			vr = eas(vc.find(va.c_str(), v1));
		else
			vr = PDDATA->ERROR;
	}
	else if(o == PDS("Cad.Tamaño"))
	{
		PDCadena a = "", r = "";
		i >> a >> r;
		PDDATA->ObtenerVariable(r) = eas(PDDATA->ObtenerVariable(a).size());
	}
	else if(o == PDS("Cad.Caracter"))
	{
		PDCadena a = "", r = "", r2 = "", v2 = "";
		i >> a >> r >> r2;
		PDEntero v = cae(PDDATA->ObtenerVariable(a));
		v2 = PDDATA->ObtenerVariable(r);
		PDCadena& v3 = PDDATA->ObtenerVariable(r2);
		if(v < v2.size())
			v3 += v2[v];
		else
		{
			v3 = PDDATA->ERROR;
			throw PDvar::ErrorDelNucleo(
				"Error en Cad.Caracter: 'Cad.Caracter a r r2': posicion invalida"
			);
		}
	}
	else if(o == PDS("Ent.Sumar"))
	{
		PDCadena a = "", b = "", r = "", va = "", vb = "";
		PDEntero v1 = 0, v2 = 0;
		i >> a >> b >> r;
		PDCadena& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		v1 = cae(va);
		v2 = cae(vb);
		PDEntero v3 = v1 + v2;
		vr = eas(v3);
	}
	else if(o == PDS("Ent.Restar"))
	{
		PDCadena a = "", b = "", r = "", va = "", vb = "";
		PDEntero v1 = 0, v2 = 0;
		i >> a >> b >> r;
		PDCadena& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		v1 = cae(va);
		v2 = cae(vb);
		PDEntero v3 = v1 - v2;
		vr = eas(v3);
	}
	else if(o == PDS("Ent.Multiplicar"))
	{
		PDCadena a = "", b = "", r = "", va = "", vb = "";
		PDEntero v1 = 0, v2 = 0;
		i >> a >> b >> r;
		PDCadena& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		v1 = cae(va);
		v2 = cae(vb);
		PDEntero v3 = v1 * v2;
		vr = eas(v3);
	}
	else if(o == PDS("Ent.Dividir"))
	{
		PDCadena a = "", b = "", r = "", va = "", vb = "";
		PDEntero v1 = 0, v2 = 0;
		i >> a >> b >> r;
		PDCadena& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		v1 = cae(va);
		v2 = cae(vb);
		if(v2 != 0)
		{
			PDEntero v3 = v1 / v2;
			vr = eas(v3);
		}
		else
		{
			vr = PDDATA->ERROR;
			throw PDvar::ErrorDeSemantica(
				"Error en Ent.Dividir: 'Ent.Dividir a b c': division entre cero"
			);
		}
	}
	else if(o == PDS("Ent.Comparar"))
	{
		PDCadena c = "", a = "", b = "", r = "", va = "", vb = "";
		PDEntero v1 = 0, v2 = 0;
		i >> a >> c >> b >> r;
		PDCadena& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		v1 = cae(va);
		v2 = cae(vb);
		if(c == "=")
		{
			vr = ((v1 == v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == "<")
		{
			vr = ((v1 < v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == "<=")
		{
			vr = ((v1 <= v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == ">")
		{
			vr = ((v1 > v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == ">=")
		{
			vr = ((v1 >= v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == "!=")
		{
			vr = ((v1 != v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == "<?")
		{
			vr = eas(((v1 <= v2)? v1 : v2));
		}
		else if(c == "?>")
		{
			vr = eas(((v1 >= v2)? v1 : v2));
		}
		else
		{
			vr = PDDATA->ERROR;
			throw PDvar::ErrorDeSintaxis(
				"Error en Ent.Comparar, 'Ent.Comparar a OP b r': no se reconoce 'OP'"
			);
		}
	}
	else if(o == PDS("Dec.Sumar"))
	{
		PDCadena a = "", b = "", r = "", va = "", vb = "";
		PDDecimal v1 = 0, v2 = 0;
		i >> a >> b >> r;
		PDCadena& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		v1 = caf(va);
		v2 = caf(vb);
		PDDecimal v3 = v1 + v2;
		vr = dac(v3);
	}
	else if(o == PDS("Dec.Restar"))
	{
		PDCadena a = "", b = "", r = "", va = "", vb = "";
		PDDecimal v1 = 0, v2 = 0;
		i >> a >> b >> r;
		PDCadena& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		v1 = caf(va);
		v2 = caf(vb);
		PDDecimal v3 = v1 - v2;
		vr = dac(v3);
	}
	else if(o == PDS("Dec.Multiplicar"))
	{
		PDCadena a = "", b = "", r = "", va = "", vb = "";
		PDDecimal v1 = 0, v2 = 0;
		i >> a >> b >> r;
		PDCadena& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		v1 = caf(va);
		v2 = caf(vb);
		PDDecimal v3 = v1 * v2;
		vr = dac(v3);
	}
	else if(o == PDS("Dec.Dividir"))
	{
		PDCadena a = "", b = "", r = "", va = "", vb = "";
		PDDecimal v1 = 0, v2 = 0;
		i >> a >> b >> r;
		PDCadena& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		v1 = caf(va);
		v2 = caf(vb);
		if(v2 == 0)
		{
			vr = PDDATA->ERROR;
			throw PDvar::ErrorDeSemantica(
				"Error en Dec.Dividir: 'Dec.Dividir a b c': division entre cero"
			);
		}
		PDDecimal v3 = v1 / v2;
		vr = dac(v3);
	}
	else if(o == PDS("Dec.Comparar"))
	{
		PDCadena c = "", a = "", b = "", r = "", va = "", vb = "";
		PDDecimal v1 = 0, v2 = 0;
		i >> a >> c >> b >> r;
		PDCadena& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		v1 = caf(va);
		v2 = caf(vb);
		if(c == "=")
		{
			vr = ((v1 == v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == "<")
		{
			vr = ((v1 < v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == "<=")
		{
			vr = ((v1 <= v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == ">")
		{
			vr = ((v1 > v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == ">=")
		{
			vr = ((v1 >= v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == "!=")
		{
			vr = ((v1 != v2)? PDDATA->VERDADERO : PDDATA->FALSO);
		}
		else if(c == "<?")
		{
			vr = dac(((v1 <= v2)? v1 : v2));
		}
		else if(c == "?>")
		{
			vr = dac(((v1 >= v2)? v1 : v2));
		}
		else
		{
			vr = PDDATA->ERROR;
			throw PDvar::ErrorDeSintaxis(
				"Error en Dec.Comparar: 'Comparar a OP b r': no se reconoce 'OP'"
			);
		}
	}
	#include "codefile.cpp"
}

extern "C" void PDFinal(void)
{
	delete PDDATA;
}
