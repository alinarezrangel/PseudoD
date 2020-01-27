## PseudoD fue creado por Alejandro Linarez Rangel
# Copyright 2016-2018 Alejandro Linarez Rangel
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
CC=gcc
endif
ifndef CXX
CXX=g++
endif
NIAPATH=NIA
NEAPATH=NEA
CFLAGS=-fPIC -O3 -I./ -I./$(NIAPATH)/ -Wl,--no-as-needed
SHARED=-shared
LIBS=-ldl
DEBUG=-g -Wall
OPT=-O3
COPT=-c
CPPLANG=-std=c++14
ifndef COMP
COMP =
endif

all: pseudod PseudoD libpseudod.so libpseudodsrc.a

run: pseudod
	./pseudod

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

nia_interno_memory.o: $(NEAPATH)/interno/memory.cpp $(NEAPATH)/interno/memory.hh
	$(CXX) $(COPT) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< $(LIBS) -o $@

nia_builtins.o: $(NEAPATH)/interno/builtins.cpp $(NEAPATH)/interno/builtins.hh
	$(CXX) $(COPT) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< $(LIBS) -o $@

nia_interprete.o: $(NIAPATH)/interprete.cpp $(NIAPATH)/interprete.hh
	$(CXX) $(COPT) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< $(LIBS) -o $@

nia_new_interpreter.o: $(NEAPATH)/interno/new_interpreter.cpp $(NEAPATH)/interno/new_interpreter.hh
	$(CXX) $(COPT) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< $(LIBS) -o $@

nia_new_tokenizer.o: $(NEAPATH)/interno/new_tokenizer.cpp $(NEAPATH)/interno/new_tokenizer.hh
	$(CXX) $(COPT) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< $(LIBS) -o $@

memory_types.o: $(NEAPATH)/interno/memory_types.cpp $(NEAPATH)/interno/memory_types.hh
	$(CXX) $(COPT) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< $(LIBS) -o $@

libpseudodsrc.a: nea_pddata.o nia_interprete.o nea_interno_data.o nia_interno_nmemonic.o nia_interno_token.o nia_interno_memory.o memory_types.o nia_interno_tokenizer.o nia_new_tokenizer.o nia_new_interpreter.o nia_builtins.o
	ar -cvr $@ $^

libpseudod.so: $(NIAPATH)/pseudod.cpp $(NIAPATH)/pseudod.hh $(NIAPATH)/codefile.cpp $(NIAPATH)/definitionfile.cpp $(NIAPATH)/includefile.hh $(NIAPATH)/includefilelib.hh libpseudodsrc.a
	$(CXX) $(DEBUG) $(CPPLANG) $(OPT) $(SHARED) $(CFLAGS) $< libpseudodsrc.a $(LIBS) -o $@

pseudod: $(NIAPATH)/Main.cpp libpseudodsrc.a
	$(CXX) $(DEBUG) $(CPPLANG) $(OPT) $(CFLAGS) $< libpseudodsrc.a $(LIBS) -o $@

PseudoD: pseudod
	cp pseudod PseudoD

clean:
	rm nea_interno_data.o nea_pddata.o nia_interno_nmemonic.o nia_interprete.o libpseudodsrc.a libpseudod.so PseudoD pseudod memory_types.o nia_interno_token.o nia_interno_tokenizer.o nia_interno_memory.o nia_new_interpreter.o nia_new_tokenizer.o nia_builtins.o

install:
	./instalar.sh
