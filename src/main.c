#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdio.h>


int main(){

	//color
	void yellow(){
  		printf("\033[1;33m");
	}

	void red(){
		perror("\033[1;31m");
	}

	void green(){
		printf("\033[0;32m");
	}

	void reset(){
		printf("\033[0m");
	}
	red();
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("\n     Simple Webserver     \n");
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	reset();

	//socket
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket == -1){
		red();
		perror("webserver (socket)");
		return 1;
	}

	green();
	printf("\nsocket was created successfully\n");
	reset();

	
	//bind
	uint16_t PORT;
	printf("\nPlease enter the server port: ");
	scanf("%hi", &PORT);
	struct sockaddr_in host_addr;
	int host_addrlen = sizeof(host_addr);

	host_addr.sin_family		=	AF_INET;
	host_addr.sin_port		=	htons(PORT);
	host_addr.sin_addr.s_addr	=	htonl(INADDR_ANY);

	if(bind(server_socket,(struct sockaddr*)&host_addr, host_addrlen) != 0){
		red();		
		perror("webserver (bind)");
		return 1;
	}
	green();
	printf("\nsocket successfully bound to address\n");

	//listen, read and write
	if(listen(server_socket, SOMAXCONN) != 0){
		red();
		perror("webserver (listen)");
		return 1;
	}
	reset();	
	printf("\nserver is listening for incoming connections on port %i\n", PORT);

	for(;;){

		int new_server_socket = accept(server_socket, (struct sockaddr*)&host_addr, (socklen_t*)&host_addrlen);

		if(new_server_socket < 0){
			red();
			perror("webserver (accept)");
			continue;
		}
		
		green();
		printf("connection established!");

		
		close(new_server_socket);
	}

	return 0;
}
