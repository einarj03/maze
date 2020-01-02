maze: main.o maze.o
	g++ -Wall -g main.o maze.o -o maze

main.o: main.cpp maze.h
	g++ -Wall -c main.cpp

maze.o: maze.cpp maze.h
	g++ -Wall -c maze.cpp