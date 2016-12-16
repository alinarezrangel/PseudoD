## PseudoD fue creado por Alejandro Linarez Rangel

ifndef CC
CC = gcc
endif
ifndef CXX
CXX = g++
endif
NIAPATH=NIA
NEAPATH=NEA
CFLAGS = -fPIC -O3 -I$(NIAPATH)/ -I.
SHARED = -shared
LIBS = -Wl,--no-as-needed -ldl
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

PseudoD: libpseudod.so libpseudodsrc.a $(NIAPATH)/Main.cpp
	$(CXX) $(DEBUG) $(OPT) $(CFLAGS) -std=c++11 $(NIAPATH)/Main.cpp $(LIBS) libpseudodsrc.a -o PseudoD

libpseudodsrc.a: Data.o pdbase.o nmemoic.o niadata.o variante.o
	ar -cvq libpseudodsrc.a niadata.o variante.o Data.o pdbase.o nmemoic.o

pdbase.o: $(NIAPATH)/interprete.cpp $(NIAPATH)/interprete.hh
	$(CXX) $(DEBUG) $(OPT) $(CFLAGS) -c -std=c++11 $(NIAPATH)/interprete.cpp $(LIBS) -o pdbase.o

libpseudod.so: $(NIAPATH)/pseudod.cpp $(NIAPATH)/pseudod.hh libpseudodsrc.a
	$(CXX) $(DEBUG) $(OPT) $(CFLAGS) $(SHARED) -std=c++11 $(NIAPATH)/pseudod.cpp $(LIBS) libpseudodsrc.a -o libpseudod.so

Data.o: $(MEM) $(NEAPATH)/PDData.hh $(NEAPATH)/PDData.cpp
	$(CXX) $(DEBUG) $(OPT) $(CFLAGS) -c -std=c++11 $(NEAPATH)/PDData.cpp $(LIBS) $(COMP) -o Data.o

nmemoic.o: $(MEM) $(NIAPATH)/nmemoic.hh $(NIAPATH)/nmemoic.cpp
	$(CXX) $(DEBUG) $(OPT) $(CFLAGS) -c -std=c++11 $(NIAPATH)/nmemoic.cpp $(LIBS) $(COMP) -o nmemoic.o

niadata.o: $(NEAPATH)/interno/data.cpp $(NEAPATH)/interno/data.hh
	$(CXX) $(DEBUG) $(OPT) $(CFLAGS) -c -std=c++11 $(NEAPATH)/interno/data.cpp $(LIBS) $(COMP) -o niadata.o

variante.o: $(NEAPATH)/interno/variante.cpp $(NEAPATH)/interno/variante.hh
	$(CXX) $(DEBUG) $(OPT) $(CFLAGS) -c -std=c++11 $(NEAPATH)/interno/variante.cpp $(LIBS) $(COMP) -o variante.o

clean:
	rm Data.o
	rm pdbase.o
	rm libpseudodsrc.a
	rm nmemoic.o
	rm niadata.o
	rm variante.o
