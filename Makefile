## g++ -fPIC -shared pseudod.cpp -ldl -o libpseudod.so
## g++ Main.cpp -ldl -o PseudoD

CC = g++
CFLAGS = -fPIC -shared
LIBS = -ldl

PseudoD:	pseudod.cpp Main.cpp
	$(CC) $(CFLAGS) -std=c++11 pseudod.cpp $(LIBS) -o libpseudod.so
	$(CC) -std=c++11 Main.cpp $(LIBS) -o PseudoD
