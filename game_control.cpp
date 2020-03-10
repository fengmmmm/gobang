#include "game_control.h"

game_control::game_control()
	:local_role(1), expected_role(1), end_flag(false), net_flag(false), 
	g(new gui(10,10)), gobang_algo(new gobang_algorithm(10,10)), net_sync(new net)
{ 
	que = threadsafe_queue::get_instance();
}

game_control::~game_control(){
	end_flag.store(true);
	//打断GUI消费线程的阻塞状态
	que->push(-1,-1,-1);
	gui_thread.join();
	
	delete net_sync;
	if(recv_thread.joinable()){
		recv_thread.join();
	}
	threadsafe_queue::destroy_instance();
	delete gobang_algo;
	delete g;
}

void game_control::set_size(){
	
}

void game_control::set_white(){
	local_role = 2;
	net_sync->set_white();
	Box::set_white();
}

void game_control::set_net(){
	net_flag = true;
	Box::set_net();
}

void game_control::recv_task(){
	if(local_role == 1){				//黑方调用accept
		g->show_state("等待白方连接中...");
		if(!net_sync->net_accept()){	//accept失败
			g->show_state("accept error");
			return;
		}
		g->show_state("白方已连接...");
	}
	int r, x, y;
	while(!end_flag.load()){	
		switch(net_sync->net_recv(r, x, y)){
		case 0: 	//正常接收
			que->push(r, x, y);
			break;
		case 1:		//超时100ms
			continue;
		case 2:		//对方退出
			g->show_state("对面已经溜了~");
			return;
		case 3:
			g->show_state("socket发生了错误！");
			return;
		default: 
			g->show_state("丧心病狂的未知错误!");
			return;
		}
	}
}

void game_control::gui_task(){
	if(!net_flag){
		local_mode();
	}
	else{
		net_mode();
	}
	
}

void game_control::game_start(){
	//启动GUI消费线程
	std::thread tg(&game_control::gui_task,this); gui_thread = std::move(tg);
	if(net_flag){
		g->show_role(local_role);
		if(net_sync->net_init() && local_role == 2){		//如果是白方，在此连接
			g->show_state("已经连上黑方!");
		}
		//黑方启动socket的accept/recv线程
		std::thread cg(game_control::recv_task,this); recv_thread = std::move(cg);
	}
	//使用主线程加载主界面
	g->gui_start();
	
}

void game_control::local_mode(){
	g->show_role(expected_role);
	while(!end_flag.load()){
		int role, x, y;
		que->wait_and_pop(role, x, y);
		if(gobang_algo->insert(role, x, y)){
			g->change_image(role, x, y);
			int game_result = gobang_algo->get_status(role, x, y);
			g->show_result(role, game_result);
			expected_role = !(expected_role-1) + 1;
			g->show_role(expected_role);
			if(game_result){
				g->show_state("-------对局结束-------");
				return;
			}
		}
	}
	//fl_alert("local mode exit...");
}

void game_control::net_mode(){
	int role, x, y;
	while(!end_flag.load()){	
		que->wait_and_pop(role, x, y);
		if(!net_sync->is_connected()){	//还没有连接
			g->show_state("等等吧,对面老年人操作还没上线呢~");
			continue;
		}
		if(expected_role != role){ //还没有轮到本用户下棋
			g->show_state("等等吧,心急吃不了热豆腐O~");
			continue;
		}
		if(role == local_role){	//如果是本用户下的棋子,则发送出去
			if(!net_sync->net_send(role, x, y) ){
				g->show_state("发送错误,对方可能已经离线!");
				return;
			}
		}
		if(gobang_algo->insert(role, x, y)){
			g->change_image(role, x, y);
			int game_result = gobang_algo->get_status(role, x, y);
			g->show_result(role, game_result);
			if(role != local_role){
				g->show_state("---------该你下棋了哦~");
			}else{
				g->show_state("等待对面的下棋.......");
			}
			if(game_result){
				g->show_state("-------对局结束-------");
				return;
			}
			expected_role = !(expected_role-1) + 1;	//棋子颜色反向
		}
	}

}



