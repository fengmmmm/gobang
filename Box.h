#ifndef BOX_H
#define BOX_H

#include <FL/Fl_Box.H>
#include <FL/FL_PNG_Image.H>
#include "threadsafe_queue.h"
#include <FL/Fl_ask.H>


class Box : public Fl_Box{
public:
	Box(int x,int y);
	~Box();
	int handle(int e);
	void change_image(int role);
	static void set_space(int N,int M);
	static void set_white();
	static void set_net();
private:
	void insert_image();
	int x,y;
	Fl_PNG_Image* pngImg;
	static int N, M, current_role;
	threadsafe_queue* que;
	static bool net_flag;
};

#endif