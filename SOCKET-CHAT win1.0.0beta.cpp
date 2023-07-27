#include <cstring>
#include <stdlib.h>
#include <iostream>
#include "winsock2.h"
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

bool hasNewMessage=false;
char newMessage[65536],proc[65536];
char recvBuf[4096],name[32];
int nowstage=0;
SOCKET sockConn,sockClient;
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
DWORD WINAPI GetMessage(LPVOID);
void GetIntoSocket(int);
void SendMessageToServer(char[]);
bool DecodeSockPort(char[]);

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
	wc.lpszClassName,"Chat Client System Windows Upgrade 1.0.0beta",
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
			UpdateWindow(hwndEditbox);
			SetWindowText(hwndEditbox,"Your name?");
			hwndTalkingbox=CreateWindow(TEXT("edit"),NULL,
			WS_CHILD|WS_VISIBLE|WS_VSCROLL|ES_READONLY|
			WS_BORDER|ES_MULTILINE|ES_WANTRETURN,
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
						if(nowstage==0) {
							sprintf(name,"%s",buff),++nowstage;
							SetWindowText(hwndEditbox,
							"The Server Port?");
						}
						else if(nowstage==1) {
							if(DecodeSockPort(buff)) nowstage++;
							else SetWindowText(hwndEditbox,
							"Your Port is wrong.");
						}
						else {
							SendMessageToServer(buff);
							sprintf(buffs,"%s : %s",name,buff);
							send(sockClient,buffs,strlen(buffs),0);
						}
					}
					else
						MessageBox(NULL,"You don\'t input anything.",
						"Send Button",MB_OK);
					if(hasNewMessage) {
						hasNewMessage=false;
						GetWindowText(hwndTalkingbox,debuff,65536);
						sprintf(debuff,"%s%s",debuff,newMessage);
						SetWindowText(hwndTalkingbox,debuff);
					}
					break;
				case IDeditBox:
					GetWindowText(hwndEditbox,buff,4096);
					if(hasNewMessage) {
						hasNewMessage=false;
						GetWindowText(hwndTalkingbox,debuff,65536);
						sprintf(debuff,"%s%s",debuff,newMessage);
						SetWindowText(hwndTalkingbox,debuff);
					}
					break;
			}
			return 0;

		case WM_CLOSE:
			DestroyWindow(hwnd);
			return 0;
			
		case WM_DESTROY:
			ShellAbout(hwnd,"Chat Client Windows Upgrade 1.0.0 beta",
			"Welcome to take a look at timelesswelkin.github.io",NULL);
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}
DWORD WINAPI GetMessage(LPVOID lpParamter) {
	string make;
	while(true) {
		memset(recvBuf,0,sizeof(recvBuf));
		recv(sockConn,recvBuf,sizeof(recvBuf),0);
		if(!hasNewMessage) {
			hasNewMessage=true;
			memset(newMessage,0,sizeof(newMessage));
		}
		sprintf(proc,"%s%s\r\n",newMessage,recvBuf);
		sprintf(newMessage,"%s",proc);
	}
	closesocket(sockConn);
}
void GetIntoSocket(int port) {
	WSADATA     wsaData;
	char        buff[2048];
	SOCKADDR_IN addrSrv;
	int         recv_len;
	HANDLE      hThread;

	memset(buff,0,sizeof(buff));
	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0) {
		MessageBox(NULL,"SOCKET ERROR0",
		"Error Message",MB_ICONERROR|MB_OK);
		return ;
	}
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(port);
	addrSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
	sockClient=socket(AF_INET,SOCK_STREAM,0);
	if(SOCKET_ERROR==sockClient) {
		MessageBox(NULL,"SOCKET ERROR1",
		"Error Message",MB_ICONERROR|MB_OK);
		return ;
	}
	if(connect(sockClient,(struct sockaddr*)&addrSrv,sizeof(addrSrv))
	==INVALID_SOCKET) {
		MessageBox(NULL,"SOCKET ERROR2",
		"Error Message",MB_ICONERROR|MB_OK);
		return ;
	}
	cout<<"join finishied"<<endl;
	sprintf(buff,"%s joined the server",name);
	send(sockClient,buff,strlen(buff),0);
	sockConn=sockClient;
	hThread=CreateThread(NULL,0,GetMessage,NULL,0,NULL);
	CloseHandle(hThread);
	return ;
}
void SendMessageToServer(char mss[]) {
	cout<<mss<<endl;
	char sendBuff[4096];
	sprintf(sendBuff,"%s : %s",name,mss);
	cout<<mss<<endl;
	send(sockClient,sendBuff,strlen(sendBuff),0);
	return ;
}
bool DecodeSockPort(char code[]) {
	int len=strlen(code),port;
	if(len!=16) return false;
	for(int i=0;i+3<len;i+=4) {
		int r=(int)(code[i]+code[i+1]+code[i+2]+code[i+3])%10;
		port=port*10+r;
	}
	cout<<"join in "<<port<<endl;
	GetIntoSocket(port);
	return true;
}
