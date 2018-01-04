#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_LISTEN 10

int client_socket[MAX_LISTEN];
int num_client;

struct sockaddr_in client_address;
char server_message[256] = "You have reached the server";

//thread which handles each client
void *thread_fun(void * arg){
	char input[256];
	char *ip = inet_ntoa(client_address.sin_addr);
	while(1){
		if(read(client_socket[(int)arg],input,256) == 0)
			break;
		if(send(client_socket[(int)arg],server_message,256,0) == 0)
			break;
		printf("data from client(%s):%s\n",ip,input);
			
	}
	//one client close the connection
	num_client--;	
	return NULL;
}


void main(){
	
        num_client = 0;
	socklen_t len;
	int err;
	pthread_t threads[MAX_LISTEN];
	
	//create the server socket
	int server_socket;
	server_socket = socket(AF_INET,SOCK_STREAM,0);
	if(server_socket == -1)
	{
		printf("failed to create a socket");
		close(server_socket);
		return;
	}

	//define the server address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8080);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);


	//bind the socket to specified IP and port
	err = bind(server_socket,(struct sockaddr*) &server_address, sizeof(server_address));
	if(err == -1){
		printf("bind error.\n");
		close(server_socket);		
		return ;
	}
	
	//Listen
	err = listen(server_socket,MAX_LISTEN);
	if(err == -1){
		printf("listen error.\n");
		close(server_socket);
		return ;
	}

	//loop for detecting connections
	while(1){
		
		client_socket[num_client] = accept(server_socket, (struct sockaddr*) &client_address,&len);
		if(client_socket[num_client] == -1){
			printf("accept error.\n");
			close(server_socket);
			return;
		}
		err = pthread_create(&threads[num_client],NULL,thread_fun, (void *)num_client);
		if(err == -1){
			printf("failed to create thread");
			close(server_socket);
		}
		//increment
		++num_client;
	}
	close(server_socket);
	

}

