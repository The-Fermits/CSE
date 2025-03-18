// command ifconfig : show all your networks (loopback , ethermet , wifi)
// Problem 1 : display local time and date on server and say hello

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<unistd.h>
#include<limits.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<ctime>
using namespace::std;

#define MAXLEN 50 // max length input in one go 

int keep_recv(int sockfd,string &recv_str){
    char buf[MAXLEN];
    int byt_recv=recv(sockfd,&buf[0],MAXLEN-1,0);
    buf[byt_recv] = '\0'; // make it string by adding '\0' in the end
    if(byt_recv<0) return 0;
    recv_str+=buf;
    if(buf[byt_recv-1]=='\0'){ // if string already end then stop receiving 
        return 0;
    }
    return 1;
}

string recv_msg(int sockfd){
    int status = 1;
    string recv_str="";
    while(status!=0){
        status = keep_recv(sockfd,recv_str);
    }
    return recv_str;
}

int main (int argc, char * argv[]) { // command line arguments , argc - no of strings , argv - string values
    int  PORT =  (atoi(argv[1])); // atoi == stoi in c++
    srand(time(0)); // set time with server/ maching 
    int socket_fd, new_socket_fd ; // socket_fd for establishing continoues pipeline , and new_socket_fd for evry query
    socklen_t client_addr_len ; // load balencer address length
    struct sockaddr_in client_addr , serv_addr ; //sockaddr_in is derived from pre-defined struct sockaddr

    // step 1 : socket creation

    socket_fd = socket(AF_INET,SOCK_STREAM,0); 
    if(socket_fd<0) {
        cout <<"socket creation at server failed"; 
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET; // used for ip4
    serv_addr.sin_addr.s_addr = INADDR_ANY; // assign any address which is free
    serv_addr.sin_port = htons(PORT); // convert port into byte 

    // step 2 : bind 
    if(bind(socket_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0) { // typecast back to sockaddr (syntax stuff)
     cout <<"bind at server failed"<< endl ; 
     exit(EXIT_FAILURE);
    }

    // step 3 : listen 
    listen(socket_fd,5); // maximum 5 connection request in a queue 

    // step 4 :accept 

    client_addr_len = sizeof(client_addr); // will lb_addr store client data in next line ? 
    new_socket_fd = accept(socket_fd,(struct sockaddr*)&client_addr,&client_addr_len) ;// use of lb adress ?

    if(new_socket_fd<0) {
        cout <<"Accept error at server" << endl ;
        exit(EXIT_FAILURE);
    }

    cout <<"Connection established with the server" << endl ;

    // step 5 : receive 
    string recv_str = recv_msg(new_socket_fd);
    // print the msg received and print hello from server 
    cout << "msg received from client :"<< recv_str << endl ; 
    cout << "sending Hello from server" << endl ; 
    string server_response = "hello from server";


    // step 6: send 
    // step 7 : close 

    // convert into .c_str() (in c) with last character '\0' that why length()+1 is here
    send(new_socket_fd,server_response.c_str(),server_response.length()+1,0);

    // sned time 
    time_t now = time(0);
    string server_time = ctime(&now);
    send(new_socket_fd,server_time.c_str(),server_time.length()+1,0);
    cout << "Timr sent from server "<<endl;
    close(new_socket_fd); 
    close(socket_fd);

    return 0 ; 
}


// to run and test 
// compile server and client on different terminal : g++ tcp_server.cpp -o server , g++ tcp_client.cpp -o client
// run server and then client : ./server 8080 , ./client 