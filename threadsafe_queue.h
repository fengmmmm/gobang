#ifndef THREADSAFE_QUEUE_H
#define THREADSAFE_QUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>

//单例模式下带阻塞的线程安全队列
class threadsafe_queue{
public:
	static threadsafe_queue* get_instance();
	static void destroy_instance();
	~threadsafe_queue();
	void push(int role, int x, int y);
	void wait_and_pop(int& role, int& x, int& y);
private:
	threadsafe_queue();
	std::mutex mut;
	std::condition_variable data_cond;
	std::queue<int> que;
	static threadsafe_queue* instance;
};

#endif