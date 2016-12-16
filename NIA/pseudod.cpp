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
	PDvar::PDDatos* datos,
	PDFuncionNIA fnc
)
{
	PDDATA = new PDvar::PDDatos(*datos);
	PDDATA->PROCESAR = fnc;
	PDDATA->PROCESO = PDEjecutar;
}

extern "C" void PDEjecutar(PDCadena o, std::istream& i)
{
	if(o == PDS("SaludarA"))
	{
		PDCadena v = "";
		i >> v;
		PDvar::Variante& r = PDDATA->ObtenerVariable(v);
		std::cout << "Hola " << r.ObtenerCadena() << " mucho gusto!" << std::endl;
	}
	else if(o == PDS("ModificarA"))
	{
		PDCadena v = "";
		i >> v;
		PDvar::Variante& r = PDDATA->ObtenerVariable(v);
		r = PDDATA->VERDADERO;
	}
	else if(o == PDS("LlamarA"))
	{
		PDCadena v;
		i >> v;
		std::ifstream in(PDDATA->ObtenerVariable(v).ObtenerCadena().c_str());
		PDvar::Variante& a = PDDATA->ObtenerVariable("__ARCH__");
		PDDATA->ObtenerVariable("__ARCH__") = PDDATA->ObtenerVariable(v);
		while(in >> v)
		{
			PDDATA->Ejecutar(v, in);
		}
		PDDATA->ObtenerVariable("__ARCH__") = a;
	}
	else if(o == PDS("Cad.Concatenar"))
	{
		PDCadena a = "", b = "", r = "";
		i >> a >> b >> r;
		PDvar::Variante& vr = PDDATA->ObtenerVariable(r);
		PDvar::Variante& va = PDDATA->ObtenerVariable(a);
		PDvar::Variante& vb = PDDATA->ObtenerVariable(b);
		vr = va.ObtenerCadena() + vb.ObtenerCadena();
	}
	else if(o == PDS("Cad.Sub_Cadena"))
	{
		PDCadena a = "", b = "", r = "", r2 = "";
		i >> a >> b >> r2 >> r;
		PDvar::Variante& vr = PDDATA->ObtenerVariable(r);
		PDvar::Variante& va = PDDATA->ObtenerVariable(a);
		PDvar::Variante& vb = PDDATA->ObtenerVariable(b);
		PDvar::Variante& vr2 = PDDATA->ObtenerVariable(r2);
		if((va.ObtenerEntero() + vb.ObtenerEntero()) <= vr2.ObtenerCadena().size())
			vr = vr2.ObtenerCadena().substr(va.ObtenerEntero(), vb.ObtenerEntero());
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
		PDCadena a = "", b = "", r = "";
		i >> a >> b >> r;
		PDvar::Variante& vr = PDDATA->ObtenerVariable(r);
		PDvar::Variante& va = PDDATA->ObtenerVariable(a);
		PDvar::Variante& vb = PDDATA->ObtenerVariable(b);
		while(vr.ObtenerCadena().find(va.ObtenerCadena().c_str(), 0) != std::string::npos)
		{
			int i2 = vr.ObtenerCadena().find(va.ObtenerCadena().c_str(), 0);
			vr.ObtenerCadena().replace(i2, va.ObtenerCadena().size(), vb.ObtenerCadena().c_str());
		}
	}
	else if(o == PDS("Cad.Caracter_Especial"))
	{
		PDCadena a = "", r = "";
		i >> a >> r;
		PDvar::Variante& va = PDDATA->ObtenerVariable(a);
		PDvar::Variante& vr = PDDATA->ObtenerVariable(r);
		if(va == PDCadena("NL"))
			vr.ObtenerCadena() += "\n";
		else if(va == PDCadena("RC"))
			vr.ObtenerCadena() += "\r";
		else if(va == PDCadena("CN"))
			vr.ObtenerCadena() += "\0";
		else if(va == PDCadena("EB"))
			vr.ObtenerCadena() += " ";
		else if(va == PDCadena("CV"))
			vr.ObtenerCadena() = "";
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
		PDCadena a = "", b = "", c = "", r = "";
		i >> a >> b >> c >> r;
		PDvar::Variante& vr = PDDATA->ObtenerVariable(r);
		PDvar::Variante& va = PDDATA->ObtenerVariable(a);
		PDvar::Variante& vb = PDDATA->ObtenerVariable(b);
		PDvar::Variante& vc = PDDATA->ObtenerVariable(c);
		if(vc.ObtenerCadena().find(va.ObtenerCadena().c_str(), vb.ObtenerEntero()) != std::string::npos)
			vr.ObtenerEntero() = vc.ObtenerCadena().find(va.ObtenerCadena().c_str(), vb.ObtenerEntero());
		else
			vr = PDDATA->ERROR;
	}
	else if(o == PDS("Cad.Tamaño"))
	{
		PDCadena a = "", r = "";
		i >> a >> r;
		PDDATA->ObtenerVariable(r).ObtenerEntero() = PDDATA->ObtenerVariable(a).ObtenerCadena().size();
	}
	else if(o == PDS("Cad.Caracter"))
	{
		PDCadena a = "", r = "", r2 = "";
		i >> a >> r >> r2;
		PDEntero v = PDDATA->ObtenerVariable(a);
		PDvar::Variante& v2 = PDDATA->ObtenerVariable(r);
		PDvar::Variante& v3 = PDDATA->ObtenerVariable(r2);
		if(v < v2.ObtenerCadena().size())
			v3.ObtenerCadena() += v2.ObtenerCadena()[v];
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
		PDCadena a = "", b = "", r = "";
		i >> a >> b >> r;
		PDvar::Variante& vr = PDDATA->ObtenerVariable(r);
		PDvar::Variante& va = PDDATA->ObtenerVariable(a);
		PDvar::Variante& vb = PDDATA->ObtenerVariable(b);
		vr = va.ObtenerEntero() + vb.ObtenerEntero();
	}
	else if(o == PDS("Ent.Restar"))
	{
		PDCadena a = "", b = "", r = "";
		i >> a >> b >> r;
		PDvar::Variante& vr = PDDATA->ObtenerVariable(r);
		PDvar::Variante& va = PDDATA->ObtenerVariable(a);
		PDvar::Variante& vb = PDDATA->ObtenerVariable(b);
		vr = va.ObtenerEntero() - vb.ObtenerEntero();
	}
	else if(o == PDS("Ent.Multiplicar"))
	{
		PDCadena a = "", b = "", r = "";
		i >> a >> b >> r;
		PDvar::Variante& vr = PDDATA->ObtenerVariable(r);
		PDvar::Variante& va = PDDATA->ObtenerVariable(a);
		PDvar::Variante& vb = PDDATA->ObtenerVariable(b);
		vr = va.ObtenerEntero() * vb.ObtenerEntero();
	}
	else if(o == PDS("Ent.Dividir"))
	{
		PDCadena a = "", b = "", r = "";
		i >> a >> b >> r;
		PDvar::Variante& vr = PDDATA->ObtenerVariable(r);
		PDvar::Variante& va = PDDATA->ObtenerVariable(a);
		PDvar::Variante& vb = PDDATA->ObtenerVariable(b);
		if(vb.ObtenerEntero() != 0)
		{
			vr = va.ObtenerEntero() / vb.ObtenerEntero();
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
		PDCadena c = "", a = "", b = "", r = "";
		i >> a >> c >> b >> r;
		PDvar::Variante& vr = PDDATA->ObtenerVariable(r);
		PDvar::Variante& va = PDDATA->ObtenerVariable(a);
		PDvar::Variante& vb = PDDATA->ObtenerVariable(b);
		if(c == "=")
			vr = ((va.ObtenerEntero() == vb.ObtenerEntero())? PDDATA->VERDADERO : PDDATA->FALSO);
		else if(c == "<")
			vr = ((va.ObtenerEntero() < vb.ObtenerEntero())? PDDATA->VERDADERO : PDDATA->FALSO);
		else if(c == "<=")
			vr = ((va.ObtenerEntero() <= vb.ObtenerEntero())? PDDATA->VERDADERO : PDDATA->FALSO);
		else if(c == ">")
			vr = ((va.ObtenerEntero() > vb.ObtenerEntero())? PDDATA->VERDADERO : PDDATA->FALSO);
		else if(c == ">=")
			vr = ((va.ObtenerEntero() >= vb.ObtenerEntero())? PDDATA->VERDADERO : PDDATA->FALSO);
		else if(c == "!=")
			vr = ((va.ObtenerEntero() != vb.ObtenerEntero())? PDDATA->VERDADERO : PDDATA->FALSO);
		else if(c == "<?")
			vr = ((va.ObtenerEntero() <= vb.ObtenerEntero())? va : vb);
		else if(c == "?>")
			vr = ((va.ObtenerEntero() >= vb.ObtenerEntero())? va : vb);
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
		PDCadena a = "", b = "", r = "";
		i >> a >> b >> r;
		PDvar::Variante& vr = PDDATA->ObtenerVariable(r);
		PDvar::Variante& va = PDDATA->ObtenerVariable(a);
		PDvar::Variante& vb = PDDATA->ObtenerVariable(b);
		vr = va.ObtenerReal() + vb.ObtenerReal();
	}
	else if(o == PDS("Dec.Restar"))
	{
		PDCadena a = "", b = "", r = "";
		i >> a >> b >> r;
		PDvar::Variante& vr = PDDATA->ObtenerVariable(r);
		PDvar::Variante& va = PDDATA->ObtenerVariable(a);
		PDvar::Variante& vb = PDDATA->ObtenerVariable(b);
		vr = va.ObtenerReal() - vb.ObtenerReal();
	}
	else if(o == PDS("Dec.Multiplicar"))
	{
		PDCadena a = "", b = "", r = "";
		i >> a >> b >> r;
		PDvar::Variante& vr = PDDATA->ObtenerVariable(r);
		PDvar::Variante& va = PDDATA->ObtenerVariable(a);
		PDvar::Variante& vb = PDDATA->ObtenerVariable(b);
		vr = va.ObtenerReal() * vb.ObtenerReal();
	}
	else if(o == PDS("Dec.Dividir"))
	{
		PDCadena a = "", b = "", r = "";
		i >> a >> b >> r;
		PDvar::Variante& vr = PDDATA->ObtenerVariable(r);
		PDvar::Variante& va = PDDATA->ObtenerVariable(a);
		PDvar::Variante& vb = PDDATA->ObtenerVariable(b);
		if(vb.ObtenerReal() == 0)
		{
			vr = PDDATA->ERROR;
			throw PDvar::ErrorDeSemantica(
				"Error en Dec.Dividir: 'Dec.Dividir a b c': division entre cero"
			);
		}
		vr = va.ObtenerReal() / vb.ObtenerReal();
	}
	else if(o == PDS("Dec.Comparar"))
	{
		PDCadena c = "", a = "", b = "", r = "";
		i >> a >> c >> b >> r;
		PDvar::Variante& vr = PDDATA->ObtenerVariable(r);
		PDvar::Variante& va = PDDATA->ObtenerVariable(a);
		PDvar::Variante& vb = PDDATA->ObtenerVariable(b);
		if(c == "=")
			vr = ((va.ObtenerReal() == vb.ObtenerReal())? PDDATA->VERDADERO : PDDATA->FALSO);
		else if(c == "<")
			vr = ((va.ObtenerReal() < vb.ObtenerReal())? PDDATA->VERDADERO : PDDATA->FALSO);
		else if(c == "<=")
			vr = ((va.ObtenerReal() <= vb.ObtenerReal())? PDDATA->VERDADERO : PDDATA->FALSO);
		else if(c == ">")
			vr = ((va.ObtenerReal() > vb.ObtenerReal())? PDDATA->VERDADERO : PDDATA->FALSO);
		else if(c == ">=")
			vr = ((va.ObtenerReal() >= vb.ObtenerReal())? PDDATA->VERDADERO : PDDATA->FALSO);
		else if(c == "!=")
			vr = ((va.ObtenerReal() != vb.ObtenerReal())? PDDATA->VERDADERO : PDDATA->FALSO);
		else if(c == "<?")
			vr = ((va.ObtenerReal() <= vb.ObtenerReal())? va : vb);
		else if(c == "?>")
			vr = ((va.ObtenerReal() >= vb.ObtenerReal())? va : vb);
		else
		{
			vr = PDDATA->ERROR;
			throw PDvar::ErrorDeSintaxis(
				"Error en Ent.Comparar, 'Ent.Comparar a OP b r': no se reconoce 'OP'"
			);
		}
	}
	#include "codefile.cpp"
}

extern "C" void PDFinal(void)
{
	delete PDDATA;
}
