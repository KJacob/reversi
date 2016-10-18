#Our compiler.

CC = g++
CFLAGS = -std=c++0x -g

default = reversi

reversi: main.o RemotePlayer.o GameEngine.o Strategy.o
	$(CC) $(CFLAGS) main.o RemotePlayer.o GameEngine.o Strategy.o -o reversi

RemotePlayer.o: RemotePlayer.cpp RemotePlayer.hpp
	$(CC) $(CFLAGS) -c RemotePlayer.cpp -o RemotePlayer.o

GameEngine.o: GameEngine.cpp GameEngine.hpp
	$(CC) $(CFLAGS) -c GameEngine.cpp -o GameEngine.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o
	
Strategy.o: Strategy.cpp
	$(CC) $(CFLAGS) -c Strategy.cpp -o Strategy.o

clean:
	rm reversi main.o GameEngine.o RemotePlayer.o
