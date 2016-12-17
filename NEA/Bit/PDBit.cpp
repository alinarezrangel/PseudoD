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
	Bit::PseudoBitAnd l;
	if(o == PDS(l.ObtenerClave()))
	{
		l.LeerParametros(i);
		l.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
{
	Bit::PseudoBitOr l;
	if(o == PDS(l.ObtenerClave()))
	{
		l.LeerParametros(i);
		l.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
{
	Bit::PseudoBitXor l;
	if(o == PDS(l.ObtenerClave()))
	{
		l.LeerParametros(i);
		l.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
{
	Bit::PseudoBitNot l;
	if(o == PDS(l.ObtenerClave()))
	{
		l.LeerParametros(i);
		l.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
