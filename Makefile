## PseudoD fue creado por Alejandro Linarez Rangel
# Copyright 2016 Alejandro Linarez Rangel
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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

all: PseudoD libpseudod.so libpseudodsrc.a

nea_interno_data.o: $(NEAPATH)/interno/data.cpp $(NEAPATH)/interno/data.hh
	$(CXX) $(COPT) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< $(LIBS) -o $@

nea_pddata.o: $(NEAPATH)/PDData.cpp $(NEAPATH)/PDData.hh
	$(CXX) $(COPT) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< $(LIBS) -o $@

nia_interno_nmemonic.o: $(NEAPATH)/interno/nmemoic.cpp $(NEAPATH)/interno/nmemoic.hh
	$(CXX) $(COPT) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< $(LIBS) -o $@

nia_interno_token.o: $(NEAPATH)/interno/token.cpp $(NEAPATH)/interno/token.hh
	$(CXX) $(COPT) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< $(LIBS) -o $@

nia_interno_tokenizer.o: $(NEAPATH)/interno/tokenizer.cpp $(NEAPATH)/interno/tokenizer.hh
	$(CXX) $(COPT) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< $(LIBS) -o $@

nia_interprete.o: $(NIAPATH)/interprete.cpp $(NIAPATH)/interprete.hh
	$(CXX) $(COPT) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< $(LIBS) -o $@

libpseudodsrc.a: nea_pddata.o nia_interno_nmemonic.o nia_interprete.o nea_interno_data.o nia_interno_token.o nia_interno_tokenizer.o
	ar -cvr $@ $^

libpseudod.so: $(NIAPATH)/pseudod.cpp $(NIAPATH)/pseudod.hh $(NIAPATH)/codefile.cpp $(NIAPATH)/definitionfile.cpp $(NIAPATH)/includefile.hh $(NIAPATH)/includefilelib.hh libpseudodsrc.a
	$(CXX) $(DEBUG) $(CPPLANG) $(OPT) $(SHARED) $(CFLAGS) $< libpseudodsrc.a $(LIBS) -o $@

PseudoD: $(NIAPATH)/Main.cpp libpseudodsrc.a
	$(CXX) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< libpseudodsrc.a $(LIBS) -o $@

clean:
	rm nea_interno_data.o nea_pddata.o nia_interno_nmemonic.o nia_interprete.o libpseudodsrc.a libpseudod.so PseudoD

install:
	./instalar.sh
