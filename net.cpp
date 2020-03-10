#include "net.h"

//初始化为未连接，默认以黑方的方式初始化
net::net():
	connected_flag(false), role(1)
{ }

net::~net(){
	closesocket(connect_sock);
	if (role == 1){
		closesocket(listen_sock);
	}
}

bool net::net_init(){
	//初始化winsock
	WSADATA data;
	int ret = WSAStartup(MAKEWORD(2, 2), &data);
	if (ret){
		fl_alert("winsock 初始化失败");
		return false;
	}
	if (data.wVersion != MAKEWORD(2, 2)){
		fl_alert("winsock 版本错误");
		return false;
	}
	//初始化socket
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	//以黑方的方式初始化
	if (role == 1){
		addr.sin_addr.S_un.S_addr = INADDR_ANY;
		return init_black();
	}
	//白方初始化
	else if (role == 2){
		addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		return init_white();
	}
	return false;
}

void net::set_white(){
	role = 2;
}

bool net::is_connected(){
	return connected_flag.load();
}

bool net::init_black(){
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (!listen_sock){
		fl_alert("socket 创建失败");
		return false;
	}
	if (::bind(listen_sock, (SOCKADDR*)&addr, sizeof(SOCKADDR)) != 0){
		fl_alert("socket 绑定失败");
		return false;
	}
	if (listen(listen_sock, 5)){
		fl_alert("socket 监听失败");
		return false;
	}
	return true;
}

bool net::init_white(){
	connect_sock = socket(AF_INET, SOCK_STREAM, 0);
	int recv_timeout = 100;
	setsockopt(connect_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&recv_timeout, sizeof(int));	//每100ms为一次超时
	if (connect(connect_sock, (SOCKADDR*)&addr, sizeof(SOCKADDR)) != 0){
		return false;
	}
	connected_flag.store(true);
	return true;
}

bool net::net_accept(){
	connect_sock = accept(listen_sock, NULL, NULL);
	if (-1 == connect_sock){
		fl_alert("accept error");
		return false;
	}
	//对方已连接
	int recv_timeout = 100;
	setsockopt(connect_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&recv_timeout, sizeof(int));	//每100ms为一次超时
	connected_flag.store(true);
	return true;
}




bool net::net_send(int role, int x, int y){
	char buf[12];
	memcpy(buf, &role, 4); memcpy(buf + 4, &x, 4); memcpy(buf + 8, &y, 4);
	int len = send(connect_sock, buf, 12, 0);
	//发送失败
	if (12 != len){
		return false;
	}
	return true;
}

int net::net_recv(int& role, int& x, int& y){
	char buf[12];
	int len = recv(connect_sock, buf, 12, 0);
	//接收成功
	if (12 == len){
		memcpy(&role, buf, 4); memcpy(&x, buf + 4, 4); memcpy(&y, buf + 8, 4);
		return 0;
	}
	if(-1 == len){
		int err = WSAGetLastError();
		if(err == WSAETIMEDOUT) return 1;	//超时100ms
		if(err == WSAECONNRESET) return 2;	//对方重置连接
		else return 3;			//其他错误
	}
	if (0 == len){	//对方正常退出
		return 2;
	}
	return 3;	//其他错误
}


