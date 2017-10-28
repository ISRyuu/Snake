
CC = clang++ -g --std=c++11 -lshader -lcamera -lSOIL -lGLEW -lglfw -framework Cocoa -framework OpenGL -I/usr/local/include -I/Users/Kevin/Developer/include -L/usr/local/lib -L/Users/Kevin/Developer/lib
CFLAGS = -Wall

all: main clean

main: main.o Snake.o SnakePlane.o
	${CC} -o main main.o Snake.o SnakePlane.o

main.o: main.cpp
	${CC} -c main.cpp

Snake.o: Snake.hpp Snake.cpp
	${CC} -c Snake.cpp

SnakePlane.o: SnakePlane.cpp SnakePlane.hpp
	${CC} -c SnakePlane.cpp

clean:
	rm -rf *o
