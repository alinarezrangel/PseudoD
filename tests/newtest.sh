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

ayuda() {
	echo "Sistema automatizado de pruebas de PseudoD"
	echo "Utiliza este programa para crear una nueva prueba."
	echo
	echo "Opciones:"
	echo
	echo "  $0 -h|--help|--ayuda       Muestra esta ayuda y termina"
	echo

	exit
}

if [ "$1" = "-h" ]; then ayuda; fi
if [ "$1" = "--help" ]; then ayuda; fi
if [ "$1" = "--ayuda" ]; then ayuda; fi

###############################################################################

echo "Bienvenido al sistema de creación de pruebas de PseudoD"
echo
echo "Ahora se realizarán una serie de preguntas simples para crear la nueva"
echo "prueba, puedes cancelar el proceso y no crear una prueba utilizando la"
echo "tecla [q]."
echo
echo "Deseas continuar? (s = si / [n] = no)"

read opt

if [ "$opt" != "s" ]; then
	exit
fi

echo
echo "Ingresa el nombre de la prueba."
echo
echo "Los nombres de pruebas generalmente son solo en minusculas, sin espacios"
echo "y son autodescriptivos (por ejemplo, para una prueba de los elementos"
echo "condicionales de PseudoD, un buen nombre seria [condicionales])."
echo
echo -n "> "

read test_name

if [ '(' -z "$test_name" ')' -o '(' -d "$test_name" ')' ]; then
	echo "La prueba [$test_name] ya existe o [$test_name] es un nombre invalido"

	exit 1
fi

echo "Creando prueba llamada [$test_name]..."

mkdir "$test_name"

touch "$test_name/prueba.txt"
touch "$test_name/test.pd"
touch "$test_name/fmt.txt"
touch "$test_name/espr.txt"
touch "$test_name/salida.txt"
touch "$test_name/entrada.txt"
touch "$test_name/diff.txt"

echo "Estructura básica creada."

echo "Rellenando archivos..."
echo

echo "Ingresa la descripción corta de la prueba."
echo
echo "No debe ocupar más de una única línea y debe describir de manera breve"
echo "y concisa la prueba. Por ejemplo, para una prueba de los condicionales"
echo "de PseudoD, la descripción corta podría ser 'Prueba de los condicionales'"
echo "pero sin las comillas. Recuerda no agregar un punto al final de la"
echo "línea, eso se hace automáticamente."
echo

echo -n "> "

read test_brief

cat > "$test_name/prueba.txt" <<EOS
$test_brief.

Describe a mayor medida tu prueba.
EOS

echo "Se terminó de rellenar los archivos."

echo

echo "Prueba creada! ahora puedes modificarla en el directorio [$test_name]"
echo "y probarla con el comando ./runtests.sh"
