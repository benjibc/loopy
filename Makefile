CC=g++

SRC=./library/sys
APPSRC=./app/*
APPSRC2=./app/*/*

TGT=obj
SOURCES=$(wildcard $(SRC)/*.cpp) $(wildcard $(APPSRC)/*.cpp) $(wildcard $(APPSRC2)/*.cpp)./library/threadlocal.cpp ./library/threadshared.cpp
OBJS=$(addprefix $(TGT)/, $(notdir $(SOURCES:.cpp=.o)))

CFLAGS=-Wall -I./library/third_party/include -I./ -std=c++11
LIBS=-lctemplate -levhtp -lpthread -levent
OBJSPATH=./library/obj/

LPATH= -L./library/third_party/lib

all: server 

$(TGT)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) $(LPATH) $(LIBS) -c $< -o $@

$(TGT)/%.o: $(APPSRC)/%.cpp
	$(CC) $(CFLAGS) $(LPATH) $(LIBS) -c $< -o $@

$(TGT)/%.o: ./library/%.cpp
	$(CC) $(CFLAGS) $(LPATH) $(LIBS) -c $< -o $@

server: $(OBJS)
	$(CC) $(CFLAGS) $(LPATH) $(OBJS) $(LIBS) -o loopy.bin main.cpp 

run:
	make all
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./library/third_party/lib; export LD_LIBRARY_PATH; ./loopy.bin

init_repo:
	cd library && sh fetch_libs.sh

clean:
	rm obj/*
