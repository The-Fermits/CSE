all:
	g++ tcp_client.cpp -o client
	g++ tcp_server.cpp -o server
clean:
	rm client server