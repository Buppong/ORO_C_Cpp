#define _WINSOCK_DEPRECATED_NO_WARNINGS //대체되었거나 주의해서 사용하라는 말이다.
#include<iostream> 
#include<string>
#include<WinSock2.h>

using namespace std;



void ShowErrorMessage(string message) {

	cout <<"[오류발생]:" << message << '\n';
	system("pause");
	exit(1);
}

int main()
{
	WSADATA wsaData;
	SOCKET clientSocket; // 서버주소를 받은것
	SOCKADDR_IN serverAddress; //서버주소를 담기위해서

	int severPort = 9876; //서버랑동일포트를 사용해야함
	char received[256]; 
	string sent;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //WINsock을 초기화합니다.
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0); //tcp 소켓을 생성합니다.

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); // 서버주소를 0으로 초기화한이후에 서버에 대한정보를 집어넣었다.
	serverAddress.sin_family= AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); //문자열 IP를 네트워크 바이트 형식으로
	serverAddress.sin_port = htons(severPort); //2바이트 정수 네트워크 바이트 형식으로
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect()");
	cout << "[현재 상태] connect()\n";

	while (1) {	//반복적으로 서버로 메시지를 전송하고 에코 메시지를 전달 받습니다.
		cout << "[메시지전송]";
		getline(cin, sent);
		if (sent == "") continue;
		send(clientSocket, sent.c_str(), sent.length(), 0);
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0';
		if (strcmp(received, "[exit]") == 0) {
			cout << "[서버 메시지]:" << received << '\n';
			break;
		}
		cout << "[서버 메시지]" << received << '\n';
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;




}