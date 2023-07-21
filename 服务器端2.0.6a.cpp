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
	system("title 服务器2.0.6a");
	cout<<"使用本程序须知："<<endl;
	cout<<"  1. 由于该程序导致的一切问题，作者 CaxxSzn233 不负任何责任"<<endl;
	cout<<"  2. 如果程序出了 BUG ，可以告知作者或看一看博客中的 >使用手册<"<<endl;
	cout<<"  3. 该程序版本还在更新，如出现新旧版本冲突的情况请更新你的客户端或服务器端"<<endl;
	cout<<"  4. 擅自修改代码等且擅自改动开发者名字使用的版本将 依法追究"<<endl;
	cout<<"  5. 该程序由______等人启发而来，如有问题，请谅解"<<endl;
	cout<<"如果你对这些条件有问题，你有 10秒 时间关闭该程序"<<endl;
//	Sleep(10000);
	system("cls");
	WSADATA wsadata;
	int port;
	cout<<"请输入服务器port：（即你的服务器的接口，一个四位整数，你需要它来让用户连接你的服务器）"<<endl;
	cin>>port;
	system("cls");
	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0) {
		printf("初始化失败。");
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
		printf("连接失败:%d\n",WSAGetLastError());
		system("pause");
		return 0;
	}
	if(listen(sockSrv,10)==SOCKET_ERROR) {
		printf("监听失败:%d\n",WSAGetLastError());
		system("pause");
		return 0;
	}
	cout<<"+-------------------------------------------------+"<<endl;
	cout<<"| 欢迎使用由 CaxxSzn233 开发的 socket 服务端程序  |"<<endl;
	cout<<"+------------------+----------------+-------------+"<<endl;
	cout<<"| 服务器端         | 版本号：2.0.6a | port: "<<port<<"  |"<<endl;
	cout<<"+------------------+----------------+-------------+"<<endl;
	cout<<"| 线程数：0        | 连接状态：良好               |"<<endl;
	cout<<"+------------------+------------------------------+"<<endl;
	cout<<"|                                                 |"<<endl;
	cout<<"+-------------------------------------------------+"<<endl;
	cout<<"| 以下是聊天内容：                                |"<<endl;
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
			printf("等待请求失败:%d\n",WSAGetLastError());
			break;
		}
		sockVec.push_back(sockConn);
		printf("IP[%s]加入了服务器\n",inet_ntoa(addrCilent.sin_addr));
		HANDLE hThread=CreateThread(NULL,0,Fun,NULL,0,NULL);
		CloseHandle(hThread);
	}
	closesocket(sockSrv);
	WSACleanup();
	system("pause");
	return 0;
}
