#include <stdio.h>
#include <winsock2.h> // which provides networking functionality on Windows. crucial for using the Windows Sockets API (Winsock).
#include <windows.h>
#include <conio.h> // For _kbhit and _getch
#pragma comment(lib, "ws2_32.lib")//instructs the linker to link against the ws2_32.lib library, which is necessary for Winsock functionality.

#define PORT 12345
#define SERVER_IP "192.168.1.93"

int main() {
    WSADATA wsaData; //This structure holds details about the Windows Sockets implementation. It needs to be initialized using the WSAStartup function.
    SOCKET clientSocket; //hold the socket descriptor for the client.
    struct sockaddr_in serverAddr; //server's address information.

    // Initialize Winsock library, version makeword2,2, pointer
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) { //check
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    // Initialize server address struct
    serverAddr.sin_family = AF_INET; //sets the address family to IPv4
    serverAddr.sin_port = htons(PORT);//Sets the port number in network byte order.

    // Convert server IP address string to binary form
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    if (serverAddr.sin_addr.s_addr == INADDR_NONE) {
        printf("Invalid address or address not supported\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Connection failed\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    printf("Connected to server\n");
    //predefined message 
     char message[] = "Hello, Pi Rasp are u there?";

        
    // Send periodic messages to the server
    while (1) {
      
        printf("Sending periodic message...\n");

        // Send the message to the server
        send(clientSocket, message, strlen(message), 0);

        // Check for exit condition (non-blocking input)
        if (_kbhit()) {
            char key = _getch();
            if (key == 'e' || key == 'E') {
                printf("Exiting...\n");
                break;
            }
        }

        // Sleep for 10 milliseconds
        Sleep(10);

    }

    // Close the socket
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
