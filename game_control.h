#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include <atomic>
#include <memory>
#include <thread>
#include <FL/Fl_ask.H>
#include "gui.h"
#include "threadsafe_queue.h"
#include "gobang_algorithm.h"
#include "net.h"
#include "Box.h"	//仅使用静态函数设置白方和网络模式

class game_control{
public:
	game_control();
	~game_control();
	void set_size();
	void set_white();
	void set_net();
	void game_start();
	//结束所有任务
	void set_end_flag();
private:
	//从socket or clickEvent中接收坐标和棋子，放入队列(网络模式下有两个生产者)
	void recv_task();
	//从队列中取出坐标和棋子,送入GUI消费线程(唯一一个消费者)
	void gui_task();
	void local_mode();
	void net_mode();
	
	
	int local_role, expected_role;	//设置棋手的黑白方,网络模式下才有效;
	threadsafe_queue* que;
	gobang_algorithm* gobang_algo;
	bool net_flag; 					//是否为网络模式
	std::atomic<bool> end_flag;		//打断子线程任务的标记
	gui* g;
	//std::shared_ptr<gui> g;
	net* net_sync;
	std::thread recv_thread, gui_thread;
};

#endif