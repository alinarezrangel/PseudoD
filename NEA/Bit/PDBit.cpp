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
