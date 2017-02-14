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


{
	PDTipos::PseudoArray arr;
	if(o == PDS(arr.ObtenerClave()))
	{
		arr.LeerParametros(i);
		arr.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
{
	PDTipos::PseudoClase cls;
	if(o == PDS(cls.ObtenerClave()))
	{
		cls.LeerParametros(i);
		cls.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
{
	PDTipos::PseudoReferenciaClase rc;
	if(o == PDS(rc.ObtenerClave()))
	{
		rc.LeerParametros(i);
		rc.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
{
	PDTipos::PseudoArrayEstructura ae;
	if(o == PDS(ae.ObtenerClave()))
	{
		ae.LeerParametros(i);
		ae.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
{
	PDTipos::PseudoDebug db;
	if(o == PDS(db.ObtenerClave()))
	{
		db.LeerParametros(i);
		db.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
{
	PDTipos::PseudoBorrarVariable bv;
	if(o == PDS(bv.ObtenerClave()))
	{
		bv.LeerParametros(i);
		bv.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
{
	PDTipos::PseudoHerencia h;
	if(o == PDS(h.ObtenerClave()))
	{
		h.LeerParametros(i);
		h.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
{
	PDTipos::PseudoDireccionarPuntero dp;
	if(o == PDS(dp.ObtenerClave()))
	{
		dp.LeerParametros(i);
		dp.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
{
	PDTipos::PseudoMientras pm;
	if(o == PDS(pm.ObtenerClave()))
	{
		pm.LeerParametros(i);
		pm.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
{
	PDTipos::PseudoClaseContenida cc;
	if(o == PDS(cc.ObtenerClave()))
	{
		cc.LeerParametros(i);
		cc.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
{
	PDTipos::PseudoBorrarInteligente cc;
	if(o == PDS(cc.ObtenerClave()))
	{
		cc.LeerParametros(i);
		cc.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
