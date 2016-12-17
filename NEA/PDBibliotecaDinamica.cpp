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
	BibliotecaDinamica::PseudoLlamar l;
	if(o == PDS(l.ObtenerClave()))
	{
		l.LeerParametros(i);
		l.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
{
	BibliotecaDinamica::PseudoLlamarOO l;
	if(o == PDS(l.ObtenerClave()))
	{
		l.LeerParametros(i);
		l.InscribirInstancia(PDDATA);
		PSEUDOD_NEA_SALIR_PC;
	}
}
