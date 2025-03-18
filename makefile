# commands : make will execute make all automatically , make clean for delete client and server from current dir
all:
	g++ tcp_client.cpp -o client
	g++ tcp_server.cpp -o server
clean:
	rm client server
