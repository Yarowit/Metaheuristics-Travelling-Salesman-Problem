CC := g++
CFLAGS := -pthread -std=c++17 -O3
DBFlags := -fdiagnostics-color=always -g -ggdb3
BUILD := build
SRC := src

SRCSCPP := $(wildcard ${SRC}/*.cpp)

OBJS := $(patsubst ${SRC}/%.cpp,${BUILD}/%.o,$(SRCSCPP))

all: main

main: $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $^

${BUILD}/%.o: ${SRC}/%.cpp ${SRC}/*.hpp
	$(CC) $(CFLAGS) -o $@ -c $<

debug: $(OBJS) 
	$(CC) $(DBFlags) -o $@ $^

clean:
	rm ${BUILD}/*
	rm main

cleanG:
	rm tree/*
	rm path/*
	rm random/*
