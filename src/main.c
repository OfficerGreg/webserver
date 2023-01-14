#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int BUFFER_SIZE = 1024;

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

	printf("Simple Webserver written in C\n");

	char path[100];
	yellow();
	printf("Enter file path: ");
	scanf("%s", path);

	FILE *file = fopen(path, "r");
	char *resp;
	long size;


	if(file == NULL){
		red();
		perror("could not open file!\n");
		return 1;
	}
	
	fseek(file, 0, SEEK_END);	
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	resp = malloc(sizeof(char) * (size + 1));

	fread(resp, size, 1, file);
	fclose(file);

//	printf("%s", resp);


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
	uint16_t	PORT;
	yellow();
	printf("\nPlease enter the server port: ");
	scanf("%hi", &PORT);

	printf("\nEnter the Buffer Size: ");
	scanf("%i", &BUFFER_SIZE);
	reset();
	
	char buffer[BUFFER_SIZE];

	char http_header[BUFFER_SIZE];

	strncpy(http_header, "HTTP/1.1 200 OK\r\nServer: webserver-c\r\nContent-type: text/html\r\n\r\n", BUFFER_SIZE);
        //"Server: webserver-c\r\n"
        //"Content-type: text/html\r\n\r\n";

	strcat(http_header, resp);
//	strcat(http_header, "\r\n");

//	printf("%s", http_header);	



	struct sockaddr_in host_addr;
	int host_addrlen = sizeof(host_addr);

	host_addr.sin_family		=	AF_INET;
	host_addr.sin_port		=	htons(PORT);
	host_addr.sin_addr.s_addr	=	htonl(INADDR_ANY);

	struct sockaddr_in client_addr;
	int client_addrlen = sizeof(client_addr);

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
	printf("\nserver is listening for incoming connections on port %i\n\n", PORT);

	for(;;){

		int new_server_socket = accept(server_socket, (struct sockaddr*)&host_addr, (socklen_t*)&host_addrlen);

		if(new_server_socket < 0){
			red();
			perror("webserver (accept)");
			continue;
		}
		
		green();
		printf("connection established!");

		int sockn = getsockname(new_server_socket, (struct sockaddr*)&client_addr, (socklen_t*)&client_addrlen);

		if(sockn < 0){
			red();
			perror("webserver (getsocketname");
			continue;
		}
		
		
		int read_value = read(new_server_socket, buffer, BUFFER_SIZE);
		if(read_value < 0){
			red();
			perror("webserver (read)");
			continue;
		}

		char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
		sscanf(buffer, "%s %s %s", method, uri, version);
		printf("[%s:%u] %s %s %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), method ,version, uri);
		

		int write_value = write(new_server_socket, http_header, strlen(http_header));
		if(write_value < 0){
			red();
			perror("webserver (write)");
			continue;
		}

		close(new_server_socket);
	}

	return 0;
}
