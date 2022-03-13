CC = g++

CFLAGS = -lsqlite3 -std=c++11

all:
	$(CC) main.cpp timeManager/timeManager.cpp DBManager/DBManager.cpp -o newPasswordManager $(CFLAGS)

install:
	sudo cp newPasswordManager /usr/local/bin
	rm newPasswordManager

clean:
	sudo rm -r /usr/local/bin/newPasswordManager
	sudo rm -r /usr/local/bin/passwordsDB.db
