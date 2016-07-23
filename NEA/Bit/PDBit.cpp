/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*** Este es PseudoD version 2.0.0                                      *****
*** Log de este archivo:                                               *****
*** Formato: DD/MM/YYYY: txt                                           *****
*** **** 16/07/2016: Se creo el archivo.                               *****
****************************************************************************
**************************************************************************/

{
	Bit::PseudoBitAnd l;
	if(o == PDS(l.ObtenerClave()))
	{
		l.LeerParametros(i);
		l.InscribirInstancia(PDDATA);
	}
}
{
	Bit::PseudoBitOr l;
	if(o == PDS(l.ObtenerClave()))
	{
		l.LeerParametros(i);
		l.InscribirInstancia(PDDATA);
	}
}
{
	Bit::PseudoBitXor l;
	if(o == PDS(l.ObtenerClave()))
	{
		l.LeerParametros(i);
		l.InscribirInstancia(PDDATA);
	}
}
{
	Bit::PseudoBitNot l;
	if(o == PDS(l.ObtenerClave()))
	{
		l.LeerParametros(i);
		l.InscribirInstancia(PDDATA);
	}
}