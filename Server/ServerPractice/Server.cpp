#include <cstdio>
#include <cstdlib>
#include <winsock2.h>

const int PORT		= 9090;

void ErrorHandling(char* message) 
{
	fputs( message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main()
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int clientAddressSize;
	char msg[128] = "Hello World!";

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}

	serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(PORT);
	
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		ErrorHandling("bind() error");
	}

	if (listen(serverSocket, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() error");
	}

	clientAddressSize = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &clientAddressSize);
	if (clientSocket == INVALID_SOCKET) {
		ErrorHandling("accept() error");
	}

	send(clientSocket, msg, sizeof(msg), 0);
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();

	return 0;
}