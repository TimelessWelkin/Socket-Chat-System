#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "winsock2.h"
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
COORD bb;
SOCKET sockConn;
DWORD WINAPI Fun(LPVOID lpParamter) {
	while(true) {
		memset(recvBuf,0,sizeof(recvBuf));
		recv(sockConn,recvBuf,sizeof(recvBuf),0);
		COORD ccoord=GetConsoleCursor();
		gettoCoord(bb);
		printf("- %s\n",recvBuf);
		bb=GetConsoleCursor();
		gettoCoord(ccoord);
	}
	closesocket(sockConn);
}

bool compare(char buf1[],char buf2[]) {
	if(strlen(buf1)!=strlen(buf2)) return false;
	for(int i=0;i<strlen(buf1);i++) if(buf1[i]!=buf2[i]) return false;
	return true;
}

int main() {
	int port;
	system("title �ͻ���2.0.6a");
	cout<<"ʹ�ñ�������֪��"<<endl;
	cout<<"  1. ���ڱ������в����ƣ�����������ʱ������Ҫ���ֿո񣨷������һЩ�����ŵ���ʾ���⣩"<<endl;
	cout<<"  2. ���ڸó����µ�һ�����⣬���� CaxxSzn233 �����κ�����"<<endl;
	cout<<"  3. ���������� BUG �����Ը�֪���߻�һ�������е� >ʹ���ֲ�<"<<endl;
	cout<<"  4. �ó���汾���ڸ��£�������¾ɰ汾��ͻ������������Ŀͻ��˻��������"<<endl;
	cout<<"  5. �����޸Ĵ���������ԸĶ�����������ʹ�õİ汾�� ����׷��"<<endl;
	cout<<"  6. �ó�����______���������������������⣬���½�"<<endl;
	cout<<"��������Щ���������⣬���� 10�� ʱ��رոó���"<<endl;
//	Sleep(10000);
	system("cls");
	char Tag[32];
	cout<<"������������ֻ��ǳƣ�";
	cin>>Tag;
	cout<<"�������������port��";
	cin>>port;
	getchar();
	system("cls");
	WSADATA wsaData;
	char buff[65536];
	memset(buff,0,sizeof(buff));
	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0) {
		printf("��ʼ��Winsockʧ��\n");
		system("pause");
		return 0;
	}
	SOCKADDR_IN addrSrv;
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(port);
	addrSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
	SOCKET sockCilent=socket(AF_INET,SOCK_STREAM,0);
	if(SOCKET_ERROR==sockCilent) {
		printf("Socket() Error:%d\n",WSAGetLastError());
		system("pause");
		return 0;
	}
	if(connect(sockCilent,(struct sockaddr*)&addrSrv,sizeof(addrSrv))==INVALID_SOCKET) {
		printf("����ʧ��:%d\n",WSAGetLastError());
		system("pause");
		return 0;
	}
	else {
		cout<<"+-----------------------------------------------------------------------------------+"<<endl;
		cout<<"| ��ӭʹ���� CaxxSzn233 ������ socket �������˳���                                  |"<<endl;
		cout<<"+--------------+----------------------+----------------------------+----------------+"<<endl;
		cout<<"| ��������     |                      | �汾�ţ�2.0.6a             | ����״̬������ |"<<endl;
		cout<<"+--------------+----------------------+----------------------------+----------------+"<<endl;
		cout<<"| ���룺                                                                            |"<<endl;
		cout<<"+-----------------------------------------------------------------------------------+"<<endl;
		cout<<"| �������������ݣ�                                                                  |"<<endl;
		cout<<"+-----------------------------------------------------------------------------------+"<<endl;
	}
	bb.X=0,bb.Y=9;
	getto(10,3),cout<<port;
	getto(17,3),cout<<Tag;
	getto(8,5);
	int recv_len;
	int count=1;
	char buffs[65536],buf[65536];
	sprintf(buffs,"%s �����˷�����",Tag);
	send(sockCilent,buffs,strlen(buffs),0);
	sockConn=sockCilent;
	HANDLE hThread=CreateThread(NULL,0,Fun,NULL,0,NULL);
	CloseHandle(hThread);
	while(true) {
		memset(buf,0,sizeof(buf));
		cin>>buf;
		if(compare(buf,(char*)"UploadProgram")) {
			ifstream fin;
			fin.open("D:/program.txt");
		    fin.get(buf,65535,'\0');
			sprintf(buffs,"%s�����˴���:\n%s",Tag,buf);
		}
		else {
			for(int i=0;i<strlen(buf);i++) {
				if(buf[i]=='`') buf[i]=' ';
				if(buf[i]=='$') {
					buf[i++]=' ';
					while(buf[i]!='$') i++;
					buf[i]=' ';
				}
			}
			sprintf(buffs,"%s: %s",Tag,buf);
		}
		send(sockCilent,buffs,strlen(buffs),0);
		getto(8,5);
	}
	closesocket(sockCilent);
	WSACleanup();
	system("pause");
	return 0;
}
