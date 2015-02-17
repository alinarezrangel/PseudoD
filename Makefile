## g++ -fPIC -shared pseudod.cpp -ldl -o libpseudod.so
## g++ Main.cpp -ldl -o PseudoD

CC = g++
CFLAGS = -fPIC
SHARED = -shared
LIBS = -ldl
MEM = NEA/PDData.cpp

PseudoD:	pseudod.cpp Main.cpp	
	$(CC) $(CFLAGS) -c -std=c++11 $(MEM) $(LIBS) -o Data.o
	$(CC) $(CFLAGS) $(SHARED) -std=c++11 pseudod.cpp $(LIBS) Data.o -o libpseudod.so
	$(CC) -std=c++11 Main.cpp $(LIBS) Data.o -o PseudoD
