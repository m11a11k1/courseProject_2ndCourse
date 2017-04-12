#pragma once
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <windows.h>
#include <icmpapi.h>
#include <locale>
#include <TCHAR.h>
using namespace std;

int answer_type(char *answer,SOCKET sock)
{
	int ret;
	char szSendBuff [1024];
//	cout <<"answer_type"<<endl;
	if (answer[0]=='*')
	{
//		cout <<"func_part"<<endl;
		if (!strcmp(answer,"*shutdown"))
		{
			strcpy(szSendBuff, "shutdown OK");
			cout<<answer;
			system("shutdown -s -f -t 0");
			// Отправка содержимого переменной szSendBuff клиенту
			ret = send(sock, szSendBuff, sizeof(szSendBuff), 0);
		}
		if (!strcmp(answer,"*reboot"))
		{
			strcpy(szSendBuff, "reboot OK");
			cout<<answer;
			system("shutdown -r -f -t 0");
			// Отправка содержимого переменной szSendBuff клиенту
			ret = send(sock, szSendBuff, sizeof(szSendBuff), 0);
		}
		if (strcmp(answer,"*shutdown")&&(strcmp(answer,"*reboot")))
		{
			strcpy(szSendBuff, "command OK");
			for(int l=0;l<strlen(answer);l++)
				answer[l]=answer[l+1];
			system(answer);
			// Отправка содержимого переменной szSendBuff клиенту
			ret = send(sock, szSendBuff, sizeof(szSendBuff), 0);
		}
	}
	else
	{
		wchar_t* wString=new wchar_t[1024];
		MultiByteToWideChar(CP_ACP, 0, answer, -1, wString, 1024);
		MessageBox(0,wString,wString,MB_OK);
		// Подготовка строки для отправки клиенту
		strcpy(szSendBuff, "message OK");

		// Отправка содержимого переменной szSendBuff клиенту
		ret = send(sock, szSendBuff, sizeof(szSendBuff), 0);
	}
	return 0;
}

DWORD WINAPI ClientThread(LPVOID lpParam)
{
	SOCKET	sock=(SOCKET)lpParam;
	char	szRecvBuff[1024],
		szSendBuff[1024];
	int	ret;
	// Запуск бесконечного цикла
	while(1)
	{
		// Получение данных
		ret = recv(sock, szRecvBuff, 1024, 0);
		// Проверка полученных данных
		if (ret == 0)
			break;
		else if (ret == SOCKET_ERROR)
		{
			cout<< "Recive data failed"<<endl;
			break;
		}
		szRecvBuff[ret] = 0;
//		cout <<szRecvBuff<<" "<<ret<<endl;
		answer_type(szRecvBuff,sock);
	}
	return 0;
}

DWORD WINAPI NetThread(LPVOID lpParam)
{
	SOCKET	sServerListen,
		sClient;
	struct sockaddr_in localaddr,
			clientaddr;
	HANDLE	hThread;
	DWORD	dwThreadId;
	int	iSize;

	// Создание сокета
	sServerListen = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (sServerListen == SOCKET_ERROR)
	{
		cout<<"Can't load WinSock"<<endl;
		return 0;
	}
	// Заполнение структуры localaddr типа sockaddr_in
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons(2282);

	// Связывание адреса с переменной localaddr типа sockaddr_in
	if (bind(sServerListen, (struct sockaddr *)&localaddr, 
		sizeof(localaddr)) == SOCKET_ERROR)
	{
		cout<<"Can't bind"<<endl;
		return 1;
	}

	// Вывод сообщения об удачной операции bind
	cout<<"Bind OK"<<endl;

	// Запуск прослушивания порта
	listen(sServerListen, 4);

	// Вывод сообщения об удачном начале операции прослушивания
	cout<<"Listen OK"<<endl;

	// Запуск бесконечного цикла
	while (1)
	{
		iSize = sizeof(clientaddr);
		// Прием соединения из очереди. Если его нет,
		// то функция будет ожидать соединения клиента
		sClient = accept(sServerListen, (struct sockaddr *)&clientaddr,
				&iSize);
		//Проверка корректности идентификатора клиентского сокета
		if (sClient == INVALID_SOCKET)
		{        
			cout<< "Accept failed"<<endl;
			break;
		}

		// Создание нового потока для работы с клиентом
		hThread = CreateThread(NULL, 0, ClientThread, 
			(LPVOID)sClient, 0, &dwThreadId);
		if (hThread == NULL)
		{
			cout<<"Create thread filed"<<endl;
			break;
		}
		CloseHandle(hThread);
	}
	// Закрытие сокета после работы с потоком
	closesocket(sServerListen);
	return 0;
}

int main()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
	{
		cout<<"error_wsadata(winsock)"<<endl;
		return 0;
	}
	HANDLE	hNetThread;
	DWORD	dwNetThreadId;
	hNetThread = CreateThread(NULL, 0, NetThread, 0, 0, &dwNetThreadId);
	WaitForMultipleObjects(1, &hNetThread, TRUE, INFINITE);
	CloseHandle(hNetThread);
	return 77;
}