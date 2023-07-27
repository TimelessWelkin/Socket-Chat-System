#include <string>
#include <time.h>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include "winsock2.h"
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

bool hasNewMessage=false;
char newMessage[65536],proc[65536],debuff[65536];
char recvBuf[4096],enPort[64],recvbuff[4096];
SOCKET sockSrv,sockConn;
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void GetEncryptionPort();
void MakeSocket(int);
DWORD WINAPI GetNewClient(LPVOID);
struct socketNode {
	int idress;
	SOCKET sockNodeClient;
};
socketNode socks[16];
int socksNum,idcount;
DWORD WINAPI ReceiveMessage(LPVOID);

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
LPSTR lpCmdLine,int nCmdShow) {
	HWND winHwnd=GetConsoleWindow();
	ShowWindow(winHwnd,SW_HIDE);

	MSG        msg;
	RECT       rect;
	WNDCLASSEX wc;
	HWND       hwnd,hwndDT;
	int        dtWidth,dtHeight;

	memset(&wc,0,sizeof(wc));
	wc.cbSize       =sizeof(WNDCLASSEX);
	wc.lpfnWndProc  =WndProc;
	wc.hInstance    =hInstance;
	wc.hCursor      =LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName="WindowClass";
	wc.hIcon        =LoadIcon(NULL,IDI_INFORMATION);
	wc.hIconSm      =LoadIcon(NULL,IDI_INFORMATION);

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL,"Window Registration Failed!","Error!",
		MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwndDT=GetDesktopWindow();
	GetWindowRect(hwndDT,&rect);
	dtWidth=rect.right-rect.left;
	dtHeight=rect.bottom-rect.top;

	hwnd=CreateWindowEx(WS_EX_CLIENTEDGE,
	wc.lpszClassName,"Chat Server System Windows Upgrade 1.0.0beta",
	WS_VISIBLE|WS_OVERLAPPEDWINDOW,
	(dtWidth-593)/2,(dtHeight-426)/2,
	593,426,NULL,NULL,hInstance,NULL);

	if(hwnd==NULL) {
		MessageBox(NULL,"Window Creation Failed!","Error!",
		MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	while(GetMessage(&msg,NULL,0,0)>0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,
WPARAM wParam,LPARAM lParam) {
	HDC         hdc;
	PAINTSTRUCT ps;
	RECT        rect;
	POINT       mouse;
	static int  cxChar,cyChar;
	static HWND hwndButton;
	static HWND hwndEditbox;
	static HWND hwndTalkingbox;
	char        buff[4096]={0},debuff[65536]={0},buffs[4096]={0};
	int         iLength;
	const int   IDcmdButton=1;
	const int   IDeditBox=2;
	const int   IDtalkingBox=3;
	cxChar=LOWORD(GetDialogBaseUnits());
	cyChar=HIWORD(GetDialogBaseUnits());

	switch(message) {
		case WM_CREATE:
			hwndButton=CreateWindow(TEXT("button"),TEXT("Send"),
			WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
			cxChar*3,cyChar*15,cxChar*30,cyChar*2.5,hwnd,
			(HMENU)IDcmdButton,((LPCREATESTRUCT)lParam)->hInstance,
			NULL);
			if(!hwndButton)
				MessageBox(NULL,"Send Button Creation Failed",
				"Error Message",MB_ICONERROR|MB_OK);
			ShowWindow(hwndButton,SW_SHOW);
			UpdateWindow(hwndButton);
			hwndEditbox=CreateWindow(TEXT("edit"),NULL,
			WS_CHILD|WS_VISIBLE|WS_VSCROLL|ES_MULTILINE|WS_BORDER,
			cxChar*3,cyChar*2,cxChar*30,cyChar*9.5,hwnd,
			(HMENU)IDeditBox,NULL,NULL);
			if(!hwndEditbox)
				MessageBox(NULL,"Edit Creation Failed",
				"Error Message",MB_ICONERROR|MB_OK);
			ShowWindow(hwndEditbox,SW_SHOW);
			UpdateWindow(hwndEditbox),GetEncryptionPort();
			SetWindowText(hwndEditbox,enPort);
			hwndTalkingbox=CreateWindow(TEXT("edit"),NULL,
			WS_CHILD|WS_VISIBLE|WS_VSCROLL|
			ES_MULTILINE|WS_BORDER|ES_READONLY,
			cxChar*36,cyChar*2,cxChar*32,cyChar*16,hwnd,
			(HMENU)IDtalkingBox,NULL,NULL);
			if(!hwndTalkingbox)
				MessageBox(NULL,"Talking Creation Failed",
				"Talking Message",MB_ICONERROR|MB_OK);
			ShowWindow(hwndTalkingbox,SW_SHOW);
			UpdateWindow(hwndTalkingbox);
			return 0;

		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case 0:
					PostQuitMessage(0);
					break;
				case IDcmdButton:
					iLength=GetWindowTextLength(hwndEditbox);
					GetWindowText(hwndEditbox,buff,iLength+1);
					buff[iLength]='\0';
					if(buff[0]) {
					}
					else
						MessageBox(NULL,"You don\'t input anything.",
						"Send Button",MB_OK);
					break;
				case IDeditBox:
					GetWindowText(hwndEditbox,buff,4096);
					break;
			}
			return 0;

		case WM_MOUSEMOVE:
			if(hasNewMessage) {
				hasNewMessage=false;
				GetWindowText(hwndTalkingbox,debuff,65536);
				sprintf(debuff,"%s%s",debuff,newMessage);
				SetWindowText(hwndTalkingbox,debuff);
			}
			return 0;

		case WM_CLOSE:
			DestroyWindow(hwnd);
			return 0;

		case WM_DESTROY:
			ShellAbout(hwnd,"Chat Server Windows Upgrade 1.0.0 beta",
			"Welcome to take a look at timelesswelkin.github.io",NULL);
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}
void GetEncryptionPort() {
	srand((unsigned)time(NULL));
	int port=rand()%10000,t,tt;
	cout<<port<<endl;
	char a,b,c,d,s[20];
	t=port/1000%10,a=rand()%32+64,b=rand()%32+64,c=rand()%32+64;
	d=(t-(a+b+c)%10+10)%10+40,s[tt++]=a,s[tt++]=b,s[tt++]=c,s[tt++]=d;
	t=port/100%10,a=rand()%32+64,b=rand()%32+64,c=rand()%32+64;
	d=(t-(a+b+c)%10+10)%10+40,s[tt++]=a,s[tt++]=b,s[tt++]=c,s[tt++]=d;
	t=port/10%10,a=rand()%32+64,b=rand()%32+64,c=rand()%32+64;
	d=(t-(a+b+c)%10+10)%10+40,s[tt++]=a,s[tt++]=b,s[tt++]=c,s[tt++]=d;
	t=port%10,a=rand()%32+64,b=rand()%32+64,c=rand()%32+64;
	d=(t-(a+b+c)%10+10)%10+40,s[tt++]=a,s[tt++]=b,s[tt++]=c,s[tt++]=d;
	s[tt]='\0';
	sprintf(enPort,"port: %s",s);
	MakeSocket(port);
	return ;
}
void MakeSocket(int port) {
	WSADATA     wsaData;
	SOCKADDR_IN addrSrv;
	int         retVal;
	HANDLE      hThread;
	
	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0) {
		MessageBox(NULL,"SOCKET ERROR0",
		"Error Message",MB_ICONERROR|MB_OK);
		return ;
	}
	sockSrv=socket(AF_INET,SOCK_STREAM,0);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(port);
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	retVal=bind(sockSrv,(LPSOCKADDR)&addrSrv,sizeof(SOCKADDR_IN));
	if(retVal==SOCKET_ERROR) {
		MessageBox(NULL,"SOCKET ERROR1",
		"Error Message",MB_ICONERROR|MB_OK);
		return ;
	}
	if(listen(sockSrv,10)==SOCKET_ERROR) {
		MessageBox(NULL,"SOCKET ERROR2",
		"Error Message",MB_ICONERROR|MB_OK);
		return ;
	}
	hThread=CreateThread(NULL,0,GetNewClient,NULL,0,NULL);
	CloseHandle(hThread);
	return ;
}
DWORD WINAPI GetNewClient(LPVOID lpParamter) {
	SOCKET addrClient;
	int    len;
	
	len=sizeof(SOCKADDR);
	while(true) {
		sockConn=accept(sockSrv,(SOCKADDR *)&addrClient,&len);
		if(sockConn==SOCKET_ERROR) break;
		if(socksNum==16) {
			send(sockConn,"The Server is Full.",19,0);
			continue;
		}
		socks[socksNum].idress        =++idcount;
		socks[socksNum].sockNodeClient=sockConn;
		socksNum++;
		HANDLE hThread=CreateThread(NULL,0,ReceiveMessage,NULL,0,NULL);
		CloseHandle(hThread);
	}
	WSACleanup();
}
DWORD WINAPI ReceiveMessage(LPVOID lpParamter) {
	SOCKET sockClient=sockConn;
	int    recvLen=0,sockId=idcount;
	while(true) {
		memset(recvBuf,0,sizeof(recvBuf));
		recvLen=recv(sockClient,recvBuf,sizeof(recvBuf),0);
		if(recvLen<0) break;
		sprintf(recvbuff,"(%d)%s",sockId,recvBuf);
		if(!hasNewMessage) {
			hasNewMessage=true;
			memset(newMessage,0,sizeof(newMessage));
		}
		sprintf(proc,"%s%s\r\n",newMessage,recvbuff);
		sprintf(newMessage,"%s",proc);
		for(int i=0;i<socksNum;i++)
			send(socks[i].sockNodeClient,recvbuff,strlen(recvbuff),0);
	}
	closesocket(sockClient);
}
