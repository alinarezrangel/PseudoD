## PseudoD fue creado por Alejandro Linarez Rangel

CC = g++
CFLAGS = -fPIC
SHARED = -shared
LIBS = -ldl
MEM = NEA/PDData.cpp
# Fijar COMP a -DMINGW=1 o a -DNUMEROS_EN_C=1 para compilar en sistemas Microsoft
COMP = 
# Fijar INTE a -DINTERACTIVO=1 para compilar un interactivo
# y agrege -DMACOSX=1 para que PseudoD busque las bibliotecas en sistemas
#  Mac OS X
# TODO: Se modifico: ahora PseudoD genera el interactivo y el interprete de archivos de una vez,
# no es necesaria la modificacion descrita en el README/LEEME/ESTRUCTURA para el interactivo
# pero si es necesaria para compilar en Windows.
#INTE = -DINTERACTIVO=1

PseudoD:	libpseudod.so Data.o Main.cpp libpseudodsrc.a
	$(CC) -std=c++11 Main.cpp $(LIBS) libpseudodsrc.a -o PseudoD

libpseudodsrc.a: Data.o pdbase.o
	ar -cvq libpseudodsrc.a Data.o pdbase.o

pdbase.o: interprete.cpp
	$(CC) $(CFLAGS) -c -std=c++11 interprete.cpp -o pdbase.o $(LIBS) Data.o

libpseudod.so:	pseudod.cpp pseudod.hh Data.o
	$(CC) $(CFLAGS) $(SHARED) -std=c++11 pseudod.cpp $(LIBS) Data.o -o libpseudod.so

Data.o: $(MEM) NEA/PDData.hh	
	$(CC) $(CFLAGS) -c -std=c++11 $(MEM) $(LIBS) $(COMP) -o Data.o
