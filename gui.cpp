#include "gui.h"
#include <FL/Fl_Box.H>


gui::gui(int N, int M)//51*N
	: N(N), M(M), Fl_Window(51*M + 250,51*N,"五子棋")
{ 
	//给布局块传入棋盘规格
	Box::set_space(N, M);	
	//布局块对象表 的初始化
	pbox_table.resize(N, std::vector<Box*>(M,NULL) );
	this->begin();
	chessboard_init();
	this->end();
}

int gui::gui_start(){
	this->show();
	return Fl::run();
}

gui::~gui(){
	
}

void gui::show_role(int role){
	if(role == 1){
		courrent_role_box->label("------我是黑棋图片");
	}
	else if(role ==2){
		courrent_role_box->label("我是白棋图片------");
	}
}

void gui::show_state(char* s)const {
	notify_state_box->label(s);
}

void gui::show_result(int role, int state){
	if(role == 1){
		switch(state){
		case 0: return;	//继续
		case 1: 		//黑胜
				victory_state_box->label("黑胜");
				break;
		case 2:			//长连禁手
				victory_state_box->label("白方胜,黑方违反长连禁手！");
				break;
		case 3:			//四四禁手
				victory_state_box->label("白方胜,黑方违反四四禁手！");
				break;
		case 4: 		//三三禁手
				victory_state_box->label("白方胜,黑方违反三三禁手！");
				break;
		case 5:	//平局
				//victory_state_box->label("平局！");
				break;
		default: return;
		}
	}
	if(role == 2){
		switch(state){
		case 0: return;	//继续
		case 1: 		//白胜
				victory_state_box->label("白胜");
				break;
		case 3:			//平局
				//victory_state_box->label("平局！");
				break;
		default: return;
		}
	}
}

void gui::chessboard_init(){
	for(int i=0;i<N;++i){
		for(int j=0;j<M;++j){
			pbox = new Box(i, j);
			pbox_table[i][j] = pbox;
		}
	}
	//250*250
	courrent_role_box = new Fl_Box(M*51, 0, 250, 250,"-----------");
	courrent_role_box->color(FL_RED);
	//250*50
	notify_state_box = new Fl_Box(M*51, 250, 250, 50,"本地模式");
	//250*50
	victory_state_box = new Fl_Box(M*51, 300, 250, 50,"进行中...");
}

void gui::change_image(int role, int x, int y){
	pbox = pbox_table[x][y];
	//if(NULL == pbox) throw "获取Box失败";
	pbox ->change_image(role);
}
