#include <iostream>
#include <string>
#include <WinSock2.h>
 
using namespace std;

void ShowErrorMessage(string message) {
    cout << "ERROR OCCURED! : " << message << endl;
    system("pause");
    exit(1);
}

int main()
{
    WSADATA WsaData;
    SOCKET ServerSocket, ClientSocket;
    SOCKADDR_IN ServerAddress, ClientAddress;

    int ServerPort = 9704;
    char Received[256];

    if (WSAStartup(MAKEWORD(2, 2), &WsaData) != 0) // initialize Winsock
        ShowErrorMessage("WSAStartup Error");

    ServerSocket = socket(PF_INET, SOCK_STREAM, 0); //create TCP socket

    if (ServerSocket == INVALID_SOCKET)
        ShowErrorMessage("Socket Error");

    memset(&ServerAddress, 0, sizeof(ServerAddress));
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY); //from 4 bytes of integar to network byte
    ServerAddress.sin_port = htons(ServerPort); //from 2 bytes of integar to network byte
    if (bind(ServerSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress)) == SOCKET_ERROR)
        ShowErrorMessage("Binding..");
    cout << "[Current Status] : Binding..\n";

    if (listen(ServerSocket, 5) == SOCKET_ERROR)
        ShowErrorMessage("Listening..");
    cout << "[Current Status] : Listening..\n";

    int SizeClientAddress = sizeof(ClientAddress);
    ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddress, &SizeClientAddress);
    cout << "[Current Status] : Accepted\n";

    if (ClientSocket == INVALID_SOCKET)
        ShowErrorMessage("Accepted");
    while (1) { //Recieve client's message and regive it
        int length = recv(ClientSocket, Received, sizeof(Received), 0);
        Received[length] = NULL;
        cout << "[Message from Client] : " << Received << endl;
        cout << "[Sending Message] -> " << Received << endl;
        if (strcmp(Received, "[quit]") == 0) {
            send(ClientSocket, Received, sizeof(Received) - 1, 0);
            cout << "[Quit Server]\n";
            break;
        }
        send(ClientSocket, Received, sizeof(Received) - 1, 0);
    }

    closesocket(ClientSocket);
    closesocket(ServerSocket);
    WSACleanup();
    system("pause");
    return 0;

}   