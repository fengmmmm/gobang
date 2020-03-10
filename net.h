#ifndef NET_H
#define NET_H

#include <winsock.h>
#include <iostream>
#include <cstring>
#include <FL/Fl_ask.H>
#include <atomic>
#include <mutex>
#include <cstdlib>

class net{
public:	
	net();
	~net();
	void set_white();
	//true:初始化成功, false:失败
	bool net_init();	
	
	//0:正常接收, 1:等待超时, 2:对方退出 3:其他,视为接收错误
	int net_recv(int& role, int& x, int& y);	
	
	//true:正常发送 false:发送失败
	bool net_send(int role, int x, int y);	
	bool net_accept();
	//测试变量 connected_flag
	bool is_connected();
	
private:
	SOCKET listen_sock, connect_sock;
	SOCKADDR_IN addr;
	bool init_black();	//黑方的特有初始化
	bool init_white(); 	//白方的特有初始化
	int role;			//1:黑方, 2:白方	
	std::atomic<bool> connected_flag;	//连接是否成功
};

#endif