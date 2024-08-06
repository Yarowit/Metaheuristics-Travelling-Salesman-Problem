CC := g++
CFLAGS := -O3
BUILD := build
SRC := src

SRCSCPP := $(wildcard ${SRC}/*.cpp)

OBJS := $(patsubst ${SRC}/%.cpp,${BUILD}/%.o,$(SRCSCPP))

all: main

main: $(OBJS) 
	$(CC) -o $@ $^

${BUILD}/%.o: ${SRC}/%.cpp ${SRC}/*.hpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm ${BUILD}/*
	rm main

cleanG:
	rm tree/*
	rm path/*
	rm random/*
