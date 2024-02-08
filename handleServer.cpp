#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "handleServer.h"
#include "colors.h"

int handleServer() {
    std::cout << YELLOW << "Attempting to create server...\n" << RESET;

    SOCKET wsocket;
    SOCKET new_wsocket;
    WSADATA wsaData;
    struct sockaddr_in server;
    int server_len;
    int BUFFER_SIZE = 30720;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << RED << "WSAStartup failed.\n" << RESET;
        return 1;
    }

    // Create a socket
    wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (wsocket == INVALID_SOCKET) {
        std::cout << RED << "Socket creation failed.\n" << RESET;
        WSACleanup();
        return 1;
    }

    //bind socket do address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(8080);
    server_len = sizeof(server);

    if (bind(wsocket, reinterpret_cast<SOCKADDR*>(&server), server_len) != 0) {
        std::cout << RED << "Could not bind socket.\n" << RESET;
        closesocket(wsocket);
        WSACleanup();
        return 1;
    }

    //listen to address
    if (listen(wsocket, SOMAXCONN) != 0) {
        std::cout << RED << "Could not start listening to socket.\n" << RESET;
        closesocket(wsocket);
        WSACleanup();
        return 1;
    }

    std::cout << GREEN << "Server started!\n" << RESET;
    std::cout << GREEN << "Listening on 127.0.0.1:8080\n" << RESET;

    int bytes = 0;
    while (true) {
        //accept client request
        new_wsocket = accept(wsocket, (SOCKADDR*)&server, &server_len);
        if (new_wsocket == INVALID_SOCKET) {
            std::cout << RED << "Could not accept.\n" << RESET;
            closesocket(wsocket);
            WSACleanup();
            return 1;
        }

        //read request
        char buff[30720] = { 0 };
        bytes = recv(new_wsocket, buff, BUFFER_SIZE, 0);
        if (bytes < 0) {
            std::cout << RED << "Could not read request\n" << RESET;
        }

        std::string serverMessage = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
        std::string response = "<html><h1>Hello World</h1></html>";
        serverMessage.append(std::to_string(response.size()));
        serverMessage.append("\n\n");
        serverMessage.append(response);

        int bytesSend = 0;
        int totalBytesSend = 0;
        while (totalBytesSend < serverMessage.size()) {
            bytesSend = send(new_wsocket, serverMessage.c_str(), serverMessage.size(), 0);
            if (bytesSend < 0) {
                std::cout << RED << "Could not send response\n" << RESET;
            }

            totalBytesSend = +bytesSend;
        }

        std::cout << GREEN << "Sent response to client\n" << RESET;

        closesocket(new_wsocket);
    }

    closesocket(wsocket);
    WSACleanup();

    return 0;
}
