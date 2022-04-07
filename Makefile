CC = g++

CFLAGS = -lsqlite3 -std=c++11

all:
	$(CC) src/main.cpp src/timeManager/timeManager.cpp src/DBManager/DBManager.cpp -o newPasswordManager $(CFLAGS)

install:
	sudo cp newPasswordManager /usr/local/bin
	rm newPasswordManager

clean:
	sudo rm -r /usr/local/bin/newPasswordManager
	sudo rm -r /usr/local/bin/passwordsDB.db
