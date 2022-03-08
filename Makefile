CC = g++

CFLAGS = -lsqlite3

all:
	$(CC) main.cpp timeManager/timeManager.cpp DBManager/DBManager.cpp -o newPasswordManager $(CFLAGS)

install:
	sudo cp newPasswordManager /usr/local/bin

clean:
	sudo rm -r /usr/local/bin/newPasswordManager
