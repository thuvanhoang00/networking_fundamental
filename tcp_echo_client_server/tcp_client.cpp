#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h> // for inet_pton
#include <unistd.h> // for close()
#include <cstdlib> // for exit()

#define PORT 8080
const char* SERVER_IP = "127.0.0.1"; // localhost

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    
    // Create a socket
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        std::cerr << "Socket creation error\n";
        exit(EXIT_FAILURE);
    }

    // Specify the server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if(inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0){
        std::cerr << "Invalid address/Address not supported\n";
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        std::cerr << "Connection failed\n";
        exit(EXIT_FAILURE);
    }

    // Read user input, send to server, and display the echoed reponse
    std::string input;
    while(true){
        std::cout << "Enter message(type 'exit' to quit): ";
        std::getline(std::cin, input);
        if(input == "exit") break;

        send(sock, input.c_str(), input.size(), 0);
        int bytesRead = read(sock, buffer, sizeof(buffer));
        if(bytesRead > 0){
            std::cout  << "Echo from server: " << std::string(buffer, bytesRead) << std::endl;
        }
        memset(buffer, 0, sizeof(buffer));
    }

    close(sock);
    return 0;
}