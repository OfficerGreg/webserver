#include <errno.h>
#include <sys/socket.h>
#include <stdio.h>


int main(){
	//socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("webserver (socket)");
		return 1;
	}
	printf("socket was created successfully\n");

	//bind
	struct sockaddr_in host_addr;
	int host_addrlen = sizeof(host_addr);

	host_addr.sin_family		=	F_INET;
	host_addr.sin_port		=	htons(PORT);
	host_addr.sin_addr.s_addr	=	htonl(INADDR_ANY);

	if(bind(sockfd,(struct sockaddr*)&host_addr, host_addrlen) != 0){
		perror("webserver (bind)");
		return 1;
	}
	prinf("socket successfully bound to address\n");


	return 0;
}
