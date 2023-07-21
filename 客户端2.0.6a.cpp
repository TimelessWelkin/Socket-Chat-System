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
	system("title 客户端2.0.6a");
	cout<<"使用本程序须知："<<endl;
	cout<<"  1. 由于本程序尚不完善，所以在输入时尽量不要出现空格（否则会有一些不优雅的显示问题）"<<endl;
	cout<<"  2. 由于该程序导致的一切问题，作者 CaxxSzn233 不负任何责任"<<endl;
	cout<<"  3. 如果程序出了 BUG ，可以告知作者或看一看博客中的 >使用手册<"<<endl;
	cout<<"  4. 该程序版本还在更新，如出现新旧版本冲突的情况请更新你的客户端或服务器端"<<endl;
	cout<<"  5. 擅自修改代码等且擅自改动开发者名字使用的版本将 依法追究"<<endl;
	cout<<"  6. 该程序由______等人启发而来，如有问题，请谅解"<<endl;
	cout<<"如果你对这些条件有问题，你有 10秒 时间关闭该程序"<<endl;
//	Sleep(10000);
	system("cls");
	char Tag[32];
	cout<<"请输入你的名字或昵称：";
	cin>>Tag;
	cout<<"请输入服务器的port：";
	cin>>port;
	getchar();
	system("cls");
	WSADATA wsaData;
	char buff[65536];
	memset(buff,0,sizeof(buff));
	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0) {
		printf("初始化Winsock失败\n");
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
		printf("连接失败:%d\n",WSAGetLastError());
		system("pause");
		return 0;
	}
	else {
		cout<<"+-----------------------------------------------------------------------------------+"<<endl;
		cout<<"| 欢迎使用由 CaxxSzn233 开发的 socket 服务器端程序                                  |"<<endl;
		cout<<"+--------------+----------------------+----------------------------+----------------+"<<endl;
		cout<<"| 服务器：     |                      | 版本号：2.0.6a             | 连接状态：良好 |"<<endl;
		cout<<"+--------------+----------------------+----------------------------+----------------+"<<endl;
		cout<<"| 输入：                                                                            |"<<endl;
		cout<<"+-----------------------------------------------------------------------------------+"<<endl;
		cout<<"| 以下是聊天内容：                                                                  |"<<endl;
		cout<<"+-----------------------------------------------------------------------------------+"<<endl;
	}
	bb.X=0,bb.Y=9;
	getto(10,3),cout<<port;
	getto(17,3),cout<<Tag;
	getto(8,5);
	int recv_len;
	int count=1;
	char buffs[65536],buf[65536];
	sprintf(buffs,"%s 加入了服务器",Tag);
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
			sprintf(buffs,"%s分享了代码:\n%s",Tag,buf);
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
