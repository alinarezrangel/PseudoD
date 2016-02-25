/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*** Este es PseudoD version 2.0.0                                      *****
*** Log de este archivo:                                               *****
*** Formato: DD/MM/YYYY: txt                                           *****
*** **** 14/09/2014: Se creo el archivo.                               *****
****************************************************************************
**************************************************************************/


{
	PDTipos::PseudoArray arr;
	if(o == PDS(arr.ObtenerClave()))
	{
		arr.LeerParametros(i);
		arr.InscribirInstancia(PDDATA);
	}
}
{
	PDTipos::PseudoClase cls;
	if(o == PDS(cls.ObtenerClave()))
	{
		cls.LeerParametros(i);
		cls.InscribirInstancia(PDDATA);
	}
}
{
	PDTipos::PseudoReferenciaClase rc;
	if(o == PDS(rc.ObtenerClave()))
	{
		rc.LeerParametros(i);
		rc.InscribirInstancia(PDDATA);
	}
}
{
	PDTipos::PseudoArrayEstructura ae;
	if(o == PDS(ae.ObtenerClave()))
	{
		ae.LeerParametros(i);
		ae.InscribirInstancia(PDDATA);
	}
}
{
	PDTipos::PseudoDebug db;
	if(o == PDS(db.ObtenerClave()))
	{
		db.LeerParametros(i);
		db.InscribirInstancia(PDDATA);
	}
}
{
	PDTipos::PseudoBorrarVariable bv;
	if(o == PDS(bv.ObtenerClave()))
	{
		bv.LeerParametros(i);
		bv.InscribirInstancia(PDDATA);
	}
}
{
	PDTipos::PseudoHerencia h;
	if(o == PDS(h.ObtenerClave()))
	{
		h.LeerParametros(i);
		h.InscribirInstancia(PDDATA);
	}
}
{
	PDTipos::PseudoDireccionarPuntero dp;
	if(o == PDS(dp.ObtenerClave()))
	{
		dp.LeerParametros(i);
		dp.InscribirInstancia(PDDATA);
	}
}
{
	PDTipos::PseudoMientras pm;
	if(o == PDS(pm.ObtenerClave()))
	{
		pm.LeerParametros(i);
		pm.InscribirInstancia(PDDATA);
	}
}
{
	PDTipos::PseudoClaseContenida cc;
	if(o == PDS(cc.ObtenerClave()))
	{
		cc.LeerParametros(i);
		cc.InscribirInstancia(PDDATA);
	}
}
{
	PDTipos::PseudoBorrarInteligente cc;
	if(o == PDS(cc.ObtenerClave()))
	{
		cc.LeerParametros(i);
		cc.InscribirInstancia(PDDATA);
	}
}
