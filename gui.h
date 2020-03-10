#ifndef GUI_H
#define GUI_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <vector>
#include "Box.h"



class gui : public Fl_Window{
public:
	gui(int N,int M);
	int gui_start();
	void change_image(int role, int x, int y);
	void show_result(int role, int state);
	void show_state(char* s) const;
	void show_role(int role);
	~gui();
private:
	Box *pbox;
	Fl_Box	*notify_state_box, *victory_state_box, *courrent_role_box;
	std::vector<std::vector<Box*> > pbox_table;
	void chessboard_init();
	int N,M;
};

#endif