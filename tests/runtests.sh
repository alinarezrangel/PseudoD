#!/bin/sh

## PseudoD fue creado por Alejandro Linarez Rangel
# Copyright 2016 Alejandro Linarez Rangel
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

# Basicamente es ejecutar esto para cada prueba:
#
# test-[name]: [name]/espr.txt ...
#     pseudod [name]/test.pd > [name]/salida.txt < [name]/entrada.txt
#     diff [name]/salida.txt [name]/espr.txt > [name]/diff.txt
#     local_diff_cpy=$(wc -l [name]/diff.txt | cut -d " " -f 1)
#     if [ "$local_diff_cpy" != "0" ]; then
#         echo Test failed
#     else
#         echo Test passed
#     fi

###############################################################################
# Variables globales de configuración:

# 1 si los binarios de PseudoD estan en el código fuente (tal como justo
# después de ejecutar `make`). 0 si PseudoD esta instalado globalmente o para
# el usuario actual.
IN_SOURCE=1

# 1 si se desea una salida con color, 0 en caso contrario
USE_COLOR=0

# Colores
CR_TEST_FAILED=1
CR_TEST_PASSED=2
CR_MSG=7
CR_MSG_FL=5
CR_SRC=6

###############################################################################

ayuda() {
	echo "Sistema automatizado de pruebas de PseudoD"
	echo -n "Solamente ejecuta ./rubtests.sh y este programa ejecutará todas las"
	echo " pruebas"
	echo
	echo "Opciones:"
	echo
	echo "  $0 -h|--help|--ayuda       Muestra esta ayuda y termina"
	echo "  $0 --in-source             Especifica que se usen los binarios desde"
	echo "                             el código fuente (predeterminado)"
	echo "  $0 --global                Especifica que se usen los binarios"
	echo "                             instalados globalmente"
	echo "  $0 --use-color             Habilita la salida con color"
	echo "  $0 --no-use-color          No imprime mensajes con color"
	echo

	exit
}

imprimir() {
	msg=$2
	opts=

	if [ "$2" = "-n" ]; then
		msg="$3"
		opts="-n"
	fi

	if [ "$USE_COLOR" = 1 ]; then
		tput setaf $1

		if [ $1 -eq 1 ]; then
			tput setab 0
		fi

		echo $opts "$msg"
		tput sgr0
	else
		echo $opts "$msg"
	fi
}

for i in $@; do
	if [ "$i" = "-h" ]; then ayuda; fi
	if [ "$i" = "--help" ]; then ayuda; fi
	if [ "$i" = "--ayuda" ]; then ayuda; fi

	if [ "$i" = "--global" ]; then IN_SOURCE=0; fi
	if [ "$i" = "--no-global" ]; then IN_SOURCE=1; fi
	if [ "$i" = "--in-source" ]; then IN_SOURCE=1; fi
	if [ "$i" = "--no-in-source" ]; then IN_SOURCE=0; fi

	if [ "$i" = "--use-color" ]; then USE_COLOR=1; fi
	if [ "$i" = "--no-use-color" ]; then USE_COLOR=0; fi

	if [ "$i" = "--" ]; then
		break
	fi
done

###############################################################################
# Variables de trabajo:

imprimir $CR_MSG "Iniciando sistema de pruebas..."

# Nombres de los archivos utilidados para determinar que directorios son
# pruebas y cuales no:
fname='prueba.txt' # Para `find`
sfname='prueba\.txt' # Para `sed`

imprimir $CR_MSG "Buscando pruebas..."

# `folders` contiene todos los directorios que son pruebas
folders=$(
	find . -type d |
	find -type f -name $fname |
	sed 's/\.\///' |
	sed "s/\/$sfname//"
)

# Posee cualquier cadena de caracteres de tamaño no cero si se ejecuto al menos
# una prueba.
at_least_one_test=""

nth_tests_to_do=$(echo "$folders" | wc -l)
nth_tests_executed=0 # Pruebas ejecutadas.
nth_tests_passed=0 # Pruebas pasadas con éxito.

imprimir $CR_MSG "Encontradas $nth_tests_to_do pruebas"

###############################################################################
# Programa:

imprimir $CR_MSG "Ejecutando las pruebas..."

for i in $folders; do
	at_least_one_test="x"
	nth_tests_executed=$((nth_tests_executed + 1))

	cd "./$i"

	# Descripción rápida de la prueba:
	brief=$(head prueba.txt -n 1)

	echo
	imprimir $CR_MSG "Iniciando prueba [$i]"
	imprimir $CR_MSG_FL "[$i]>> $brief"

	if [ "$IN_SOURCE" = 1 ]; then
		imprimir $CR_SRC "$ ../../pseudod $i/test.pd -b ../../bepd -l ../../libpseudod.so"

		../../pseudod test.pd -b ../../bepd/ -l ../../libpseudod.so \
			< entrada.txt \
			> salida.txt 2>&1
	else
		imprimir $CR_SRC "$ pseudod $i/test.pd -d -d"

		pseudod test.pd \
			< entrada.txt
			> salida.txt 2>&1
	fi

	imprimir $CR_SRC "$ diff salida.txt espr.txt"

	diff salida.txt espr.txt > diff.txt

	difference=$(cat diff.txt)

	if [ "$difference" != "" ]; then
		# No paso la prueba
		imprimir $CR_TEST_FAILED "[$i] La prueba no se paso con éxito"
	else
		# Paso la prueba
		imprimir $CR_TEST_PASSED "[$i] La prueba se paso con éxito"

		nth_tests_passed=$((nth_tests_passed + 1))
	fi

	cd ..

	imprimir $CR_MSG "Se han pasado $nth_tests_passed de $nth_tests_executed pruebas"
done

if [ -z "$at_least_one_test" ]; then
	imprimir $CR_MSG "No se encontro ninguna prueba"

	exit 1
fi

echo

# Porcentaje de pruebas pasadas con éxito
tests_passed=$(
	echo "scale=2.0; ($nth_tests_passed * 100.0) / $nth_tests_executed" |
	bc -l
)

imprimir $CR_MSG -n "El $tests_passed% ($nth_tests_passed de $nth_tests_executed) "
imprimir $CR_MSG "de las pruebas se paso con éxito"
echo

exit
