## PseudoD fue creado por Alejandro Linarez Rangel

CC = g++
CFLAGS = -fPIC
SHARED = -shared
LIBS = -ldl
MEM = NEA/PDData.cpp
# Fijar COMP a -DMINGW=1 para compilar con soporte numerico de MinGW
COMP = 
# Fijar INTE a -DINTERACTIVO=1 para compilar un interactivo
INTE = 

PseudoD:	pseudod.cpp Main.cpp	
	$(CC) $(CFLAGS) -c -std=c++11 $(MEM) $(LIBS) $(COMP) -o Data.o
	$(CC) $(CFLAGS) $(SHARED) -std=c++11 pseudod.cpp $(LIBS) Data.o -o libpseudod.so
	$(CC) -std=c++11 $(INTE) Main.cpp $(LIBS) Data.o -o PseudoD
