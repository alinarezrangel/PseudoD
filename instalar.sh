#!/bin/bash
# coding: utf-8

#################################
#  Instalador de PseudoD 2.3.0  #
# Por Alejandro Linarez Rangel  #
#################################
# Copyright 2016-2018 Alejandro Linarez Rangel
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#################################

RUTA_INSTAL_GLOBAL="/opt/pseudod"
RUTA_INSTAL_LOCAL="$HOME/.pseudod"
STEPBYSTEP=""

echo "Instalador de PseudoD 2.3.0-alpha"
echo
echo "Ahora se realizarán una serie de preguntas, puede terminar la instalación"
echo "en cualquier momento ingresando 'q' pero no se garantiza que los cambios"
echo "hechos sean reversibles. Puede ejecutar este instalador con la opcion"
echo "'--step-by-step' para indicar si se desea utilizar la ejecución"
echo "paso por paso. En todas las preguntas habrá una respuesta predeterminada"
echo "que generalmente es la mejor opción en muchos sistemas, más puede no"
echo "serlo en el actual. Esta 'opción predeterminada' estará entre llaves []."
echo
echo "Recuerde que las opciones son afectadas por las MaYuScUlAs ('s' no es"
echo "lo mismo que 'S')."
echo

echo "Deseas continuar? (s = si / [n] = no):"
read opt
case "$opt" in
	s)
		;;
	*)
		echo "Abortando..."
		exit
		;;
esac

if [ "$1" = "--step-by-step" ]; then
	echo "Detectada ejecución paso por paso..."
	echo "Ahora entre cada acción se esperará 1 segundo"
	STEPBYSTEP="x"
fi

echo
echo "Detectando ruta de instalación..."

RUTA_PSEUDOD_INSTALACION_TG="/opt/pseudod"

if [ -z ${RUTA_PSEUDOD_INSTALACION+x} ]; then
	echo "No esta configurada"
	echo
	echo "Parece que no has configurado la ruta donde deseas instalar PseudoD,"
	echo "esto generalmente no es importante a menos que desees utilizar una"
	echo "instalación personalizada:"
	echo
	echo "La instalación global permitirá a todos los usuarios del sistema"
	echo "utilizar PseudoD, la local solo permitirá a su propio usuario utilizar"
	echo "PseudoD y la personalizada le permitirá seleccionar donde desea"
	echo "instalar PseudoD. Tip: Normalmente, solo los usuarios avanzados"
	echo "utilizarán la instalación personalizada, y generalmente la instalación"
	echo "global es la mejor. Cabe resaltar que, la instalación global solo podrá"
	echo "ejecutarse si usted posee permisos de administrador."
	echo
	echo "Que tipo de instalación deseas utilizar?"
	echo " ([g] = global / l = local / p = personalizada)"
	echo

	read opt
	case "$opt" in
		q)
			echo "Abortando..."
			exit
			;;
		p)
			echo "Seleccionastes la instalación personalizada:"
			echo
			echo "En este modo, tu ingresas la ruta donde se debería instalar"
			echo "PseudoD, en la instalación local esta es $RUTA_INSTAL_LOCAL y"
			echo "en la global es $RUTA_INSTAL_GLOBAL pero tu puedes ahora"
			echo "seleccionar una propia."
			echo
			echo "No ingresar una ruta utilizara la predeterminada."
			echo
			echo "Ingresa la ruta donde se instalará PseudoD ([$RUTA_INSTAL_GLOBAL]):"
			echo

			read RUTA_PSEUDOD_INSTALACION_TG

			if [ -z "$RUTA_PSEUDOD_INSTALACION_TG" ]; then
				echo "No se ingresó ruta alguna, utilizando $RUTA_INSTAL_GLOBAL"

				RUTA_PSEUDOD_INSTALACION_TG="$RUTA_INSTAL_GLOBAL"
			fi

			P=$(echo "$RUTA_PSEUDOD_INSTALACION_TG" | sed "s/\/$//")
			RUTA_PSEUDOD_INSTALACION_TG="$P"

			echo "La ruta donde se instalará PseudoD es $RUTA_PSEUDOD_INSTALACION_TG"
			;;
		l)
			echo "Utilizando una instalación local..."

			RUTA_PSEUDOD_INSTALACION_TG="$RUTA_INSTAL_LOCAL"

			echo "La ruta donde se instalará PseudoD es $RUTA_PSEUDOD_INSTALACION_TG"
			;;
		*)
			echo "Utilizando una instalación global..."

			RUTA_PSEUDOD_INSTALACION_TG="$RUTA_INSTAL_GLOBAL"

			echo "La ruta donde se instalará PseudoD es $RUTA_PSEUDOD_INSTALACION_TG"
			;;
	esac
else
	echo "Configurada a $RUTA_PSEUDOD_INSTALACION"
	echo
	echo "Desea cambiar este valor? (s = si / [n] = no)"
	echo

	read opt
	case "$opt" in
		q)
			echo "Abortando..."
			exit
			;;
		s)
			echo "Ahora tu puedes ingresar la ruta donde se debería instalar"
			echo "PseudoD, en la instalación local esta es $RUTA_INSTAL_LOCAL y"
			echo "en la global es $RUTA_INSTAL_GLOBAL pero tu puedes ahora"
			echo "seleccionar una propia."
			echo
			echo "No ingresar una ruta utilizara la predeterminada."
			echo
			echo "Ingresa la ruta donde se instalará PseudoD ([$RUTA_INSTAL_GLOBAL]):"
			echo

			read RUTA_PSEUDOD_INSTALACION_TG

			if [ -z "$RUTA_PSEUDOD_INSTALACION_TG" ]; then
				echo "No se ingresó ruta alguna, utilizando $RUTA_INSTAL_GLOBAL"

				RUTA_PSEUDOD_INSTALACION_TG="$RUTA_INSTAL_GLOBAL"
			fi

			P=$(echo "$RUTA_PSEUDOD_INSTALACION_TG" | sed "s/\/$//")
			RUTA_PSEUDOD_INSTALACION_TG="$P"

			echo "La ruta donde se instalará PseudoD es $RUTA_PSEUDOD_INSTALACION_TG"
			;;
		*)
			echo "No se cambiará la ruta, continuando con la instalación..."
			;;
	esac
fi

echo
echo "[Verificando] PseudoD se instalará en $RUTA_PSEUDOD_INSTALACION_TG"
echo "> RUTA_PSEUDOD_INSTALACION = $RUTA_PSEUDOD_INSTALACION_TG"
echo "Exportando..."

export RUTA_PSEUDOD_INSTALACION="$RUTA_PSEUDOD_INSTALACION_TG"

echo "Exportado."

if [ -z "$STEPBYSTEP" ]; then
	sleep 1
fi

echo
echo "Detectando si el directorio '$RUTA_PSEUDOD_INSTALACION' existe..."

if [ ! -d "$RUTA_PSEUDOD_INSTALACION" ]; then
	echo "No existe, creandolo..."
	mkdir "$RUTA_PSEUDOD_INSTALACION"
else
	echo "Si existe. PseudoD ya estaba instalado en este sistema..."
fi

if [ -z "$STEPBYSTEP" ]; then
	sleep 1
fi

echo "Verificando la estructura del directorio..."

if [ -d "$RUTA_PSEUDOD_INSTALACION/lib/" ]; then
	echo "Existe el directorio lib/"
else
	echo "No existe el directorio lib/, creandolo..."
	mkdir "$RUTA_PSEUDOD_INSTALACION/lib/"
fi

if [ -d "$RUTA_PSEUDOD_INSTALACION/bepd/" ]; then
	echo "Existe el directorio bepd/"
else
	echo "No existe el directorio bepd/, creandolo..."
	mkdir "$RUTA_PSEUDOD_INSTALACION/bepd/"
fi

if [ -d "$RUTA_PSEUDOD_INSTALACION/bin/" ]; then
	echo "Existe el directorio bin/"
else
	echo "No existe el directorio bin/, creandolo..."
	mkdir "$RUTA_PSEUDOD_INSTALACION/bin/"
fi

if [ -d "$RUTA_PSEUDOD_INSTALACION/Ejemplos/" ]; then
	echo "Existe el directorio Ejemplos/"
else
	echo "No existe el directorio Ejemplos/, creandolo..."
	mkdir "$RUTA_PSEUDOD_INSTALACION/Ejemplos/"
fi

if [ -z "$STEPBYSTEP" ]; then
	sleep 1
fi

echo "Ahora el directorio si posee la estructura válida."
echo
echo "Instalando PseudoD en $RUTA_PSEUDOD_INSTALACION..."
echo "Compilando PseudoD"
echo "Esto puede tomar un tiempo:"
echo
echo "Veo que posees aproximadamente $(nproc) cores para utilizar,"
echo "la compilación puede ser más rápida si se utilizan todos."
echo
echo -n "Deseas utilizar todos los $(nproc) cores para compilar?"
echo " (s = si / [n] = no)"
echo

JOPT=""

read opt
case "$opt" in
	q)
		echo "Intentando abortar, no se revierten cambios en"
		echo "$RUTA_PSEUDOD_INSTALACION..."
		echo "Abortando..."
		exit
		;;
	s)
		echo "Utilizando $(nproc) cores..."
		JOPT="-j $(nproc)"
		;;
	*)
		echo "Utilizando solo un core..."
		;;
esac

echo
echo "Compilando..."
echo

make $JOPT

echo
echo "Compilación terminada..."
echo "Detectando versión de PseudoD..."

PDVERSION=$(./pseudod --version | sed "s/PseudoD //")

if [ -z "$STEPBYSTEP" ]; then
	sleep 1
fi

echo "PseudoD versión $PDVERSION"
echo

echo "Moviendo las librerias..."

LIBPSEUDOD_TG="libpseudod.so"
LIBPSEUDODSRC_TG="libpseudodsrc.a"
PSEUDOD_TG="pseudod"
BEPD_TG="bepd/"

if [ -e "$RUTA_PSEUDOD_INSTALACION/lib/libpseudod.so" ]; then
	echo "Existe otro lib/libpseudod.so en la instalación, cambiando..."

	# No podemos detectar la versión de bibliotecas dinámicas

	echo "Lamentablemente no he logrado detectar la version utilizada,"
	echo "desea reemplazarla por la actual? (perdería el NEA para siempre,"
	echo "si utiliza varias versiones de PseudoD a la vez esto puede dañar"
	echo "a las demas versiones) (s = si / [n] = no)"
	echo
	echo "Tip: si está actualizando PseudoD y solo posee una versión instalada,"
	echo "y no desea poseer más de una (este es el caso común) es recomendable"
	echo "que acceda (responda 's')."
	echo

	read opt
	case "$opt" in
		q)
			echo "Intentando abortar, no se revierten cambios en"
			echo "$RUTA_PSEUDOD_INSTALACION..."
			echo "Abortando..."
			exit
			;;
		s)
			echo "Reemplazando el NEA..."
			LIBPSEUDOD_TG="libpseudod.so"
			;;
		*)
			echo "No se reemplazará el NEA (configurando para"
			echo " libpseudod.so.$PDVERSION)"
			LIBPSEUDOD_TG="libpseudod.so.$PDVERSION"
			;;
	esac
fi

if [ -z "$STEPBYSTEP" ]; then
	sleep 1
fi

if [ -e "$RUTA_PSEUDOD_INSTALACION/lib/libpseudodsrc.a" ]; then
	echo "Existe otro lib/libpseudodsrc.a en la instalación, cambiando..."

	# No podemos detectar la versión de bibliotecas estáticas

	echo "Lamentablemente no he logrado detectar la version utilizada,"
	echo "desea reemplazarla por la actual? (perdería el NIA para siempre,"
	echo "si compila software para varias versiones de PseudoD a la vez, esto"
	echo "podría prohibir que se compile software para otras versiones)"
	echo " (s = si / [n] = no)"
	echo
	echo "Tip: si está actualizando PseudoD y solo posee una versión instalada,"
	echo "y no desea poseer más de una (este es el caso común) es recomendable"
	echo "que acceda (responda 's')."
	echo

	read opt
	case "$opt" in
		q)
			echo "Intentando abortar, no se revierten cambios en"
			echo "$RUTA_PSEUDOD_INSTALACION..."
			echo "Abortando..."
			exit
			;;
		s)
			echo "Reemplazando el NIA..."
			LIBPSEUDODSRC_TG="libpseudodsrc.so"
			;;
		*)
			echo "No se reemplazará el NIA (configurando para"
			echo " libpseudodsrc.so.$PDVERSION)"
			LIBPSEUDODSRC_TG="libpseudodsrc.a.$PDVERSION"
			;;
	esac
fi

if [ -z "$STEPBYSTEP" ]; then
	sleep 1
fi

export RUTA_PSEUDOD_LIB="$RUTA_PSEUDOD_INSTALACION/lib/$LIBPSEUDOD_TG"
export RUTA_PSEUDOD_DEV_LIB="$RUTA_PSEUDOD_INSTALACION/lib/$LIBPSEUDODSRC_TG"

echo "> libpseudod.so..."
echo "> RUTA_PSEUDOD_LIB = $RUTA_PSEUDOD_LIB"
cp ./libpseudod.so "$RUTA_PSEUDOD_LIB"

echo "> libpseudodsrc.a..."
echo "> RUTA_PSEUDOD_DEV_LIB = $RUTA_PSEUDOD_DEV_LIB"
cp ./libpseudodsrc.a "$RUTA_PSEUDOD_DEV_LIB"

if [ -z "$STEPBYSTEP" ]; then
	sleep 1
fi

echo
echo "Moviendo los binarios..."

if [ -e "$RUTA_PSEUDOD_INSTALACION/bin/pseudod" ]; then
	echo "Existe un archivo en bin/pseudod, este puede ser otra versión de"
	echo "PseudoD. Desea reemplazar este binario por el actual? (perdería"
	echo "esta versión de PseudoD para siempre, si posee varias versiones de"
	echo "PseudoD a la vez, esto prohibirá utilizar la versión antigua)"
	echo " (s = si / [n] = no)"
	echo
	echo "Tip: si está actualizando PseudoD y solo posee una versión instalada,"
	echo "y no desea poseer más de una (este es el caso común) es recomendable"
	echo "que acceda (responda 's')."
	echo

	read opt
	case "$opt" in
		q)
			echo "Intentando abortar, no se revierten cambios en"
			echo "$RUTA_PSEUDOD_INSTALACION..."
			echo "Abortando..."
			exit
			;;
		s)
			echo "Reemplazando el binario PseudoD antiguo..."
			PSEUDOD_TG="pseudod"
			;;
		*)
			echo "No se reemplazará el binario antiguo (configurando para"
			echo " pseudod$PDVERSION)"
			PSEUDOD_TG="pseudod$PDVERSION"
			;;
	esac
fi

export RUTA_PSEUDOD_BIN="$RUTA_PSEUDOD_INSTALACION/bin/$PSEUDOD_TG"

echo "> PseudoD"
echo "> RUTA_PSEUDOD_BIN = $RUTA_PSEUDOD_BIN"
cp ./pseudod "$RUTA_PSEUDOD_BIN"

if [ -z "$STEPBYSTEP" ]; then
	sleep 1
fi

echo "Nota: ipdc siempre es sobreescrito."

echo "> ipdc $(dirname "$RUTA_PSEUDOD_BIN")"
echo "> RUTA_PSEUDOD_BIN = $RUTA_PSEUDOD_BIN"
cp ./ipdc $(dirname "$RUTA_PSEUDOD_BIN")/ipdc

if [ -z "$STEPBYSTEP" ]; then
	sleep 1
fi

echo
echo "Moviendo la BEPD..."

if [ -d "$RUTA_PSEUDOD_INSTALACION/bepd/bepd/" ]; then
	echo "Existe una BEPD instalada, este puede ser de otra versión de"
	echo "PseudoD. Desea reemplazar esta BEPD por la actual? (perdería"
	echo "esta versión BEPD para siempre, si posee varias versiones de"
	echo "PseudoD a la vez, esto prohibirá utilizar la versión antigua)"
	echo " (s = si / [n] = no)"
	echo
	echo "Tip: si está actualizando PseudoD y solo posee una versión instalada,"
	echo "y no desea poseer más de una (este es el caso común) es recomendable"
	echo "que acceda (responda 's')."
	echo

	read opt
	case "$opt" in
		q)
			echo "Intentando abortar, no se revierten cambios en"
			echo "$RUTA_PSEUDOD_INSTALACION..."
			echo "Abortando..."
			exit
			;;
		s)
			echo "Reemplazando la BEPD antigua..."
			BEPD_TG="bepd/"
			;;
		*)
			echo "No se reemplazará la BEPD antigua (configurando para"
			echo " bepd/$PDVERSION/)"
			BEPD_TG="$PDVERSION/"
			;;
	esac
fi

if [ -z "$STEPBYSTEP" ]; then
	sleep 1
fi

export RUTA_PSEUDOD="$RUTA_PSEUDOD_INSTALACION/bepd/$BEPD_TG"

if [ ! -d "$RUTA_PSEUDOD" ]; then
	echo "RUTA_PSEUDOD ($RUTA_PSEUDOD) no existe, creandolo..."
	mkdir "$RUTA_PSEUDOD"
	echo "Creado"
fi

if [ -z "$STEPBYSTEP" ]; then
	sleep 1
fi

echo "> bepd/*"
echo "> RUTA_PSEUDOD = $RUTA_PSEUDOD"
cp -R bepd/* "$RUTA_PSEUDOD"

if [ -z "$STEPBYSTEP" ]; then
	sleep 1
fi

echo
echo "Moviendo los ejemplos..."

if [ ! -d "$RUTA_PSEUDOD_INSTALACION/Ejemplos/$PDVERSION/" ]; then
	echo "El directorio de ejemplos no existe, creandolo..."
	mkdir "$RUTA_PSEUDOD_INSTALACION/Ejemplos/$PDVERSION/"
fi

echo "> Ejemplos/*"
echo "> // $RUTA_PSEUDOD_INSTALACION/Ejemplos/$PDVERSION/"
cp -R Ejemplos/* $RUTA_PSEUDOD_INSTALACION/Ejemplos/$PDVERSION/

if [ -z "$STEPBYSTEP" ]; then
	sleep 1
fi

echo
echo
echo "La instalación está casi completa, solo falta configurar las variables"
echo "globales. Debido a que no hay manera estándar para hacer esto,"
echo "el usuario que desee utilizar PseudoD deberá hacerlo:"
echo
echo "Código para Shell:"
echo
echo "export RUTA_PSEUDOD=\"$RUTA_PSEUDOD\""
echo "export RUTA_PSEUDOD_BIN=\"$RUTA_PSEUDOD_BIN\""
echo "export RUTA_PSEUDOD_LIB=\"$RUTA_PSEUDOD_LIB\""
echo "export RUTA_PSEUDOD_INSTALACION=\"$RUTA_PSEUDOD_INSTALACION/\""
echo "export RUTA_PSEUDOD_DEV_LIB=\"$RUTA_PSEUDOD_DEV_LIB\""
echo "export PATH=\"\$PATH:$(dirname $RUTA_PSEUDOD_BIN)\""
echo
echo "Fin del código."
echo
echo "Instalación completa, intente con:"
echo " $ ipdc \"\$RUTA_PSEUDOD_INSTALACION/Ejemplos/$PDVERSION/HolaMundo.pd\""
echo "Para probar."
echo

if [ -z "$STEPBYSTEP" ]; then
	sleep 1
fi
