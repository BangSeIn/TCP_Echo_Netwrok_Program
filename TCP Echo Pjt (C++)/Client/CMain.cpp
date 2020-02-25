#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message) {
	cout << "ERROR OCCURED! : " << message << endl;
	system("pause");
	exit(1);
}

int main() {

	WSADATA WsaData;
	SOCKET ClientSocket;
	SOCKADDR_IN ServerAddress;

	int ServerPort = 9704;
	char Received[256];
	string sent;

	if (WSAStartup(MAKEWORD(2, 2), &WsaData) != 0) // initialize Winsock
		ShowErrorMessage("WSAStartup Error");

	ClientSocket = socket(PF_INET, SOCK_STREAM, 0); //create TCP socket

	if (ClientSocket == INVALID_SOCKET)
		ShowErrorMessage("Socket Error");

	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); //from string IP to network byte type
	ServerAddress.sin_port = htons(ServerPort); // from 2 bytes of integar to network byte type
	if (connect(ClientSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress)) == SOCKET_ERROR)
		ShowErrorMessage("Connect Error");
	cout << "[Current Status] : Connected" << endl;

	while (1) {//Recursively sending message to server and receive echo message
		cout << "[Send Message to Echo] : ";
		getline(cin, sent);
		if (sent == "") continue;
		send(ClientSocket, sent.c_str(), sent.length(), 0);
		int length = recv(ClientSocket, Received, sizeof(Received), 0);
		Received[length] = '\0';
		if (strcmp(Received, "[quit]") == 0) {
			cout << "[Quit Server]" << endl;
			break;
		}
		cout << "[Server] Echo : " << Received << endl;
	}

	closesocket(ClientSocket);
	WSACleanup();
	system("pause");
	return 0;
}
