#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#define PORT 8080

int main()
{
	// create TCP socket (IPv4, stream socket)
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd == 0){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Bind the socket to an IP address and port
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0){
		perror("bind failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	// listen for incoming connections with a backlog of 5
	if(listen(server_fd, 5) < 0){
		perror("listen failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	std::cout << "Listening..." << std::endl;
	int client_fd = accept(server_fd, (sockaddr*)&addr, (socklen_t*)&addr );
	if(client_fd < 0){
		perror("accept failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}	

	// each loop: read data from client and send it back
	char buffer[1024];
	int bytesRead;
	while(bytesRead = read(client_fd, buffer, sizeof(buffer)) > 0){
		std::cout << "Received: " << buffer << std::endl;
		
		// Echo back the same data
		send(client_fd, buffer, strlen(buffer), 0);
		memset(buffer, 0, sizeof(buffer)); // clear buffer for next read
	}

	// clean up
	close(client_fd);
	close(server_fd);

	return 0;

}
