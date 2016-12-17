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
