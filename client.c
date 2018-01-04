#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]){
	int err;
        char buf[246] = "This is client ";
        char server_response[256];


	//ckeck if there are 3 commands
	if(argc != 3){
		printf("miss options:\n");
		printf("format:./client 127.0.0.1 Port\n");
		return 0;
	}
	printf("IP address:%s\n",argv[1]);
	printf("Port: %d\n",atoi(argv[2]));
	
	//client names
	printf("input your name: ");
	char c[10];
	scanf("%s",c);
	strcat(buf,c);
	
	//create a socket
	int network_socket;
	network_socket = socket(AF_INET,SOCK_STREAM,0);
	
	//specify an address for the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;//TCP
	server_address.sin_port = htons(atoi(argv[2]));
	server_address.sin_addr.s_addr = inet_addr(argv[1]);
	
	int connect_status = connect(network_socket,(struct sockaddr *) &server_address, sizeof(server_address));
	//check for the error
	if(connect_status == -1){
		printf("an error making a connection to remote socket\n");
	}

	while(1){
		//send messages to the server
		send(network_socket,buf,256,0);
		//recieve data from the server
                recv(network_socket,&server_response,sizeof(server_response),0);
                //print out the data from the server
                printf("The server sent the data: %s\n",server_response);
		sleep(1);
	}
	close(network_socket);

}
