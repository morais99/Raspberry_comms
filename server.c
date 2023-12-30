#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //POSIX operating system API
#include <arpa/inet.h>//networking functions

#define PORT 12345

int main() {
    int server_socket, client_socket; //Variables to hold the socket descriptors
    struct sockaddr_in server_addr, client_addr; //Structures to store server and client address information.
    socklen_t client_len = sizeof(client_addr); //Variable to store the size of the client address structure.
    char buffer[1024];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0); //Creates a socket using the IPv4 address family (AF_INET) and the TCP socket type (SOCK_STREAM).
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    memset(&server_addr, 0, sizeof(server_addr)); //Clears the server address structure to ensure no garbage data is present.
    server_addr.sin_family = AF_INET; //Sets the address family to IPv4.
    server_addr.sin_addr.s_addr = INADDR_ANY; //Binds the server to any available network interface on the machine.
    server_addr.sin_port = htons(PORT); //Sets the port number in network byte order.

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) { //Binds the socket to the specified address and port.
        perror("Binding failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 1) == -1) {
        perror("Listening failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Accept a connection
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket == -1) {
        perror("Acceptance failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connection accepted\n");

    // Receive and print data from the client
    while (1) {
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            break;
        }
        buffer[bytes_received] = '\0'; //Null-terminates the received data to treat it as a string.
        printf("Received message: %s", buffer);
    }
    //The loop breaks when recv returns a value less than or equal to 0, indicating that the client has closed the connection or an error has occurred.
    //.
    // Close the sockets
    close(client_socket);
    close(server_socket);

    return 0;
}
