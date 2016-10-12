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
DEBUG=-g -Wall
OPT=-O3
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
	$(CXX) -std=c++11 $(DEBUG) $(OPT) Main.cpp $(LIBS) libpseudodsrc.a -o PseudoD

libpseudodsrc.a: Data.o pdbase.o nmemoic.o
	ar -cvq libpseudodsrc.a Data.o pdbase.o nmemoic.o

pdbase.o: interprete.cpp
	$(CXX) $(DEBUG) $(OPT) $(CFLAGS) -c -std=c++11 interprete.cpp $(LIBS) Data.o -o pdbase.o

libpseudod.so:	pseudod.cpp pseudod.hh Data.o
	$(CXX) $(DEBUG) $(OPT) $(CFLAGS) $(SHARED) -std=c++11 pseudod.cpp $(LIBS) Data.o -o libpseudod.so

Data.o: $(MEM) NEA/PDData.hh
	$(CXX) $(DEBUG) $(OPT) $(CFLAGS) -c -std=c++11 $(MEM) $(LIBS) $(COMP) -o Data.o

nmemoic.o: Data.o
	$(CXX) $(DEBUG) $(OPT) $(CFLAGS) -c -std=c++11 nmemoic.cpp $(LIBS) $(COMP) -o nmemoic.o

clean:
	rm Data.o
	rm pdbase.o
	rm libpseudodsrc.a
	rm nmemoic.o

