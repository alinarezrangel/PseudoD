## PseudoD fue creado por Alejandro Linarez Rangel

ifndef CC
CC = gcc
endif
ifndef CXX
CXX = g++
endif
CFLAGS = -fPIC -O3
SHARED = -shared
LIBS = -Wl,--no-as-needed -ldl
MEM = NEA/PDData.cpp
# Fijar COMP a -DMINGW=1 o a -DNUMEROS_EN_C=1 para compilar en sistemas Microsoft
ifndef COMP
COMP =
endif
# Fijar INTE a -DINTERACTIVO=1 para compilar un interactivo
# y agrege -DMACOSX=1 para que PseudoD busque las bibliotecas en sistemas
#  Mac OS X
# TODO: Se modifico: ahora PseudoD genera el interactivo y el interprete de archivos de una vez,
# no es necesaria la modificacion descrita en el README/LEEME/ESTRUCTURA para el interactivo
# pero si es necesaria para compilar en Windows.
#INTE = -DINTERACTIVO=1

PseudoD: libpseudod.so libpseudodsrc.a Main.cpp libpseudodsrc.a
	$(CXX) -std=c++11 -O3 Main.cpp $(LIBS) libpseudodsrc.a -o PseudoD

libpseudodsrc.a: Data.o pdbase.o
	ar -cvq libpseudodsrc.a Data.o pdbase.o

pdbase.o: interprete.cpp
	$(CXX) $(CFLAGS) -c -std=c++11 interprete.cpp $(LIBS) Data.o -o pdbase.o

libpseudod.so:	pseudod.cpp pseudod.hh Data.o
	$(CXX) $(CFLAGS) $(SHARED) -std=c++11 pseudod.cpp $(LIBS) Data.o -o libpseudod.so

Data.o: $(MEM) NEA/PDData.hh
	$(CXX) $(CFLAGS) -c -std=c++11 $(MEM) $(LIBS) $(COMP) -o Data.o

