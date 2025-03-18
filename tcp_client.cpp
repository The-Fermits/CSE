#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std ; 

#define PORT 8080
#define MAXLEN 50

int keep_recv (int socket_fd, string &recv_str) {
    char buf[MAXLEN];
    int byte_recv = recv(socket_fd,&buf[0],MAXLEN-1,0);
    buf[byte_recv] ='\0';
    if(byte_recv <0) return 0;
    recv_str += buf;
    if(buf[byte_recv-1]=='\0') {
        return 0;
    }
    return 1;
}

string recv_msg (int socket_fd) {
    string recv_str = ""; 
    int status = 1; 
    while(status!=0) {
        status = keep_recv(socket_fd,recv_str);
    }
    return recv_str;
}

int main () {

    int socket_fd ; // everything in unix is a file and we need file descriptor to acess
    struct sockaddr_in serv_addr ; // in server code we have 2 fd but in client only server address is enough

    // step 1 : create socket 
    socket_fd = socket(AF_INET,SOCK_STREAM,0) ;
    if(socket_fd <0) {
        cout <<"client socket failed" << endl ; 
        exit(EXIT_FAILURE);
    }

    
    // adress specification at client 
    serv_addr.sin_family = AF_INET ; 
    inet_aton("127.0.0.1",&serv_addr.sin_addr); // convert ip into required struct automatically (this is loopback ip)
    serv_addr.sin_port=htons(PORT); // set PORT number 

    if (inet_aton("127.0.0.1", &serv_addr.sin_addr) == 0) {
    perror("Invalid IP address");
    exit(EXIT_FAILURE);
    } 


    // step 2 : connect 
    int connect_fd = connect(socket_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) ; 
    if(connect_fd <0) {
        perror("connection of client with server failed");
        exit(EXIT_FAILURE);
    }

    cout <<"client connected to server successfully"<< endl ; 

    // step 3 : send msg to server 
    string client_msg = "Hello from client"; 
    send(socket_fd,client_msg.c_str(),client_msg.length()+1,0);

    // step 4 : receive response from server 
    string server_response = recv_msg(socket_fd) ; 
    cout << "msg from server : "<<server_response << endl ;

    server_response = recv_msg(socket_fd) ; 
    cout << "time from server : "<<server_response << endl ;

    // step 5 : close the client connection 
    cout <<"closing the connection of the client side" << endl ;
    close(socket_fd);

    return 0;
}

// to run and test 
// compile server and client on different terminal : g++ tcp_server.cpp -o server , g++ tcp_client.cpp -o client
// run server and then client : ./server 8080 , ./client 