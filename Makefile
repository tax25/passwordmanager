CC = g++

CFLAGS = -lsqlite3

all:
	$(CC) $(CFLAGS) main.cpp timeManager/timeManager.cpp DBManager/DBManager.cpp -o main.o
