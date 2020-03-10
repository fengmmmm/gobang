#include "Box.h"
#include <vector>

int Box::N = 10;
int Box::M = 10;
//默认当前用户为黑色
int Box::current_role = 1;
//默认为本地模式
bool Box::net_flag = false;

Box::Box(int x,int y)
	: x(x), y(y), Fl_Box(y*51,x*51,51,51)
{ 
	insert_image();
}

Box::~Box(){
	
}

void Box::set_space(int n, int m)
{
	N = n, M = m;
}

void Box::set_white(){
	current_role = 2;
}

void Box::set_net(){
	net_flag = true;
}

void Box::insert_image(){
	if( x&&y && x<N-1 && y<M-1){
		pngImg = new Fl_PNG_Image("img\\1.png");
	}
	else if( 0 == x && 0 == y){
		pngImg = new Fl_PNG_Image("img\\4.png");
	}
	else if( 0 == x && M-1 == y){
		pngImg = new Fl_PNG_Image("img\\7.png");
	}
	else if(N-1 == x && 0 == y){
		pngImg = new Fl_PNG_Image("img\\10.png");
	}
	else if(N-1 ==x && M-1 == y){
		pngImg = new Fl_PNG_Image("img\\13.png");
	}
	
	else if(0 == x){
		pngImg = new Fl_PNG_Image("img\\16.png");
	}
	else if(0 == y){
		pngImg = new Fl_PNG_Image("img\\19.png");
	}
	else if(N-1 == x){
		pngImg = new Fl_PNG_Image("img\\22.png");
	}
	else{
		//M-1 == y
		pngImg = new Fl_PNG_Image("img\\25.png");
	}
	image(pngImg);
}

int Box::handle(int e){
	switch (e)
	{	
	case FL_PUSH:
	que = threadsafe_queue::get_instance();
		if(!net_flag){	//本地模式,角色切换
			que->push(current_role, x, y);
			current_role = !(current_role-1) + 1;
		}
		else{	//网络模式
			que->push(current_role, x, y);
		}
		return 1;
	default: return Fl_Box::handle(e);
	}	
}

//public
void Box::change_image(int role){
	if(role == 1){
		if( x&&y && x<N-1 && y<M-1){
			pngImg = new Fl_PNG_Image("img\\2.png");
		}
		else if( 0 == x && 0 == y){
		pngImg = new Fl_PNG_Image("img\\5.png");
		}
		else if( 0 == x && M-1 == y){
			pngImg = new Fl_PNG_Image("img\\8.png");
		}
		else if(N-1 == x && 0 == y){
			pngImg = new Fl_PNG_Image("img\\11.png");
		}
		else if(N-1 ==x && M-1 == y){
			pngImg = new Fl_PNG_Image("img\\14.png");
		}
		
		else if(0 == x){
			pngImg = new Fl_PNG_Image("img\\17.png");
		}
		else if(0 == y){
			pngImg = new Fl_PNG_Image("img\\20.png");
		}
		else if(N-1 == x){
			pngImg = new Fl_PNG_Image("img\\23.png");
		}
		else{
			//M-1 == y
			pngImg = new Fl_PNG_Image("img\\26.png");
		}
	}
	else if(role == 2){
		if( x&&y && x<N-1 && y<M-1){
			pngImg = new Fl_PNG_Image("img\\3.png");
		}
		else if( 0 == x && 0 == y){
		pngImg = new Fl_PNG_Image("img\\6.png");
		}
		else if( 0 == x && M-1 == y){
			pngImg = new Fl_PNG_Image("img\\9.png");
		}
		else if(N-1 == x && 0 == y){
			pngImg = new Fl_PNG_Image("img\\12.png");
		}
		else if(N-1 ==x && M-1 == y){
			pngImg = new Fl_PNG_Image("img\\15.png");
		}
		
		else if(0 == x){
			pngImg = new Fl_PNG_Image("img\\18.png");
		}
		else if(0 == y){
			pngImg = new Fl_PNG_Image("img\\21.png");
		}
		else if(N-1 == x){
			pngImg = new Fl_PNG_Image("img\\24.png");
		}
		else{
			//M-1 == y
			pngImg = new Fl_PNG_Image("img\\27.png");
		}
	}
	image(pngImg);
	redraw();
}










