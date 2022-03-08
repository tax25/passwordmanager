CC = g++

CFLAGS = -lsqlite3

all:
	$(CC) main.cpp timeManager/timeManager.cpp DBManager/DBManager.cpp -o main.o $(CFLAGS)
