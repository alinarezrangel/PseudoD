## PseudoD fue creado por Alejandro Linarez Rangel

CC = g++
CFLAGS = -fPIC
SHARED = -shared
LIBS = -ldl
MEM = NEA/PDData.cpp
# Fijar COMP a -DMINGW=1 para compilar en sistemas Microsoft
COMP = 
# Fijar INTE a -DINTERACTIVO=1 para compilar un interactivo
# y agrege -DMACOSX=1 para que PseudoD busque las bibliotecas en sistemas
#  Mac OS X
INTE = 

PseudoD:	libpseudod.so Data.o Main.cpp
	echo "Trabajando el Main"
	$(CC) -std=c++11 $(INTE) Main.cpp $(LIBS) Data.o -o PseudoD

libpseudod.so:	pseudod.cpp pseudod.hh Data.o
	$(CC) $(CFLAGS) $(SHARED) -std=c++11 $(INTE) pseudod.cpp $(LIBS) Data.o -o libpseudod.so

Data.o: $(MEM) NEA/PDData.hh	
	$(CC) $(CFLAGS) -c -std=c++11 $(MEM) $(LIBS) $(COMP) -o Data.o
