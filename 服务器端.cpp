#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "winsock2.h"
#include <cstdlib>
#include <vector>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

COORD GetConsoleCursor() {
	COORD coordScreen={0,0};
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi))
		coordScreen=csbi.dwCursorPosition;
	return coordScreen;
}
void getto(int lie,int hang) {
	COORD coord;
	coord.X=lie,coord.Y=hang;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
	return ;
}
void gettoCoord(COORD coord) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
	return ;
}

char recvBuf[65536];
COORD bb,ccoord;
SOCKET sockConn;
vector<SOCKET> sockVec;
int count=0,m=0;
DWORD WINAPI Fun(LPVOID lpParamter) {
	getto(10,5);
	cout<<++m;
	gettoCoord(ccoord);
	while(true) {
		memset(recvBuf,0,sizeof(recvBuf));
		recv(sockConn,recvBuf,sizeof(recvBuf),0);
		ccoord=GetConsoleCursor();
		gettoCoord(bb);
		printf("%s\n",recvBuf);
		bb=GetConsoleCursor();
		getto(2,7);
		cout<<"Massage "<<++count<<" Pass";
		gettoCoord(ccoord);
		for(int i=0;i<sockVec.size();i++) send(sockVec[i],recvBuf,strlen(recvBuf),0);
	}
	closesocket(sockConn);
}

int main() {
	system("title ������2.0.6a");
	cout<<"ʹ�ñ�������֪��"<<endl;
	cout<<"  1. ���ڸó����µ�һ�����⣬���� CaxxSzn233 �����κ�����"<<endl;
	cout<<"  2. ���������� BUG �����Ը�֪���߻�һ�������е� >ʹ���ֲ�<"<<endl;
	cout<<"  3. �ó���汾���ڸ��£�������¾ɰ汾��ͻ������������Ŀͻ��˻��������"<<endl;
	cout<<"  4. �����޸Ĵ���������ԸĶ�����������ʹ�õİ汾�� ����׷��"<<endl;
	cout<<"  5. �ó�����______���������������������⣬���½�"<<endl;
	cout<<"��������Щ���������⣬���� 10�� ʱ��رոó���"<<endl;
//	Sleep(10000);
	system("cls");
	WSADATA wsadata;
	int port;
	cout<<"�����������port��������ķ������Ľӿڣ�һ����λ����������Ҫ�������û�������ķ�������"<<endl;
	cin>>port;
	system("cls");
	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0) {
		printf("��ʼ��ʧ�ܡ�");
		system("pause");
		return 0;
	}
	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(port);
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	int retVal=bind(sockSrv,(LPSOCKADDR)&addrSrv,sizeof(SOCKADDR_IN));
	if(retVal==SOCKET_ERROR) {
		printf("����ʧ��:%d\n",WSAGetLastError());
		system("pause");
		return 0;
	}
	if(listen(sockSrv,10)==SOCKET_ERROR) {
		printf("����ʧ��:%d\n",WSAGetLastError());
		system("pause");
		return 0;
	}
	cout<<"+-------------------------------------------------+"<<endl;
	cout<<"| ��ӭʹ���� CaxxSzn233 ������ socket ����˳���  |"<<endl;
	cout<<"+------------------+----------------+-------------+"<<endl;
	cout<<"| ��������         | �汾�ţ�2.0.6a | port: "<<port<<"  |"<<endl;
	cout<<"+------------------+----------------+-------------+"<<endl;
	cout<<"| �߳�����0        | ����״̬������               |"<<endl;
	cout<<"+------------------+------------------------------+"<<endl;
	cout<<"|                                                 |"<<endl;
	cout<<"+-------------------------------------------------+"<<endl;
	cout<<"| �������������ݣ�                                |"<<endl;
	cout<<"+-------------------------------------------------+"<<endl;
	ccoord.X=0,ccoord.Y=11;
	bb.X=0,bb.Y=11;
	SOCKADDR_IN addrCilent;
	int len=sizeof(SOCKADDR);
	while(true) {
		sockConn=accept(sockSrv,(SOCKADDR *) &addrCilent,&len);
		getto(2,7);
		cout<<"A IP join in.";
		gettoCoord(ccoord);
		if(sockConn==SOCKET_ERROR) {
			printf("�ȴ�����ʧ��:%d\n",WSAGetLastError());
			break;
		}
		sockVec.push_back(sockConn);
		printf("IP[%s]�����˷�����\n",inet_ntoa(addrCilent.sin_addr));
		HANDLE hThread=CreateThread(NULL,0,Fun,NULL,0,NULL);
		CloseHandle(hThread);
	}
	closesocket(sockSrv);
	WSACleanup();
	system("pause");
	return 0;
}
