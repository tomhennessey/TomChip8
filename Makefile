CXX := g++
CXXFLAGS := -std=c++11 -Wno-deprecated -O3

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
endif

ifeq ($(UNAME), Linux)
LDFLAGS = -lglut -lGL -lGLU
endif

default: chip8.bin

clean:
	rm -f chip8.bin *.o

chip8.bin: main.o cpu.o
	$(CXX) $(CXXFLAGS) main.o cpu.o -o chip8.bin $(LDFLAGS)

cpu.o: cpu.cpp cpu.h defines.h
	$(CXX) $(CXXFLAGS) -c cpu.cpp


