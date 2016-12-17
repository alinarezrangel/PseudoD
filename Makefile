## PseudoD fue creado por Alejandro Linarez Rangel

ifndef CC
CC = gcc
endif
ifndef CXX
CXX = g++
endif
NIAPATH=NIA
NEAPATH=NEA
CFLAGS=-fPIC -O3 -I./ -I./$(NIAPATH)/ -Wl,--no-as-needed
SHARED=-shared
LIBS=-ldl
DEBUG=-g -Wall
OPT=-O3
COPT=-c
CPPLANG=-std=c++11
ifndef COMP
COMP =
endif

all: PseudoD libpseudod.so

#nea_interno_data.o: $(NEAPATH)/interno/data.cpp $(NEAPATH)/interno/data.hh
#	$(CXX) $(COPT) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< $(LIBS) -o $@

nea_pddata.o: $(NEAPATH)/PDData.cpp $(NEAPATH)/PDData.hh
	$(CXX) $(COPT) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< $(LIBS) -o $@

nia_nmemonic.o: $(NIAPATH)/nmemoic.cpp $(NIAPATH)/nmemoic.hh
	$(CXX) $(COPT) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< $(LIBS) -o $@

nia_interprete.o: $(NIAPATH)/interprete.cpp $(NIAPATH)/interprete.hh
	$(CXX) $(COPT) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< $(LIBS) -o $@

libpseudodsrc.a: nea_pddata.o nia_nmemonic.o nia_interprete.o #nia_interno_data.o
	ar -cvr $@ $^

libpseudod.so: $(NIAPATH)/pseudod.cpp $(NIAPATH)/pseudod.hh $(NIAPATH)/codefile.cpp $(NIAPATH)/definitionfile.cpp $(NIAPATH)/includefile.hh $(NIAPATH)/includefilelib.hh libpseudodsrc.a
	$(CXX) $(DEBUG) $(CPPLANG) $(OPT) $(SHARED) $(CFLAGS) $< libpseudodsrc.a $(LIBS) -o $@

PseudoD: $(NIAPATH)/Main.cpp libpseudodsrc.a
	$(CXX) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< libpseudodsrc.a $(LIBS) -o $@

clean:
	rm nea_interno_data.o nea_pddata.o nia_nmemonic.o nia_interprete.o libpseudodsrc.a libpseudod.so PseudoD
