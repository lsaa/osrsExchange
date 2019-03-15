CC=g++

osrs: src/main.cpp src/utils/logging.cpp
	$(CC) -o bin/osrs src/main.cpp -lcurl -l sqlite3
