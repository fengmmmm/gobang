#include "threadsafe_queue.h"

threadsafe_queue* threadsafe_queue::instance = NULL;

threadsafe_queue::threadsafe_queue(){

}

threadsafe_queue::~threadsafe_queue(){

}

threadsafe_queue* threadsafe_queue::get_instance(){
	static std::once_flag flag;
	std::call_once(flag, [](){
		instance = new threadsafe_queue;
	});
	return instance;
}

void threadsafe_queue::destroy_instance(){
	//析构非线程安全
	if (NULL != instance){
		delete instance; instance = NULL;
	}
}

void threadsafe_queue::push(int role, int x, int y){
	std::lock_guard<std::mutex> lk(mut);
	que.push(role); que.push(x); que.push(y);
	data_cond.notify_one();
}

void threadsafe_queue:: wait_and_pop(int& role, int& x, int& y){
	std::unique_lock<std::mutex> lk(mut);
	data_cond.wait(lk, [this](){
		return !que.empty();
	});
	role = que.front(); que.pop();
	x = que.front(); que.pop();
	y = que.front(); que.pop();
}