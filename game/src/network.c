#include "network.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_IP "127.0.0.1" // Localhost for testing

static SOCKET ConnectSocket = INVALID_SOCKET;
static SOCKET ListenSocket = INVALID_SOCKET;
static SOCKET ClientSocket = INVALID_SOCKET;

static bool isServer = false;

bool Network_Init(int* localPlayerId) {
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return false;
    }

    // Determine if we are Server or Client based on arguments?
    // For now, let's try to bind to the port. If we can, we are Server.
    // If not (port in use), we try to connect as Client.
    // NOTE: This is a simple auto-discovery for local testing. 
    // Ideally pass args, but user asked for "LAN" and "minimal changes".
    
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return false;
    }

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }

    // Attempt to bind
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        // Bind failed, likely port in use. Assume we are CLIENT.
        // Close ListenSocket and try to connect.
        closesocket(ListenSocket);
        ListenSocket = INVALID_SOCKET;
        freeaddrinfo(result);

        // -- CLIENT MODE --
        isServer = false;
        *localPlayerId = 1;
        printf("Bind failed (Port busy?). Starting as CLIENT (Player 2)...\n");

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        // Connect to localhost by default for now. 
        // In a real LAN scenario, we'd scan or ask for IP.
        // Assuming "LAN" means testing on same machine or manually changing IP in code for now 
        // as per "minimal changes" request.
        // Let's hardcode localhost for loopback first.
        iResult = getaddrinfo(DEFAULT_IP, DEFAULT_PORT, &hints, &result);
        if (iResult != 0) {
            printf("getaddrinfo failed: %d\n", iResult);
            WSACleanup();
            return false;
        }

        for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
            ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
            if (ConnectSocket == INVALID_SOCKET) {
                WSACleanup();
                return false;
            }

            iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (iResult == SOCKET_ERROR) {
                closesocket(ConnectSocket);
                ConnectSocket = INVALID_SOCKET;
                continue;
            }
            break;
        }

        freeaddrinfo(result);

        if (ConnectSocket == INVALID_SOCKET) {
            printf("Unable to connect to server!\n");
            WSACleanup();
            return false;
        }

        // Set Non-Blocking
        u_long mode = 1;
        ioctlsocket(ConnectSocket, FIONBIO, &mode);
        
        printf("Connected to Server!\n");
        return true;

    } else {
        // Bind Success -- SERVER MODE
        isServer = true;
        *localPlayerId = 0;
        printf("Bind success. Starting as SERVER (Player 1)...\n");
        freeaddrinfo(result);

        iResult = listen(ListenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            printf("Listen failed: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return false;
        }

        // Set Listen Non-Blocking
        u_long mode = 1;
        ioctlsocket(ListenSocket, FIONBIO, &mode);

        printf("Waiting for client connection...\n");
        return true;
    }
}

void Network_Update(Input* p1Input, Input* p2Input, int localPlayerId) {
    if (isServer) {
        // -- SERVER LOGIC --
        // 1. Accept Client if not connected
        if (ClientSocket == INVALID_SOCKET) {
            ClientSocket = accept(ListenSocket, NULL, NULL);
            if (ClientSocket != INVALID_SOCKET) {
                printf("Client connected!\n");
                // Set Client Socket Non-Blocking
                u_long mode = 1;
                ioctlsocket(ClientSocket, FIONBIO, &mode);
            }
        }

        if (ClientSocket != INVALID_SOCKET) {
             // 0. Disable Nagle's Algorithm
            int flag = 1;
            setsockopt(ClientSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));

            // 2. Send P1 Input
            send(ClientSocket, (char*)p1Input, sizeof(Input), 0);

            // 3. Receive P2 Input (Drain buffer to get latest)
            int bytesReceived;
            do {
                Input tempInput;
                bytesReceived = recv(ClientSocket, (char*)&tempInput, sizeof(Input), 0);
                if (bytesReceived == sizeof(Input)) {
                    *p2Input = tempInput;
                }
            } while (bytesReceived > 0);

            if (bytesReceived == SOCKET_ERROR) {
                 int err = WSAGetLastError();
                 if (err != WSAEWOULDBLOCK) {
                     printf("Client disconnected/Error.\n");
                     closesocket(ClientSocket);
                     ClientSocket = INVALID_SOCKET;
                 }
            } else if (bytesReceived == 0) {
                printf("Client disconnected.\n");
                closesocket(ClientSocket);
                ClientSocket = INVALID_SOCKET;
            }
        }
    } else {
        // -- CLIENT LOGIC --
        if (ConnectSocket != INVALID_SOCKET) {
             // 0. Disable Nagle's Algorithm (do once, but here is safe too)
            int flag = 1;
            setsockopt(ConnectSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));

            // 1. Send P2 Input
            send(ConnectSocket, (char*)p2Input, sizeof(Input), 0);

            // 2. Receive P1 Input (Drain buffer)
            int bytesReceived;
            do {
                Input tempInput;
                bytesReceived = recv(ConnectSocket, (char*)&tempInput, sizeof(Input), 0);
                if (bytesReceived == sizeof(Input)) {
                    *p1Input = tempInput;
                }
            } while (bytesReceived > 0);

            if (bytesReceived == SOCKET_ERROR) {
                 int err = WSAGetLastError();
                 if (err != WSAEWOULDBLOCK) {
                     printf("Server disconnected/Error.\n");
                     closesocket(ConnectSocket);
                     ConnectSocket = INVALID_SOCKET; // Stop trying
                 }
            } else if (bytesReceived == 0) {
                 printf("Server disconnected.\n");
                 closesocket(ConnectSocket);
                 ConnectSocket = INVALID_SOCKET;
            }
        }
    }
}

void Network_Shutdown() {
    if (ListenSocket != INVALID_SOCKET) closesocket(ListenSocket);
    if (ClientSocket != INVALID_SOCKET) closesocket(ClientSocket);
    if (ConnectSocket != INVALID_SOCKET) closesocket(ConnectSocket);
    WSACleanup();
}
