#include <errno.h>
#include <sys/socket.h>
#include <stdio.h>

struct sockaddr_in{
	sa_family_t	sin_family;	//address family: AF_INET
	in_port_t	sin_port;	//port in network byte order
	struct in_addr	sin_addr;	//internet address
}

struct in_addr{
	uint32_t	s_addr;
}

int main(){

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("webserver (socket)");
		return 1;
	}
	printf("socket was created successfully\n");

	return 0;
}
