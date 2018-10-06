# Lenguaje de Programación PseudoD #

[![BuildStatus][BuildStatus]](https://travis-ci.org/alinarezrangel/PseudoD)

PseudoD es un lenguaje de programación interpretado, escrito en C++14 con
características de alto y medio nivel. Es orientado a objetos y posee una
sintaxis en español.

## Características ##

* Orientación a objetos
* Biblioteca Estándar llena de utilidades
* Núcleo Extensible de la Aplicación (**NEA**) que permite extender el lenguage
fácilmente utilizando C++
* Sistema de Módulos Dinámicos (**Bibliotecas Dinámicas**) que permite cargar
módulos en C++ desde PseudoD sin recompilar el núcleo

## Obtén PseudoD ##

Este repositorio contiene la más reciente versión de PseudoD, por ahora no
existen binarios precompilados (aún se trabaja en eso) por ello, si deseas
obtener PseudoD debes compilarlo (es muy fácil):

* Verifica que posees las *dependencias* (véase más abajo)
* Clona este repositorio con
`git clone https://github.com/alinarezrangel/PseudoD.git` o descarga una
[versión estable][VersionesEstables]
* Extrae (si descargástes una versión estable) y entra al directorio del código
fuente (por ejemplo: si descargástes PseudoD 2.1.0 y lo extraes en
`pseudod-code`, entonces entra a `pseudod-code`). Si clonastes el repositorio
con Git, simplemente entra al directorio recién creado.
* Si deseas instalar PseudoD, simplemente ejecuta `./instalar.sh` (o
`sudo ./instalar.sh`) para iniciar el instalador, es recomendable instalar
PseudoD para utilizarlo, aunque si no deseas instalarlo de manera global, puedes
fijar la variable de entorno `RUTA_PSEUDOD_INSTALACION` a la ruta donde deseas
instalar PseudoD y luego ejecutar el instalador (por ejemplo, para instalar
PseudoD en `./install/` solo utiliza
`export RUTA_PSEUDOD_INSTALACION="./install/"; ./instalar.sh`).
* Luego de instalar PseudoD (esto lo compila automáticamente) y ejecutar el
código en **BASH** que el instalador provee basta con ejecutar
`PseudoD $RUTA_PSEUDOD_INSTALACION/Ejemplos/HolaMundo.pd -d -d` para ver el
*"Hola Mundo"*.
* Si no deseas instalar PseudoD, solo ejecuta `make` y los binarios estarán
en el directorio actual.

## Dependencias ##

PseudoD utiliza la biblioteca estándar de C++11 y la biblioteca estándar de
C11, por ello es necesario poseer un compilador con soporte de C++11 y C11
para compilar PseudoD (GCC y G++ 4.8 o superior). PseudoD también utiliza
parte de las APIs POSIX, como `dlfcn.h` para algunas tareas, esto implica que
PseudoD no puede compilarse en sistemas Microsoft Windows sin utilizar
herramientas adicionales: **Cygwin** es necesario para esta tarea.

En resumen:

* Compilador con soporte de C++11 y C11 o superior (gcc o g++ 4.8 o superior)
* Cygwin si se compila en sistemas Microsoft Windows
* GNU Make o compatible (en general, algún Make).

### Suite de pruebas ###

La suite de pruebas tiene los mismos requerimientos base que PseudoD pero los
siguientes programas deben estar instalados además:

* `find`
* `sed`
* `cat`
* `diff`
* `head`
* `wc`
* `bc`
* `touch`
* `mkdir`
* Los binarios resultantes de compilar e instalar PseudoD.

Opcionalmente, si deseas que la salida de las pruebas este coloreada (util para
detectar pruebas fallidas), es necesario poseer el programa `tput`.

## LEEME ##

Es recomendable leer los siguientes archivos:

* `LEEME.txt` ([LEEME.txt](LEEME.txt)): Este archivo contiene más información
sobre PseudoD.
* `CONTRIBUTE.txt` ([CONTRIBUTE.txt](CONTRIBUTE.txt)): Como contribuir con el
proyecto y estilo de código.
* `ESTRUCTURA.txt` ([ESTRUCTURA.txt](ESTRUCTURA.txt)): Este archivo explica
la estructura del directorio.
* `bepd/LEEME.txt` ([bepd/LEEME.txt](bepd/LEEME.txt)): LEEME de la BEPD.
* `tests/LEEME.txt` ([tests/LEEME.txt](tests/LEEME.txt)): LEEME de la suite de
pruebas.

## Documentación y ayuda ##

La página oficial de PseudoD se encuentra en
[http://pseudod.sourceforge.net](http://pseudod.sourceforge.net).

## Ejemplos ##

```pseudod
[ Esto es un comentario ]
[
Puede tener múltiples líneas
]

[
En PseudoD, las cadenas de carácteres están delimitadas por `{}` en vez de `""`
]

escribir {Hola Mundo}
nl [ nl es "Nueva Línea" e imprimir el fin de línea ]

[ Vayamos a algo más complejo: una clase anfitríon ]

[ PseudoD soporta caracteres Unicode UTF-8 en los nombres: ]
clase Anfitríon hereda Objeto
	metodo saludarA
finclase

[
Los parámetros comienzan con "con/de" y son una lista de nombres separados
por "y", el parámtro "yo" es como el "self" de Python o el "this" de C++
y contiene a la instancia actual.
]
metodo Anfitríon#saludarA con yo y nombre_persona
	escribir {Bienvenido }
	escribir nombre_persona
	nl
finmetodo

instancia Anfitríon anfitríon

llamar anfitríon#saludarA {Pepe} finargs
llamar anfitríon#saludarA {a PseudoD} finargs

[
La BEPD (Biblioteca Estándar de PseudoD) posee clases, funciones y
procedimientos, entre otras herramientas útiles:
]

[ Funciones trigonométricas: ]
utilizar mt/trigonometria.pseudo

[ Imprime el seno de 7 (`sin(7)`) ]
escribir llamar Seno {7} finargs
nl
```

## Licencia ##

PseudoD esta licenciado bajo la Licencia de Apache 2.0.

[LICENSE](LICENSE)

[BuildStatus]: https://travis-ci.org/alinarezrangel/PseudoD.svg?branch=master
[VersionesEstables]: http://pseudod.sourceforge.net/download.html
