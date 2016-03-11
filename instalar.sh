#!/bin/bash
# coding: utf-8

#################################
#  Instalador de PseudoD 1.0.0  #
# Por Alejandro Linarez Rangel  #
#################################

function instalarDesdeCero() {
	echo "Se esta instalando PseudoD en tu sistema..."
	echo "Copiando el codigo fuente..."
	mkdir /opt/pseudod/
	mkdir /opt/pseudod/bin/
	mkdir /opt/pseudod/libs/
	cp * /opt/pseudod/ -r
	cp *.o /opt/pseudod/libs/
	cp *.a /opt/pseudod/libs/
	mkdir /usr/lib/pseudod/
	mkdir /usr/lib/pseudod/2.0.0/
	mkdir /usr/lib/pseudod/plugins/
	cp ./bepd/* /usr/lib/pseudod/2.0.0/ -r
	echo "ya se creo /usr/lib/pseudod/ y /opt/pseudod/"
	echo "fijando los ejecutables..."
	cp ./PseudoD /opt/pseudod/bin/
	cp ./ipdc /opt/pseudod/bin/
	cp ./PseudoD /usr/bin/
	cp ./ipdc /usr/bin/
	echo "Listo, ya puedes usar PseudoD 2.0.0"
}

function instalarDesdeOtra() {
	echo "Se esta instalando PseudoD 2.0.0 actualizado en tu sistema..."
	echo "Copiando el codigo fuente"
	cp * /opt/pseudod/ -r
	cp *.o /opt/pseudod/libs/
	cp *.a /opt/pseudod/libs/
	mkdir /usr/lib/pseudod/2.0.0/
	cp ./bepd/* /usr/lib/pseudod/2.0.0/ -r
	echo "ya se creo /usr/lib/pseudod/ y /opt/pseudod/"
	echo "fijando los ejecutables..."
	cp ./PseudoD /opt/pseudod/bin/
	cp ./ipdc /opt/pseudod/bin/
	cp ./PseudoD /usr/bin/
	cp ./ipdc /usr/bin/
	echo "Listo, ya puedes usar PseudoD 2.0.0"
}

echo "Bienvenido al instalador de PseudoD 2.0.0"
echo "Si PseudoD ya esta instalado en tu sistema solo lo actualizara."
echo "Requisitos:"
echo "Debes estar como super-usuario"
echo "Debes tener make, g++ y soporte para c++11"
echo "Debes estar en el directorio del codigo fuente"
echo "Este instalador solo funciona en GNU/Linux"
echo "Deseas continuar?(s/n)"
read opt

if [ "$opt" = "n" ]
	then
		echo "Abortando..."
		exit
fi

echo "Compilando PseudoD..."
make

echo "Ya posees PseudoD instalado en tu sistema?(s/n)"
read opt

cp ./libpseudod.so /usr/lib/libpseudod.so

if [ "$opt" = "n" ]
	then
		instalarDesdeCero
	else
		instalarDesdeOtra
fi

echo "Deseas eliminar los ejecutables en el directorio actual?(s/n)"
read opt
if [ "$opt" = "s" ]
	then
		rm ./PseudoD
		rm ./libpseudod.so
		rm ./libpseudodsrc.a
		rm ./Dato.o
		rm ./pdbase.o
		echo "Eliminados..."
fi

echo "Ya puedes usar PseudoD con ipdc o PseudoD"
echo "Prueba con 'ipdc -f ./Ejemplos/HolaMundo.pd'"
echo "o 'PseudoD ./Ejemplos/HolaMundo.pd -- --'"
