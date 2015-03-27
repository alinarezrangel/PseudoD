/*
**PseudoD 1.5.0
**Creado por Alejandro Linarez Rangel
*/
#include "pseudod.hh"

#include "./definitionfile.cpp"

extern "C" void PDInicializar(vector<string>& nvar,vector<string>& vvar,vector<string>& npun,vector<int>& vpun,vector< stack<string> >& pil,void (*procesar)(string o,istream& e, void(*FUNCION)(string,istream&)))
{
	PDDATA = new PDDatos(nvar,vvar,npun,vpun,pil);
	PDDATA->PROCESAR = procesar;
	PDDATA->PROCESO = PDEjecutar;
	C = "Importar.";
}

string PDS(string o)
{
	return C + o;
}

extern "C" void PDEjecutar(string o,istream& i)
{
	if(o == PDS("SaludarA"))
	{
		string v;
		i >> v;
		string r = PDDATA->ObtenerVariable(v);
		cout << "Hola " << r << " mucho gusto!" << endl;
	}
	else if(o == PDS("ModificarA"))
	{
		string v;
		i >> v;
		string& r = PDDATA->ObtenerVariable(v);
		r = "verdadero";
	}
	else if(o == PDS("LlamarA"))
	{
		string v;
		i >> v;
		ifstream in(PDDATA->ObtenerVariable(v).c_str());
		string a = PDDATA->ObtenerVariable(string("__ARCH__"));
		PDDATA->ObtenerVariable(string("__ARCH__")) = PDDATA->ObtenerVariable(v);
		while(in >> v)
		{
			(*PDDATA->PROCESAR)(v,in,PDEjecutar);
		}
		PDDATA->ObtenerVariable(string("__ARCH__")) = a;
	}
	else if(o == PDS("Cad.Concatenar"))
	{
		string a,b,r;
		i >> a >> b >> r;
		string va,vb;
		string& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		vr = va + vb;
	}
	else if(o == PDS("Cad.Sub_Cadena"))
	{
		string a,b,r,r2;
		i >> a >> b >> r2 >> r;
		string va,vb;
		string& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		string& vr2 = PDDATA->ObtenerVariable(r2);
		long long int v1,v2;
		v1 = cae(va);
		v2 = cae(vb);
		if((v1 + v2)<=vr2.size())
			vr = vr2.substr(v1,v2);
		else
		{
			vr = ":C++:error:";
			throw string("En el modulo Cad, en Sub_Cadena v1 v2 r1 r2: v1+v2 > longitud(r1).");
		}
	}
	else if(o == PDS("Cad.Reemplazar"))
	{
		string a,b,r;
		i >> a >> b >> r;
		string va,vb;
		string& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		while(vr.find(va.c_str(),0) != string::npos)
		{
			int i2 = vr.find(va.c_str(),0);
			vr.replace(i2,va.size(),vb);
		}
	}
	else if(o == PDS("Cad.Caracter_Especial"))
	{
		string a,r;
		i >> a >> r;
		string va = PDDATA->ObtenerVariable(a);
		string& vr = PDDATA->ObtenerVariable(r);
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
			vr = ":C++:error:";
			throw string("Error en el modulo Cad, Caracter_Especial a r: no se reconoce a");
		}
	}
	else if(o == PDS("Cad.Buscar"))
	{
		string a,b,c,r;
		i >> a >> b >> c >> r;
		string va,vb,vc;
		string& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		vc = PDDATA->ObtenerVariable(c);
		long long int v1 = cae(vb);
		if(vc.find(va,v1) != string::npos)
			vr = eas(vc.find(va.c_str(),v1));
		else
			vr = ":C++:error:";
	}
	else if(o == PDS("Cad.Tamaño"))
	{
		string a,r;
		i >> a >> r;
		PDDATA->ObtenerVariable(r) = eas(PDDATA->ObtenerVariable(a).size());
	}
	else if(o == PDS("Cad.Caracter"))
	{
		string a,r,r2;
		i >> a >> r >> r2;
		long long int v = cae(PDDATA->ObtenerVariable(a));
		string v2 = PDDATA->ObtenerVariable(r);
		string& v3 = PDDATA->ObtenerVariable(r2);
		if(v < v2.size())
			v3 += v2[v];
		else
		{
			cerr << "Error en Cad.Caracter a r r2: longitud(r2) > a" << endl;
			throw string("Error en Cad.Caracter: Posicion invalida");
		}
	}
	else if(o == PDS("Ent.Sumar"))
	{
		string a,b,r;
		i >> a >> b >> r;
		string va,vb;
		string& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		long long int v1,v2;
		v1 = cae(va);
		v2 = cae(vb);
		long long int v3 = v1 + v2;
		vr = eas(v3);
	}
	else if(o == PDS("Ent.Restar"))
	{
		string a,b,r;
		i >> a >> b >> r;
		string va,vb;
		string& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		long long int v1,v2;
		v1 = cae(va);
		v2 = cae(vb);
			long long int v3 = v1 - v2;
			vr = eas(v3);
	}
	else if(o == PDS("Ent.Multiplicar"))
	{
		string a,b,r;
		i >> a >> b >> r;
		string va,vb;
		string& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		long long int v1,v2;
		v1 = cae(va);
		v2 = cae(vb);
		long long int v3 = v1 * v2;
		vr = eas(v3);
	}
	else if(o == PDS("Ent.Dividir"))
	{
		string a,b,r;
		i >> a >> b >> r;
		string va,vb;
		string& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		long long int v1,v2;
		v1 = cae(va);
		v2 = cae(vb);
		if(!(v2 == 0))
		{
			long long int v3 = v1 / v2;
			vr = eas(v3);
		}
		else
		{
			vr = ":C++:error:";
		}
	}
	else if(o == PDS("Ent.Comparar"))
	{
		string c,a,b,r;
		i >> a >> c >> b >> r;
		string va,vb;
		string& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		long long int v1,v2;
		v1 = cae(va);
		v2 = cae(vb);
		if(c == "=")
			vr = ((v1 == v2)? "verdadero" : "falso");
		else if(c == "<")
			vr = ((v1 < v2)? "verdadero" : "falso");
		else if(c == "<=")
			vr = ((v1 <= v2)? "verdadero" : "falso");
		else if(c == ">")
			vr = ((v1 > v2)? "verdadero" : "falso");
		else if(c == ">=")
			vr = ((v1 >= v2)? "verdadero" : "falso");
		else if(c == "!=")
			vr = ((v1 != v2)? "verdadero" : "falso");
		else if(c == "<?")
			vr = eas(((v1 <= v2)? v1 : v2));
		else if(c == "?>")
			vr = eas(((v1 >= v2)? v1 : v2));
		else
		{
			vr = ":C++:error:";
			throw string("Error en el modulo Ent, Comparar a OP b r: no se reconoce OP");
		}
	}
	else if(o == PDS("Dec.Sumar"))
	{
		string a,b,r;
		i >> a >> b >> r;
		string va,vb;
		string& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		long double v1,v2;
		v1 = caf(va);
		v2 = caf(vb);
			long double v3 = v1 + v2;
			vr = dac(v3);
	}
	else if(o == PDS("Dec.Restar"))
	{
		string a,b,r;
		i >> a >> b >> r;
		string va,vb;
		string& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		long double v1,v2;
		v1 = caf(va);
		v2 = caf(vb);
			long double v3 = v1 - v2;
			vr = dac(v3);
	}
	else if(o == PDS("Dec.Multiplicar"))
	{
		string a,b,r;
		i >> a >> b >> r;
		string va,vb;
		string& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		long double v1,v2;
		v1 = caf(va);
		v2 = caf(vb);
			long double v3 = v1 * v2;
			vr = dac(v3);
	}
	else if(o == PDS("Dec.Dividir"))
	{
		string a,b,r;
		i >> a >> b >> r;
		string va,vb;
		string& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		long double v1,v2;
		v1 = caf(va);
		v2 = caf(vb);
		if(v2 == 0)
		{
			cerr << "Error en Dec.Dividir a b r: b == 0: no se divide entre cero" << endl;
			throw string("Error en Dec.Dividir: division entre cero");
		}
			long double v3 = v1 / v2;
			vr = dac(v3);
	}
	else if(o == PDS("Dec.Comparar"))
	{
		string c,a,b,r;
		i >> a >> c >> b >> r;
		string va,vb;
		string& vr = PDDATA->ObtenerVariable(r);
		va = PDDATA->ObtenerVariable(a);
		vb = PDDATA->ObtenerVariable(b);
		long double v1,v2;
		v1 = caf(va);
		v2 = caf(vb);
		if(c == "=")
			vr = ((v1 == v2)? "verdadero" : "falso");
		else if(c == "<")
			vr = ((v1 < v2)? "verdadero" : "falso");
		else if(c == "<=")
			vr = ((v1 <= v2)? "verdadero" : "falso");
		else if(c == ">")
			vr = ((v1 > v2)? "verdadero" : "falso");
		else if(c == ">=")
			vr = ((v1 >= v2)? "verdadero" : "falso");
		else if(c == "!=")
			vr = ((v1 != v2)? "verdadero" : "falso");
		else if(c == "<?")
			vr = dac(((v1 <= v2)? v1 : v2));
		else if(c == "?>")
			vr = dac(((v1 >= v2)? v1 : v2));
		else
		{
			vr = ":C++:error:";
			throw string("Error en el modulo Dec, Comparar a OP b r: no se reconoce OP");
		}
	}
	#include "./codefile.cpp"
}

extern "C" void PDFinal(void)
{
	delete PDDATA;
}
