# Estructura del Proyecto #

## Archivos ##

Los directorios `NIA` y `NEA` contienen la mayor parte del intérprete. Por ahora, mientras el intérprete es reescrito, tanto el código de la versión 2 como la 3 de PseudoD se encuentran en los mismos directorios lo que complica un poco la estructura del proyecto. Cuando PseudoD 3 termine de ser escrito, la localización de los archivos cambiará drásticamente.

- `Ejemplos/`: Algunos ejemplos de programas en PseudoD 2.3.
- `tests/`: Pruebas para PseudoD 2.

## PseudoD 2.3 ##

No es recomendable trabajar con PseudoD 2, ya que es increiblemente complejo (más de lo que debería ser) y estoy reescribiendo varias partes de este como PseudoD 3.

- `NIA/Main.cpp`: El punto de entrada principal del ejecutable `pseudod`.
- `NIA/interprete.(hh|cpp)`: La parte principal del intérprete, delega bastante (pero no todo) el trabajo al NEA y al manejador de memoria.
- `NIA/pseudod.(hh|cpp)`: El punto de entrada del NEA. Incluye textualmente (con `#include`) a `NIA/codefile.cpp`, `NIA/definitionfile.cpp`, `NIA/includefile.hh` y `NIA/includefilelib.hh`.
- `NIA/codefile.cpp`: Parte del ciclo principal del intérprete. Compara cada instrucción recibida con una registrada en el NEA.
- `NIA/definitionfile.cpp`: Las definiciones de las clases declaradas en `NIA/includefile.hh`.
- `NIA/includefile.hh`: Declara las clases que implementan las instrucciones del NEA.
- `NIA/includefilelib.hh`: Incluye las dependencias de las clases de `NIA/includefile.hh`.
- `NEA/Tipos.(hh|cpp)`, `NEA/PDTipos.cpp`: Las instrucciones básicas del NEA.
- `NEA/PDDatos.(hh|cpp)`: El manejador de memoria.
- `NEA/BibliotecaDinamica.(hh|cpp)`, `NEA/INBibliotecaDinamica.hh`, `NEA/PDBibliotecaDinamica.cpp`: Instrucciones para cargar bibliotecas de manera dinámica en PseudoD.
- `NEA/Localizacion/`: Instrucciones para manejar la localización ("locale") de archivos y la entrada y salida estándar.
- `NEA/Bit/`: Instrucciones para manipular bits de números enteros.
- `NEA/interno/nmemoic.(hh|cpp)`: Estructura de datos para manejar las palabras clave del lenguaje.
- `NEA/interno/token.(hh|cpp)`: Tokens.
- `NEA/interno/tokenizer.(hh|cpp)`: El tokenizador de PseudoD 2.3.
- `NEA/interno/data.(hh|cpp)`: Estructuras de datos, funciones y clases generales utilizadas por todo el intérprete.

## PseudoD 3 ##

**Nota**: Recuerda que PseudoD 3 es aún experimental.

- `NIA/Main.cpp`: Punto de entrada del ejecutable `pseudod`.
- `NEA/interno/nmemoic.(hh|cpp)`: Estructura de datos para manejar las palabras clave del lenguaje.
- `NEA/interno/token.(hh|cpp)`: Tokens.
- `NEA/interno/new_tokenizer.(hh|cpp)`: El tokenizador de PseudoD 3.
- `NEA/interno/memory.(hh|cpp)`: Estructuras básicas para el manejo de memoria, también define la interfaz fundamental de PseudoD 3, `pseudod::Valor`, la cual representa un objeto.
- `NEA/interno/memory_types.(hh|cpp)`: Tipos de memoria básicos, como enteros, textos y otros.
- `NEA/interno/new_interpreter.(hh|cpp)`: El nuevo intérprete.
- `NEA/interno/builtins.(hh|cpp)`: Builtins del lenguaje, clases y valores cargados en cada programa automáticamente.
- `NEA/interno/data.(hh|cpp)`: Estructuras de datos, funciones y clases generales utilizadas por todo el intérprete. (Nota: Muchas de las cosas definidas por estos archivos realmente no son utilizadas. Actualmente las partes más usadas con las clases de excepciones.)

## BEPD ##

La estructura de la Biblioteca Estándar de PseudoD (BEPD) está documentada en el archivo `bepd/LEEME.txt`. El directorio `bepd/` contiene únicamente la BEPD de PseudoD 2.3, la biblioteca estándar de PseudoD 3 se encontrará en `pd-libs/bepd/`.
