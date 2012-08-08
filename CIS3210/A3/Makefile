all: server client

server: assignment3-server.c udps.h
	gcc assignment3-server.c -o server

client: client.cpp client.h
	g++ client.cpp -o client

clean:
	rm server client